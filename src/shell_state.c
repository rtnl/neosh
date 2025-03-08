#include "neo.h"

neo_shell_state_t *neo_shell_state_new() {
  neo_shell_state_t *self;

  self = (neo_shell_state_t *)calloc(1, sizeof(neo_shell_state_t));
  self->input_buffer = ion_buffer_new();
  self->env = neo_map_new();

  return self;
}

void neo_shell_state_free(neo_shell_state_t *self) {
  if (self == NULL)
    return;

  if (self->input_buffer != NULL)
    ion_buffer_free(self->input_buffer);

  if (self->env != NULL)
    neo_map_free(self->env);

  free(self);
}
