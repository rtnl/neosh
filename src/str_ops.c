#include "neo.h"

size_t str_len(uint8_t *str) {
  size_t x;

  if (str == NULL)
    return 0;

  for (x = 0; str[x]; x++)
    ;

  return x;
}

uint8_t str_char(uint8_t *str, uint8_t c) {
  size_t x;

  if (str == NULL)
    return 0;

  for (x = 0; str[x]; x++) {
    if (str[x] == c) {
      return 1;
    }
  }

  return 0;
}

// meow
uint8_t *str_cat(uint8_t *str_a, uint8_t *str_b) {
  uint8_t *r;
  size_t len_a;
  size_t len_b;
  size_t x;

  if (str_a == NULL)
    return NULL;

  if (str_b == NULL)
    return NULL;

  len_a = str_len(str_a);
  len_b = str_len(str_b);

  r = (uint8_t *)calloc(len_a + len_b + 1, sizeof(uint8_t));
  x = 0;

  memcpy(&r[x], str_a, len_a);
  x += len_a;

  memcpy(&r[x], str_b, len_b);
  x += len_b;

  r[x] = '\0';

  return r;
}

uint8_t *str_dup(uint8_t *str, size_t len) {
  uint8_t *r;

  if (str == NULL)
    return NULL;

  r = (uint8_t *)calloc(len + 1, sizeof(uint8_t));
  memcpy(r, str, len);

  return r;
}

uint8_t **str_split(uint8_t *str, uint8_t *separators) {
  uint8_t **r;
  uint8_t *s;
  uint64_t len;
  uint64_t count;
  size_t x;
  size_t y;

  if (str == NULL)
    return NULL;

  len = 0;
  count = 0;
  for (x = 0; str[x]; x++) {
    if (str_char(separators, str[x])) {
      if (count > 0) {
        len++;
      }

      count = 0;
    } else {
      count++;
    }
  }
  if (count > 0)
    len++;

  r = (uint8_t **)calloc(len + 1, sizeof(uint8_t *));
  y = 0;
  count = 0;
  for (x = 0; str[x]; x++) {
    if (str_char(separators, str[x])) {
      if (count > 0) {
        s = str_dup(&str[x - count], count);
        r[y] = s;
        y++;
      }

      count = 0;
    } else {
      count++;
    }
  }
  if (count > 0) {
    s = str_dup(&str[x - count], count);
    r[y] = s;
    y++;
  }
  r[y] = NULL;

  return r;
}

uint8_t *str_join(uint8_t **value, uint8_t *separator) {
  t_ion_result_code result;
  t_ion_vector *vector;
  size_t x;

  if (value == NULL)
    return NULL;

  if (separator == NULL)
    separator = (uint8_t *)"";

  vector = vector_new(sizeof(uint8_t));
  for (x = 0; value[x]; x++) {
    if (x > 0) {
      result = vector_write(vector, separator, str_len(separator));
      if (result != RESULT_OK)
        break;
    }

    result = vector_write(vector, value[x], str_len(value[x]));
    if (result != RESULT_OK)
      break;
  }

  return vector_consume(vector);
}

void str_list_free(uint8_t **l) {
  size_t x;

  if (l == NULL)
    return;

  for (x = 0; l[x]; x++)
    free(l[x]);

  free(l);
}
