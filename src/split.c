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
