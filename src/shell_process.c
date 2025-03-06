#include "neo.h"

neo_result_code_t neo_shell_process_input(neo_shell_t *self) {
  neo_result_code_t result;
  neo_shell_state_t *state;
  uint8_t *input;
  t_ion_object_kind kind;
  uint8_t len;

  if (self == NULL)
    return RESULT_NULL;

  state = self->state;
  if (state == NULL)
    return RESULT_NULL;

  result = ion_buffer_reset_peek(state->input_buffer);
  if (result != RESULT_OK)
    return result;

  result = ion_buffer_io_peek_arr_open(state->input_buffer, &kind, &len);
  if (result != RESULT_OK)
    return result;

  result = ion_buffer_io_peek_arr_check(state->input_buffer, kind, len);
  if (result != RESULT_OK)
    return result;

  input = (uint8_t *)malloc(len + 1);
  memset(input, 0x00, len + 1);

  for (uint8_t x = 0; x < len; x++) {
    result = ion_buffer_io_peek_u8(state->input_buffer, &input[x]);
    if (result != RESULT_OK) {
      free(input);
      return result;
    }
  }

  result = ion_buffer_io_peek_arr_close(state->input_buffer);
  if (result != RESULT_OK) {
    free(input);
    return result;
  }

  result = ion_buffer_seek_read(state->input_buffer,
                                state->input_buffer->body->curr_p);
  if (result != RESULT_OK) {
    free(input);
    return result;
  }

  result = ion_buffer_reduce(state->input_buffer);
  if (result != RESULT_OK) {
    free(input);
    return result;
  }

  printf("input->[%s]\n", input);

  free(input);
  return RESULT_OK;
}
