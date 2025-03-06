#include "neo.h"

neo_shell_t *neo_shell_new() {
  neo_shell_t *self;

  self = (neo_shell_t *)calloc(1, sizeof(neo_shell_t));
  self->prompt_format = (uint8_t *)"neosh >>> ";
  self->state = neo_shell_state_new();

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

  result = neo_shell_io_push_prompt(self);
  if (result != RESULT_OK)
    return result;

  result = neo_shell_io_pull_input(self);
  if (result != RESULT_OK)
    return result;

  return RESULT_OK;
}
