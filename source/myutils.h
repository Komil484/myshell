#pragma once

#include <stdio.h>

enum Mysh_command {
  MYSH_EXEC,
  MYSH_EXIT,
  MYSH_CHDR,
  MYSH_SETV,
};

enum Mysh_command get_command(char *str);
char *substitute_env_vars(const char *in_str);
unsigned long get_arg_count(char **parsed);
char **parse(const char *str);
char *fget_input(FILE *fd);
char *get_input();
void replace_escape_chars(char *str);
void free_parsed(char **parsed);
