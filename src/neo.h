#ifndef NEO_H
#define NEO_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <wait.h>
#include <openssl/sha.h>

#include <ion.h>

#ifdef TEST
#include <criterion/criterion.h>
#endif

typedef t_ion_result_code neo_result_code_t;

typedef struct nen_map_entry_s {
    uint8_t flag_active;
    uint64_t hash;
    uint8_t *key;
    void *data;
    size_t data_size;
} neo_map_entry_t;

typedef struct neo_map_s {
    t_ion_vector *body;
} neo_map_t;

typedef struct neo_shell_state_s {
    t_ion_buffer *input_buffer;
    neo_map_t *env;
} neo_shell_state_t;

typedef struct neo_shell_s {
    uint8_t *prompt_format;

    neo_shell_state_t *state;
} neo_shell_t;

typedef struct neo_shell_exec_entry_s {
    uint8_t flag_valid;
    uint8_t *path;
    uint8_t **args;
} neo_shell_exec_entry_t;

// str_ops.c
size_t str_len(uint8_t *str);
uint8_t str_char(uint8_t *str, uint8_t c);
uint8_t *str_cat(uint8_t *str_a, uint8_t *str_b);
uint8_t *str_dup(uint8_t *str, size_t len);
uint8_t *str_dup_len(uint8_t *str);
uint8_t **str_split(uint8_t *str, uint8_t *separators);
uint8_t *str_join(uint8_t **value, uint8_t *separator);
void str_list_free(uint8_t **l);
uint8_t **str_list_append(uint8_t **l, uint8_t *s);
uint8_t **str_list_insert(uint8_t **l, uint8_t *s);

// map.c
neo_map_entry_t *neo_map_entry_new(uint64_t hash, uint8_t *key, void *data, size_t data_size);
void neo_map_entry_free(neo_map_entry_t *self);
neo_result_code_t neo_map_entry_update(neo_map_entry_t *self, uint64_t hash, uint8_t *key, void *data, size_t data_size);
neo_map_t *neo_map_new();
void neo_map_free(neo_map_t *self);
uint64_t neo_map_hash(neo_map_t *self, uint8_t *key, size_t len);
neo_result_code_t neo_map_get_entry(neo_map_t *self, uint8_t *key, neo_map_entry_t **value);
neo_result_code_t neo_map_insert(neo_map_t *self, uint8_t *key, void *src, size_t len);
neo_result_code_t neo_map_get(neo_map_t *self, uint8_t *key, void *dst, size_t len);
neo_result_code_t neo_map_drop(neo_map_t *self, uint8_t *key);
neo_result_code_t neo_map_list_key(neo_map_t *self, t_ion_vector *output);
neo_result_code_t neo_map_pull(neo_map_t *self, uint8_t *key, t_ion_vector *output);

// shell.c
neo_shell_t *neo_shell_new(int32_t argc, uint8_t **argv, uint8_t **envp);
void neo_shell_free(neo_shell_t *self);
neo_result_code_t neo_shell_run(neo_shell_t *self);
neo_result_code_t neo_shell_run_cycle(neo_shell_t *self);

// shell_state.c
neo_shell_state_t *neo_shell_state_new();
void neo_shell_state_free(neo_shell_state_t *self);

// shell_env.c
neo_result_code_t neo_shell_env_add(neo_shell_t *self, uint8_t *key, uint8_t *value);
neo_result_code_t neo_shell_env_remove(neo_shell_t *self, uint8_t *key);
neo_result_code_t neo_shell_env_pull(neo_shell_t *self, uint8_t *key, t_ion_vector *output);
uint8_t **neo_shell_env_export(neo_shell_t *self);

// shell_io.c
void push_string(int fd, uint8_t *value);
void push_string_stdout(uint8_t *value);
void push_string_stderr(uint8_t *value);
neo_result_code_t neo_shell_io_push_prompt(neo_shell_t *self);
neo_result_code_t neo_shell_io_pull_input(neo_shell_t *self);

// shell_process.c
neo_result_code_t neo_shell_process_input(neo_shell_t *self);
neo_result_code_t neo_shell_process_execute(neo_shell_t *self, neo_shell_exec_entry_t *path);

// shell_exec.c
neo_shell_exec_entry_t *neo_shell_exec_entry_new(uint8_t *path, uint8_t **args);
void neo_shell_exec_entry_free(neo_shell_exec_entry_t *self);
neo_result_code_t neo_shell_derive_exec(neo_shell_t *self, uint8_t **input_split, neo_shell_exec_entry_t ***entry_list_ref);
neo_result_code_t neo_shell_validate_exec(neo_shell_t *self, neo_shell_exec_entry_t *path_entry);

#endif
