#include "neo.h"

neo_result_code_t neo_shell_process_input(neo_shell_t *self) {
  neo_result_code_t result;
  neo_shell_state_t *state;
  uint8_t *input;
  uint8_t **input_split;
  neo_shell_exec_entry_t **exec_list;
  size_t exec_list_len;
  neo_shell_exec_entry_t *exec_choice;
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

  exec_list = NULL;

  result = neo_shell_derive_exec(self, input_split, &exec_list);
  if (result != RESULT_OK)
    return result;

  if (exec_list == NULL)
    return RESULT_NULL;

  if (exec_list[0] == NULL)
    return RESULT_OK;

  exec_choice = NULL;
  exec_list_len = 0;
  for (x = 0; exec_list[x]; x++) {
    exec_list_len++;
  }

  for (x = 0; x < exec_list_len; x++) {
    result = neo_shell_validate_exec(self, exec_list[x]);
    if (result != RESULT_OK) {
      return result;
    }

    if (exec_list[x]->flag_valid == 1) {
      if (exec_choice == NULL)
        exec_choice = exec_list[x];
    } else {
      neo_shell_exec_entry_free(exec_list[x]);
      exec_list[x] = NULL;
    }
  }

  if (exec_choice == NULL) {
    push_string_stderr((uint8_t *)"error: no valid path found\n");
    free(exec_list);
    free(input);
    return RESULT_OK;
  }

  result = neo_shell_process_execute(self, exec_choice);
  if (result != RESULT_OK) {
    free(exec_list);
    free(input);
    return result;
  }

  for (x = 0; x < exec_list_len; x++) {
    if (exec_list[x] == NULL)
      continue;

    neo_shell_exec_entry_free(exec_list[x]);
  }

  free(exec_list);
  free(input);
  return RESULT_OK;
}

neo_result_code_t neo_shell_process_execute(neo_shell_t *self,
                                            neo_shell_exec_entry_t *exec) {
  uint8_t *path_final;
  int status;
  pid_t pid;

  if (self == NULL)
    return RESULT_NULL;

  if (exec == NULL)
    return RESULT_NULL;

  path_final = NULL;

  pid = fork();
  if (pid == -1) {
    neo_shell_exec_entry_free(exec);
    return RESULT_ERROR;
  }
  if (pid == 0) {
    path_final = str_dup(exec->path, str_len(exec->path));
    neo_shell_exec_entry_free(exec);

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

  return RESULT_OK;
}
