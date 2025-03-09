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

neo_result_code_t neo_shell_env_pull(neo_shell_t *self, uint8_t *key,
                                     t_ion_vector *output) {
  neo_result_code_t result;
  neo_shell_state_t *state;
  neo_map_t *env;

  if (self == NULL)
    return RESULT_NULL;

  if (key == NULL)
    return RESULT_NULL;

  if (output == NULL)
    return RESULT_NULL;

  state = self->state;
  if (state == NULL)
    return RESULT_NULL;

  env = state->env;
  if (env == NULL)
    return RESULT_NULL;

  result = neo_map_pull(env, key, output);
  if (result != RESULT_OK)
    return result;

  result = vector_write(output, (uint8_t *)"", 1);
  if (result != RESULT_OK)
    return result;

  return RESULT_OK;
}

uint8_t **neo_shell_env_export(neo_shell_t *self) {
  neo_result_code_t result;
  uint8_t **output;
  size_t output_len;
  t_ion_vector *output_buffer;
  neo_shell_state_t *state;
  neo_map_t *env;
  t_ion_vector *env_key_list;
  uint8_t **env_key_ref;
  uint8_t *env_key;
  size_t x;
  size_t y;

  if (self == NULL)
    return NULL;

  state = self->state;
  if (state == NULL)
    return NULL;

  env = state->env;
  if (env == NULL)
    return NULL;

  env_key_list = vector_new(sizeof(uint8_t *));
  if (env_key_list == NULL)
    return NULL;

  result = neo_map_list_key(env, env_key_list);
  if (result != RESULT_OK) {
    free(vector_consume(env_key_list));
    return NULL;
  }

  output_len = env_key_list->curr_w + 1;
  output = (uint8_t **)calloc(output_len, sizeof(uint8_t *));
  output[output_len - 1] = NULL;

  y = 0;
  for (x = 0; x < env_key_list->curr_w; x++) {
    env_key_ref = NULL;
    env_key = NULL;

    result = vector_get(env_key_list, (void **)&env_key_ref, x);
    if (result != RESULT_OK) {
      free(vector_consume(env_key_list));
      return NULL;
    }

    if (env_key_ref == NULL) {
      continue;
    }

    env_key = *env_key_ref;
    if (env_key == NULL)
      continue;

    output_buffer = vector_new(sizeof(uint8_t));

    result = neo_map_pull(env, env_key, output_buffer);
    if (result != RESULT_OK) {
      free(vector_consume(output_buffer));
      continue;
    }

    output[y] =
        str_cat(env_key, str_cat((uint8_t *)"=",
                                 (uint8_t *)vector_consume(output_buffer)));
    y++;
  }

  return output;
}
