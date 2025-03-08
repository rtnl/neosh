#include "neo.h"

neo_map_entry_t *neo_map_entry_new(uint64_t hash, void *data) {
  neo_map_entry_t *self;

  if (data == NULL)
    return NULL;

  self = (neo_map_entry_t *)calloc(1, sizeof(neo_map_entry_t));
  self->flag_active = 1;
  self->hash = hash;
  self->data = data;

  return self;
}

void neo_map_entry_free(neo_map_entry_t *self) {
  if (self == NULL)
    return;

  if (self->data != NULL)
    free(self->data);

  free(self);
}

neo_result_code_t neo_map_entry_update(neo_map_entry_t *self, uint64_t hash,
                                       void *data) {
  if (self == NULL)
    return RESULT_NULL;

  if (data == NULL)
    return RESULT_NULL;

  self->flag_active = 1;
  self->hash = hash;
  self->data = data;

  return RESULT_OK;
}

neo_map_t *neo_map_new() {
  neo_map_t *self;

  self = (neo_map_t *)calloc(1, sizeof(neo_map_t));
  self->body = vector_new(sizeof(neo_map_entry_t *));

  return self;
}

void neo_map_free(neo_map_t *self) {
  neo_result_code_t result;
  neo_map_entry_t **entry_ptr;
  neo_map_entry_t *entry;
  size_t x;

  if (self == NULL)
    return;

  if (self->body != NULL) {
    for (x = 0; x < self->body->curr_w; x++) {
      entry_ptr = NULL;
      entry = NULL;

      result = vector_get(self->body, (void **)&entry_ptr, x);
      if (result != RESULT_OK)
        continue;

      if (entry_ptr == NULL)
        continue;

      entry = *entry_ptr;
      if (entry == NULL)
        continue;

      neo_map_entry_free(entry);
    }

    free(vector_consume(self->body));
  }

  free(self);
}

uint64_t neo_map_hash(neo_map_t *self, uint8_t *key, size_t len) {
  uint8_t hash[SHA_DIGEST_LENGTH] = {0};
  uint64_t hash_64;

  if (self == NULL)
    return 0;

  if (key == NULL)
    return 0;

  if (len < 1)
    return 0;

  SHA1(key, len, hash);
  memcpy(&hash_64, hash, sizeof(uint64_t));

  return hash_64;
}

neo_result_code_t neo_map_insert(neo_map_t *self, uint8_t *key, void *src,
                                 size_t len) {
  neo_result_code_t result;
  t_ion_vector *body;
  uint8_t flag_found;
  neo_map_entry_t **entry_ptr;
  neo_map_entry_t *entry;
  uint64_t hash;
  void *data;
  size_t x;

  if (self == NULL)
    return RESULT_NULL;

  if (key == NULL)
    return RESULT_NULL;

  if (src == NULL)
    return RESULT_NULL;

  if (len < 1) {
    return RESULT_ERROR;
  }

  body = self->body;
  if (body == NULL)
    return RESULT_NULL;

  hash = neo_map_hash(self, key, str_len(key));
  if (hash == 0)
    return RESULT_ERROR;

  data = calloc(1, len);
  memcpy(data, src, len);

  flag_found = 0;

  for (x = 0; x < body->curr_w; x++) {
    entry_ptr = NULL;
    entry = NULL;

    result = vector_get(body, (void **)&entry_ptr, x);
    if (result != RESULT_OK)
      return result;

    if (entry_ptr == NULL)
      return RESULT_NULL;

    entry = *entry_ptr;
    if (entry == NULL)
      return RESULT_NULL;

    if (entry->flag_active)
      continue;

    result = neo_map_entry_update(entry, hash, data);
    if (result != RESULT_OK)
      return result;

    flag_found = 1;
    break;
  }

  if (!flag_found) {
    entry = neo_map_entry_new(hash, data);

    result = vector_write(body, &entry, 1);
    if (result != RESULT_OK)
      return result;
  }

  return RESULT_OK;
}
neo_result_code_t neo_map_get(neo_map_t *self, uint8_t *key, void *dst,
                              size_t len) {
  neo_result_code_t result;
  t_ion_vector *body;
  uint64_t hash;
  neo_map_entry_t **entry_ptr;
  neo_map_entry_t *entry;
  size_t x;

  if (self == NULL)
    return RESULT_NULL;

  if (key == NULL)
    return RESULT_NULL;

  if (dst == NULL)
    return RESULT_NULL;

  if (len < 1) {
    return RESULT_ERROR;
  }

  body = self->body;
  if (body == NULL)
    return RESULT_NULL;

  hash = neo_map_hash(self, key, str_len(key));
  if (hash == 0)
    return RESULT_ERROR;

  for (x = 0; x < body->curr_w; x++) {
    entry_ptr = NULL;
    entry = NULL;

    result = vector_get(body, (void **)&entry_ptr, x);
    if (result != RESULT_OK)
      return result;

    if (entry_ptr == NULL)
      return RESULT_NULL;

    entry = *entry_ptr;
    if (entry == NULL)
      return RESULT_NULL;

    if (!entry->flag_active)
      continue;

    if (entry->hash != hash)
      continue;

    memcpy(dst, entry->data, len);

    break;
  }

  return RESULT_OK;
}

neo_result_code_t neo_map_drop(neo_map_t *self, uint8_t *key) {
  neo_result_code_t result;
  t_ion_vector *body;
  neo_map_entry_t **entry_ptr;
  neo_map_entry_t *entry;
  uint64_t hash;
  size_t x;

  if (self == NULL)
    return RESULT_NULL;

  if (key == NULL)
    return RESULT_NULL;

  body = self->body;
  if (body == NULL)
    return RESULT_NULL;

  hash = neo_map_hash(self, key, str_len(key));
  if (hash == 0)
    return RESULT_ERROR;

  for (x = 0; x < body->curr_w; x++) {
    entry_ptr = NULL;
    entry = NULL;

    result = vector_get(body, (void **)&entry_ptr, x);
    if (result != RESULT_OK)
      return result;

    if (entry_ptr == NULL)
      return RESULT_NULL;

    entry = *entry_ptr;
    if (entry == NULL)
      return RESULT_NULL;

    if (!entry->flag_active)
      continue;

    if (entry->hash != hash)
      continue;

    entry->flag_active = 0;
    entry->hash = 0;
    if (entry->data != NULL)
      free(entry->data);

    break;
  }

  return RESULT_OK;
}
