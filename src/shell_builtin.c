#include "neo.h"

neo_shell_command_ctx_t *neo_shell_command_ctx_new(uint8_t **args) {
  neo_shell_command_ctx_t *self;

  self = calloc(1, sizeof(neo_shell_command_ctx_t));
  if (self == NULL)
    return NULL;

  self->args = args;

  return self;
}

void neo_shell_command_ctx_free(neo_shell_command_ctx_t *self) {
  if (self == NULL)
    return;

  free(self);
}

neo_shell_command_t *neo_shell_command_new(
    uint8_t *key,
    neo_result_code_t (*run)(neo_shell_command_t *self, neo_shell_t *shell,
                             neo_shell_command_ctx_t *ctx)) {
  neo_shell_command_t *self;

  self = calloc(1, sizeof(neo_shell_command_t));
  if (self == NULL)
    return NULL;

  self->key = key;
  self->run = run;

  return self;
}

void neo_shell_command_free(neo_shell_command_t *self) {
  if (self == NULL)
    return;

  free(self);
}

neo_result_code_t neo_builtin__change_directory(neo_shell_command_t *self,
                                                neo_shell_t *shell,
                                                neo_shell_command_ctx_t *ctx) {
  neo_result_code_t result;

  if (self == NULL)
    return RESULT_NULL;

  if (shell == NULL)
    return RESULT_NULL;

  if (ctx == NULL)
    return RESULT_NULL;

  push_string_stdout((uint8_t *)"hello!\n");

  return RESULT_OK;
}
