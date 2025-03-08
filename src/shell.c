#include "neo.h"

neo_shell_t *neo_shell_new(int32_t argc, uint8_t **argv, uint8_t **envp) {
  neo_result_code_t result;
  neo_shell_t *self;
  uint8_t *arg;
  uint8_t *env;
  uint8_t **env_split;
  int32_t x;
  size_t y;

  self = (neo_shell_t *)calloc(1, sizeof(neo_shell_t));
  self->prompt_format = (uint8_t *)"neosh >>> ";
  self->state = neo_shell_state_new();

  for (x = 0; x < argc; x++) {
    arg = argv[x];
    if (arg == NULL)
      continue;
  }

  for (x = 0; envp[x]; x++) {
    env = envp[x];
    if (env == NULL)
      continue;

    env_split = str_split(env, (uint8_t *)"=");
    if (env_split == NULL)
      continue;

    if (env_split[0] == NULL || env_split[1] == NULL) {
      free(env_split);
      continue;
    }

    result = neo_shell_env_add(self, env_split[0], env_split[1]);
    if (result != RESULT_OK) {
      push_string_stderr((uint8_t *)"failed at loading env");
    }

    for (y = 0; env_split[y]; y++) {
      free(env_split[y]);
    }
    free(env_split);
  }

  return self;
}

void neo_shell_free(neo_shell_t *self) {
  if (self == NULL)
    return;

  if (self->state != NULL)
    neo_shell_state_free(self->state);

  free(self);
}

neo_result_code_t neo_shell_run(neo_shell_t *self) {
  neo_result_code_t result;

  if (self == NULL)
    return RESULT_NULL;

  while (1) {
    result = neo_shell_run_cycle(self);
    if (result != RESULT_OK)
      return result;
  }

  return RESULT_OK;
}

neo_result_code_t neo_shell_run_cycle(neo_shell_t *self) {
  neo_result_code_t result;

  if (self == NULL)
    return RESULT_NULL;

  result = neo_shell_io_pull_input(self);
  if (result != RESULT_OK)
    return result;

  result = neo_shell_process_input(self);
  if (result != RESULT_OK)
    return result;

  return RESULT_OK;
}
