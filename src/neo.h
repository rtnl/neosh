#ifndef NEO_H
#define NEO_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <wait.h>

#include <ion.h>

#ifdef TEST
#include <criterion/criterion.h>
#endif

typedef t_ion_result_code neo_result_code_t;

typedef struct neo_shell_state_s {
    t_ion_buffer *input_buffer;
} neo_shell_state_t;

typedef struct neo_shell_s {
    uint8_t *prompt_format;

    neo_shell_state_t *state;
} neo_shell_t;

typedef struct neo_shell_path_entry_s {
    uint8_t flag_valid;
    uint8_t *path;
} neo_shell_path_entry_t;

// str_ops.c
size_t str_len(uint8_t *str);
uint8_t str_char(uint8_t *str, uint8_t c);
uint8_t *str_cat(uint8_t *str_a, uint8_t *str_b);
uint8_t *str_dup(uint8_t *str, size_t len);
uint8_t **str_split(uint8_t *str, uint8_t *separators);
uint8_t *str_join(uint8_t **value, uint8_t *separator);

// shell.c
neo_shell_t *neo_shell_new();
void neo_shell_free(neo_shell_t *self);
neo_result_code_t neo_shell_run(neo_shell_t *self);
neo_result_code_t neo_shell_run_cycle(neo_shell_t *self);

// shell_state.c
neo_shell_state_t *neo_shell_state_new();
void neo_shell_state_free(neo_shell_state_t *self);

// shell_io.c
void push_string(int fd, uint8_t *value);
void push_string_stdout(uint8_t *value);
void push_string_stderr(uint8_t *value);
neo_result_code_t neo_shell_io_push_prompt(neo_shell_t *self);
neo_result_code_t neo_shell_io_pull_input(neo_shell_t *self);

// shell_process.c
neo_result_code_t neo_shell_process_input(neo_shell_t *self);
neo_result_code_t neo_shell_process_execute(neo_shell_t *self, neo_shell_path_entry_t *path);

// shell_path.c
neo_shell_path_entry_t *neo_shell_path_entry_new(uint8_t *path);
void neo_shell_path_entry_free(neo_shell_path_entry_t *self);
neo_result_code_t neo_shell_derive_path(neo_shell_t *self, uint8_t **input_split, neo_shell_path_entry_t ***entry_list_ref);
neo_result_code_t neo_shell_validate_path(neo_shell_t *self, neo_shell_path_entry_t *path_entry);

#endif
