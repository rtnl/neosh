#ifndef NEO_H
#define NEO_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

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

// split.c
uint8_t **str_split(uint8_t *str, uint8_t *separators);

// shell.c
neo_shell_t *neo_shell_new();
void neo_shell_free(neo_shell_t *self);
neo_result_code_t neo_shell_run(neo_shell_t *self);
neo_result_code_t neo_shell_run_cycle(neo_shell_t *self);

// shell_state.c
neo_shell_state_t *neo_shell_state_new();
void neo_shell_state_free(neo_shell_state_t *self);

// shell_io.c
neo_result_code_t neo_shell_io_push_prompt(neo_shell_t *self);
neo_result_code_t neo_shell_io_pull_input(neo_shell_t *self);

// shell_process.c
neo_result_code_t neo_shell_process_input(neo_shell_t *self);

#endif
