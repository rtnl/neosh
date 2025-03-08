#include "neo.h"

neo_result_code_t neo_shell_process_input(neo_shell_t *self) {
  neo_result_code_t result;
  neo_shell_state_t *state;
  uint8_t *input;
  uint8_t **input_split;
  neo_shell_path_entry_t **path_list;
  neo_shell_path_entry_t *path_choice;
  t_ion_object_kind kind;
  uint8_t len;
  size_t x;

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

  input_split = str_split(input, (uint8_t *)" ");
  if (input_split == NULL)
    return RESULT_ERROR;

  if (input_split[0] == NULL) {
    free(input_split);
    free(input);
    return RESULT_OK;
  }

  path_list = NULL;

  result = neo_shell_derive_path(self, input_split, &path_list);
  if (result != RESULT_OK)
    return result;

  if (path_list == NULL)
    return RESULT_NULL;

  if (path_list[0] == NULL)
    return RESULT_OK;

  path_choice = NULL;
  for (x = 0; path_list[x]; x++) {
    result = neo_shell_validate_path(self, path_list[x]);
    if (result != RESULT_OK) {
      return result;
    }

    if (path_list[x]->flag_valid == 1) {
      path_choice = path_list[x];
    } else {
      neo_shell_path_entry_free(path_list[x]);
    }
  }

  free(path_list);

  if (path_choice == NULL) {
    push_string_stdout((uint8_t *)"error: no valid path found\n");
    free(input);
    return RESULT_OK;
  }

  result = neo_shell_process_execute(self, path_choice);
  if (result != RESULT_OK) {
    return result;
  }

  free(input);
  return RESULT_OK;
}

neo_result_code_t neo_shell_process_execute(neo_shell_t *self,
                                            neo_shell_path_entry_t *path) {
  uint8_t *path_final;
  int status;
  pid_t pid;

  if (self == NULL)
    return RESULT_NULL;

  if (path == NULL)
    return RESULT_NULL;

  path_final = NULL;

  pid = fork();
  if (pid == -1) {
    neo_shell_path_entry_free(path);
    return RESULT_ERROR;
  }
  if (pid == 0) {
    path_final = str_dup(path->path, str_len(path->path));
    neo_shell_path_entry_free(path);

    if (path_final == NULL)
      return RESULT_NULL;

    char *args[] = {
        (char *)str_dup(path_final, str_len(path_final)),
        NULL,
    };

    char **envp = (char **)neo_shell_env_export(self);

    if (execve((char *)path_final, args, envp) == -1) {
      return RESULT_ERROR;
    }
  } else {
    waitpid(pid, &status, 0);
  }

  neo_shell_path_entry_free(path);
  return RESULT_OK;
}
