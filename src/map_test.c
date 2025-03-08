#include "neo.h"

#ifdef TEST

Test(map, map) {
  neo_result_code_t result;
  neo_map_t *map;
  uint8_t data[1024];
  uint8_t empty[1024];

  map = neo_map_new();
  cr_assert(map != NULL);

  setbuf(stdout, 0);

  result = neo_map_insert(map, "key1", "VALUE_1", 7);
  cr_assert(result == RESULT_OK);

  result = neo_map_insert(map, "key2", "VALUE_2", 7);
  cr_assert(result == RESULT_OK);

  memset(data, 0x00, 1024);
  result = neo_map_get(map, "key1", data, 7);
  cr_assert(result == RESULT_OK);
  cr_assert(memcmp(data, "VALUE_1", 7) == 0);

  memset(data, 0x00, 1024);
  result = neo_map_get(map, "key2", data, 7);
  cr_assert(result == RESULT_OK);
  cr_assert(memcmp(data, "VALUE_2", 7) == 0);

  memset(data, 0x00, 1024);
  result = neo_map_get(map, "key1", data, 7);
  cr_assert(result == RESULT_OK);
  cr_assert(memcmp(data, "VALUE_1", 7) == 0);

  memset(data, 0x00, 1024);
  result = neo_map_get(map, "key2", data, 7);
  cr_assert(result == RESULT_OK);
  cr_assert(memcmp(data, "VALUE_2", 7) == 0);

  result = neo_map_drop(map, "key2");
  cr_assert(result == RESULT_OK);

  memset(data, 0x00, 1024);
  memset(empty, 0x00, 1024);
  result = neo_map_get(map, "key2", data, 7);
  cr_assert(result == RESULT_OK);
  cr_assert(memcmp(data, empty, 1024) == 0);
}

#endif
