#include "neo.h"

const size_t BUFFER_SIZE = 1024;

void push_string(int fd, uint8_t *value) {
  write(fd, value, strlen((char *)value));
}

void push_string_stdout(uint8_t *value) { push_string(STDOUT_FILENO, value); }

void push_string_stderr(uint8_t *value) { push_string(STDERR_FILENO, value); }

neo_result_code_t neo_shell_io_push_prompt(neo_shell_t *self) {
  if (self == NULL)
    return RESULT_NULL;

  if (isatty(STDOUT_FILENO) && isatty(STDIN_FILENO)) {
    push_string_stdout(self->prompt_format);
  }

  return RESULT_OK;
}

neo_result_code_t neo_shell_io_pull_input(neo_shell_t *self) {
  neo_result_code_t result;
  uint8_t buffer[BUFFER_SIZE];
  ssize_t size;
  uint8_t flag;
  uint8_t c;

  if (self == NULL)
    return RESULT_NULL;

  flag = 1;

  result = neo_shell_io_push_prompt(self);
  if (result != RESULT_OK)
    return result;

  while (flag) {
    memset(buffer, 0x00, BUFFER_SIZE);

    size = read(STDIN_FILENO, buffer, BUFFER_SIZE);
    if (size == -1) {
      return RESULT_IO_ERROR;
    }
    if (size == 0) {
      return RESULT_IO_CLOSED;
    }

    c = buffer[size - 1];
    switch (c) {
    case 0x0A: {
      if (size > 1) {
        result = ion_buffer_io_write_arr_open(self->state->input_buffer, U8);
        if (result != RESULT_OK)
          return result;

        for (ssize_t x = 0; x < (size - 1); x++) {
          result = ion_buffer_io_write_u8(self->state->input_buffer, buffer[x]);
          if (result != RESULT_OK)
            return result;
        }

        result = ion_buffer_io_write_arr_close(self->state->input_buffer);
        if (result != RESULT_OK)
          return result;

        flag = 0;
      } else {
        result = neo_shell_io_push_prompt(self);
        if (result != RESULT_OK)
          return result;

        flag = 1;
      }
      break;
    }
    }
  }

  return RESULT_OK;
}
