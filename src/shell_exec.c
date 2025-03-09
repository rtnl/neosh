#include "neo.h"

neo_shell_exec_entry_t *neo_shell_exec_entry_new(uint8_t *path,
                                                 uint8_t **args) {
  neo_shell_exec_entry_t *self;
  size_t x;

  if (path == NULL)
    return NULL;

  self = (neo_shell_exec_entry_t *)calloc(1, sizeof(neo_shell_exec_entry_t));
  self->flag_valid = 0;
  self->path = path;

  for (x = 0; args[x]; x++)
    ;
  self->args = calloc(x + 1, sizeof(uint8_t *));
  for (x = 0; args[x]; x++)
    self->args[x] = str_dup(args[x], str_len(args[x]));
  self->args[x] = NULL;

  return self;
}

void neo_shell_exec_entry_free(neo_shell_exec_entry_t *self) {
  size_t x;

  if (self == NULL)
    return;

  if (self->path != NULL)
    free(self->path);

  if (self->args != NULL) {
    for (x = 0; self->args[x]; x++) {
      free(self->args[x]);
    }

    free(self->args);
  }

  free(self);
}

neo_result_code_t
neo_shell_derive_exec(neo_shell_t *self, uint8_t **input_split,
                      neo_shell_exec_entry_t ***entry_list_ref) {
  neo_result_code_t result;
  neo_shell_state_t *state;
  size_t input_split_len;
  neo_shell_exec_entry_t **entry_list;
  size_t entry_list_len;
  t_ion_vector *path_env_vector;
  uint8_t *path_env;
  uint8_t **path_list;
  size_t path_list_len;
  t_ion_vector *path_vector;
  uint8_t *input_head;
  size_t x;

  if (self == NULL)
    return RESULT_NULL;

  if (input_split == NULL)
    return RESULT_NULL;

  if (entry_list_ref == NULL)
    return RESULT_NULL;

  state = self->state;
  if (state == NULL)
    return RESULT_NULL;

  path_env_vector = vector_new(sizeof(uint8_t));
  result = neo_shell_env_pull(self, (uint8_t *)"PATH", path_env_vector);
  if (result != RESULT_OK)
    return result;

  path_env = vector_consume(path_env_vector);
  if (path_env == NULL)
    return RESULT_NULL;

  path_list = str_split(path_env, (uint8_t *)":");
  if (path_list == NULL)
    return RESULT_NULL;

  free(path_env);

  if (path_list[0] == NULL) {
    path_list = str_list_append(path_list, (uint8_t *)"/bin");
    path_list = str_list_append(path_list, (uint8_t *)"/sbin");
    path_list = str_list_append(path_list, (uint8_t *)"/usr/bin");
    path_list = str_list_append(path_list, (uint8_t *)"/usr/sbin");
    path_list = str_list_append(path_list, (uint8_t *)"/usr/local/bin");
    path_list = str_list_append(path_list, (uint8_t *)"/usr/local/sbin");
  }

  path_list_len = 0;
  for (x = 0; path_list[x]; x++)
    path_list_len++;

  for (input_split_len = 0; input_split[input_split_len]; input_split_len++)
    ;

  input_head = input_split[0];
  if (input_head == NULL) {
    str_list_free(path_list);
    return RESULT_NULL;
  }

  entry_list_len = path_list_len + 3;
  entry_list = (neo_shell_exec_entry_t **)calloc(
      entry_list_len, sizeof(neo_shell_exec_entry_t *));
  for (x = 0; path_list[x]; x++) {
    path_vector = vector_new(sizeof(uint8_t));

    result = vector_write(path_vector, path_list[x], str_len(path_list[x]));
    if (result != RESULT_OK)
      return result;

    result = vector_write(path_vector, "/", 1);
    if (result != RESULT_OK)
      return result;

    result = vector_write(path_vector, input_head, str_len(input_head));
    if (result != RESULT_OK)
      return result;

    entry_list[2 + x] =
        neo_shell_exec_entry_new(vector_consume(path_vector), input_split);
  }

  // Todo: resolve relative path
  entry_list[0] = neo_shell_exec_entry_new(
      str_dup(input_head, str_len(input_head)), input_split);
  entry_list[1] = neo_shell_exec_entry_new(
      str_dup(input_head, str_len(input_head)), input_split);

  entry_list[entry_list_len - 1] = NULL;

  *entry_list_ref = entry_list;

  str_list_free(path_list);
  return RESULT_OK;
}

neo_result_code_t neo_shell_validate_exec(neo_shell_t *self,
                                          neo_shell_exec_entry_t *exec) {
  if (self == NULL)
    return RESULT_NULL;

  if (exec == NULL)
    return RESULT_NULL;

  exec->flag_valid = access((char *)exec->path, X_OK) == 0;

  return RESULT_OK;
}
