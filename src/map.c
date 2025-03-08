#include "neo.h"

neo_map_entry_t *neo_map_entry_new(uint64_t hash, void *data) {
  neo_map_entry_t *self;

  if (data == NULL)
    return NULL;

  self = (neo_map_entry_t *)calloc(1, sizeof(neo_map_entry_t));
  self->hash = hash;
  self->data = data;

  return self;
}

neo_result_code_t neo_map_entry_update(neo_map_entry_t *self, uint64_t hash,
                                       void *data) {
  if (self == NULL)
    return RESULT_NULL;

  if (data == NULL)
    return RESULT_NULL;

  self->hash = hash;
  self->data = data;

  return RESULT_OK;
}

neo_map_t *neo_map_new() {
  neo_map_t *self;

  self = (neo_map_t *)calloc(1, sizeof(neo_map_t));
  self->body = vector_new(sizeof(neo_map_entry_t));

  return self;
}

void neo_map_free(neo_map_t *self) {
  if (self == NULL)
    return;

  if (self->body != NULL)
    free(vector_consume(self->body));

  free(self);
}

neo_result_code_t neo_map_insert(neo_map_t *self, uint8_t *key, void *src,
                                 size_t len) {
  neo_result_code_t result;
  t_ion_vector *body;
  uint8_t flag_found;
  neo_map_entry_t **entry_ptr;
  neo_map_entry_t *entry;
  uint8_t hash[SHA_DIGEST_LENGTH] = {0};
  uint64_t hash_64;
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

  flag_found = 0;

  SHA1(key, str_len(key), hash);
  memcpy(&hash_64, hash, sizeof(uint64_t));
  data = calloc(1, len);
  memcpy(data, src, len);

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

    result = neo_map_entry_update(entry, hash_64, data);
    if (result != RESULT_OK)
      return result;

    flag_found = 1;
    break;
  }

  if (!flag_found) {
    entry = neo_map_entry_new(hash_64, data);
  }

  return RESULT_OK;
}
neo_result_code_t neo_map_get(neo_map_t *self, uint8_t *key, void *dst,
                              size_t len) {
  neo_result_code_t result;
  t_ion_vector *body;
  uint8_t hash[SHA_DIGEST_LENGTH] = {0};
  uint64_t hash_64;
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

  SHA1(key, str_len(key), hash);
  memcpy(&hash_64, hash, sizeof(uint64_t));

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

    if (entry->hash != hash_64)
      continue;

    memcpy(dst, entry->data, len);

    break;
  }

  return RESULT_OK;
}
