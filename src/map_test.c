#include "neo.h"

#ifdef TEST

Test(map, map) {
  neo_result_code_t result;
  neo_map_t *map;
  uint8_t data[1024];

  map = neo_map_new();
  cr_assert(map != NULL);

  result = neo_map_insert(map, "key1", "VALUE_1", 7);
  cr_assert(result == RESULT_OK);

  result = neo_map_insert(map, "key2", "VALUE_2", 7);
  cr_assert(result == RESULT_OK);

  memset(data, 0x00, 1024);
  result = neo_map_get(map, "key1", data, 7);
  cr_assert(result == RESULT_OK);
}

#endif
