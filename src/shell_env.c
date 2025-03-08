#include "neo.h"

neo_result_code_t neo_shell_env_add(neo_shell_t *self, uint8_t *key,
                                    uint8_t *value) {
  neo_result_code_t result;
  neo_shell_state_t *state;
  neo_map_t *env;

  if (self == NULL)
    return RESULT_NULL;

  if (key == NULL)
    return RESULT_NULL;

  if (value == NULL)
    return RESULT_NULL;

  state = self->state;
  if (state == NULL)
    return RESULT_NULL;

  env = state->env;
  if (env == NULL)
    return RESULT_NULL;

  result = neo_map_insert(env, key, value, str_len(value));
  if (result != RESULT_OK)
    return result;

  return RESULT_OK;
}

neo_result_code_t neo_shell_env_remove(neo_shell_t *self, uint8_t *key) {
  neo_result_code_t result;
  neo_shell_state_t *state;
  neo_map_t *env;

  if (self == NULL)
    return RESULT_NULL;

  if (key == NULL)
    return RESULT_NULL;

  state = self->state;
  if (state == NULL)
    return RESULT_NULL;

  env = state->env;
  if (env == NULL)
    return RESULT_NULL;

  result = neo_map_drop(env, key);
  if (result != RESULT_OK)
    return result;

  return RESULT_OK;
}
