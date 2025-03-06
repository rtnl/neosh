#include "neo.h"

int main() {
  neo_shell_t *shell;

  shell = neo_shell_new();

  neo_shell_run(shell);
  neo_shell_free(shell);

  return 0;
}
