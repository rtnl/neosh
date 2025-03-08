#include "neo.h"

#ifndef TEST

int32_t main(int32_t argc, char **argv, char **envp) {
  neo_shell_t *shell;

  shell = neo_shell_new(argc, (uint8_t **)argv, (uint8_t **)envp);

  neo_shell_run(shell);
  neo_shell_free(shell);

  return 0;
}

#endif
