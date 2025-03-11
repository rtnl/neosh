#include "neo.h"

neo_shell_state_t *neo_shell_state_new() {
  neo_shell_state_t *self;

  self = (neo_shell_state_t *)calloc(1, sizeof(neo_shell_state_t));
  self->input_buffer = ion_buffer_new();
  self->env = neo_map_new();

  self->dir = calloc(PATH_MAX, sizeof(uint8_t));
  self->dir = (uint8_t *)getcwd((char *)self->dir, PATH_MAX);
  if (self->dir == NULL)
    push_string_stderr((uint8_t *)"failed at loading current directory");

  self->commands =
      (neo_shell_command_t **)calloc(2, sizeof(neo_shell_command_t *));
  self->commands[0] = neo_shell_command_new((uint8_t *)"Change-Directory",
                                            &neo_builtin__change_directory);
  self->commands[1] = NULL;

  return self;
}

void neo_shell_state_free(neo_shell_state_t *self) {
  size_t x;

  if (self == NULL)
    return;

  if (self->input_buffer != NULL)
    ion_buffer_free(self->input_buffer);

  if (self->env != NULL)
    neo_map_free(self->env);

  if (self->dir != NULL)
    free(self->dir);

  if (self->commands != NULL) {
    for (x = 0; self->commands[x]; x++) {
      neo_shell_command_free(self->commands[x]);
    }

    free(self->commands);
  }

  free(self);
}
