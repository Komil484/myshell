#pragma once

#include <stdio.h>

char **parse(char *str);
char *fget_input(FILE *fd);
char *get_input();
void replace_escape_chars(char *str);
