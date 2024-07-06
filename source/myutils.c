#include "myutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// exits the program if memory allocation failure occurs
void exit_if_memory_error(void *new_memory)
{
    if (new_memory) return;
    fprintf(stderr, "Error: Failed to allocate sufficient memory");
    exit(1);
}

enum Mysh_command get_command(char *str)
{
    if (strcmp(str, "cd") == 0)   return MYSH_CHDR;
    if (strcmp(str, "var") == 0)  return MYSH_EVAR;
    if (strcmp(str, "exit") == 0) return MYSH_EXIT;
    return MYSH_EXEC;
}

// reads from c-string and finds the matching quote
// returns pointer to the closing quote
const char *get_closing_quote_ptr(const char *read)
{
    const char quotes = *read;
    const char *read_ptr = read;

    ++read_ptr;
    char c = *read_ptr;
    while (c != quotes && c != '\0') {
        if (c == '\\' && *(read_ptr + 1) == quotes) {
            ++read_ptr;
        }
        ++read_ptr;
        c = *read_ptr;
    }
        if (c == quotes) {
        ++read_ptr;
    }

    return read_ptr;
}

// gets the identifier name starting from str
// returns a malloced copy of identifier name
char *get_var_name(const char *str)
{
    char c = *str;
    if (!isalpha(*str) && *str != '_') return NULL;

    const char *end = str + 1;
    while ((c = *end), isalnum(c) || c == '_') ++end;

    unsigned long len = end - str;
    char *name = malloc(len + 1);
    exit_if_memory_error(name);
    strncpy(name, str, len);
    name[len] = '\0';

    return name;
}

void str_append_char(char **str, char c, char **end, char **write)
{
    if (*write > *end) {
        unsigned long write_offset = *write - *str;
        unsigned long size = *end - *str + 1;
        *str = realloc(*str, size * 2);
        exit_if_memory_error(*str);
        *end = *str + size * 2 - 1;
        *write = *str + write_offset;
    }

    **write = c;
    (*write) += 1;
}

// creates a new c-string, does not modify "in_str"
// replaces environment variables with values in backticks
// e.g. "$VARIABLE" -> "`value`"
char *substitute_env_vars(const char *in_str)
{
    unsigned long in_str_len = strlen(in_str);
    char *str = malloc(in_str_len + 1);
    exit_if_memory_error(str);
    char *str_end = str + in_str_len;
    
    char c;
    const char *read = in_str;
    char *write = str;
    bool prev_char_space = true;

    while ((c = *read)) {
        switch (c) {
            case '\'': case '\"': case '`':
                const char *end = get_closing_quote_ptr(read);
                while (read < end) {
                    str_append_char(&str, *read, &str_end, &write);
                    ++read;
                }
                break;

            case '$':
                ++read;
                char *var_name = get_var_name(read);
                if (var_name == NULL) break;

                read += strlen(var_name);
                char *value = getenv(var_name);
                free(var_name);
                if (value == NULL) break;

                char c, *read_val = value;

                str_append_char(&str, '`', &str_end, &write);
                while ((c = *read_val)) {
                    if (c == '`' || c == '\\') str_append_char(&str, '\\', &str_end, &write);
                    str_append_char(&str, c, &str_end, &write);
                    ++read_val;
                }
                str_append_char(&str, '`', &str_end, &write);
                break;

            default:
                str_append_char(&str, *read, &str_end, &write);
                ++read;
                break;
        }
    }

    str_append_char(&str, '\0', &str_end, &write);
    return str;
}

// replace common escape char sequences such as "\n" with respective char 0x0A (10)
// modifies *write and *read
void replace_if_escape(char **read, char **write)
{
    char *read_ptr = *read;
    char *write_ptr = *write;

    if (*read_ptr == '\\' && *(read_ptr + 1) != '\0') {
        switch (*(read_ptr + 1)) {
            case 'n':
                *write_ptr = '\n';
                break;
            case 't':
                *write_ptr = '\t';
                break;
            case 'r':
                *write_ptr = '\r';
                break;
            case '\\':
                *write_ptr = '\\';
                break;
            case '\"':
                *write_ptr = '\"';
                break;
            case '\'':
                *write_ptr = '\'';
                break;
            case '`':
                *write_ptr = '`';
                break;
            default:
                *write_ptr = *read_ptr;
                ++write_ptr;
                *write_ptr = *(read_ptr + 1);
                break;
        }
        read_ptr += 2;
    } else {
        *write_ptr = *read_ptr;
        ++read_ptr;
    }
    ++write_ptr;

    *read = read_ptr;
    *write = write_ptr;
}

// reallocates null terminated array of c-strings if full
void realloc_if_full(char **args, unsigned long arr_len, unsigned long *cap)
{
    unsigned long local_cap = *cap;
    if (arr_len < local_cap) return;

    args = realloc(args, sizeof(char*) * local_cap * 2);
    exit_if_memory_error(args);
    local_cap *= 2;
    for (unsigned long i = arr_len; i < local_cap; ++i) {
        args[i] = NULL;
    }

    *cap = local_cap;
}

// counts the amount of arguments in a null terminated array of c-strings
unsigned long get_arg_count(char **parsed)
{
    unsigned long count = 0;
    for (; parsed[count]; ++count);
    return count;
}

// returns null terminated array of c-strings
// creates copy of str, does not modify str
// free resulting array of strings using "free_parsed()"
char **parse(const char *in_str)
{
    const unsigned int INITIAL_CAP = 8;

    char *str = substitute_env_vars(in_str);

    unsigned long arr_len = 2, cap = INITIAL_CAP;
    char **args = calloc(INITIAL_CAP, sizeof(char*));
    exit_if_memory_error(args);
    args[0] = str;

    char c;
    char *read = str;
    char *write = str;
    bool prev_char_space = true;

    while ((c = *read)) {
        if (c != ' ') prev_char_space = false;

        switch (c) {
            case ' ':
                ++read;
                if (prev_char_space) break;

                realloc_if_full(args, arr_len, &cap);
                *write++ = '\0';
                args[arr_len - 1] = write; // using write (not read), as it is the resulting string
                ++arr_len;
                break;

            case '\"':
                ++read;
                c = *read;
                while(c != '\"' && c != '\0') {
                    replace_if_escape(&read, &write);
                    c = *read;
                }
                if (c == '\"') {
                    ++read;
                }
                break;

            case '\'':
                ++read;
                c = *read;
                while(c != '\'' && c != '\0') {
                    *write++ = *read;
                    ++read;
                    c = *read;
                }
                if (c == '\'') {
                    ++read;
                }
                break;

            case '`':
                ++read;
                c = *read;
                while(c != '`' && c != '\0') {
                    if (c == '\\' && (read[1] == '`' || read[1] == '\\')) {
                        ++read;
                    }
                    *write++ = *read;
                    ++read;
                    c = *read;
                }
                if (c == '`') {
                    ++read;
                }
                break;

            default:
                *write++ = *read;
                ++read;
                break;
        }
    }

    *write = '\0';

    return args;
}

// frees string array returned by "parse()"
void free_parsed(char **parsed)
{
    free(parsed[0]);
    free(parsed);
}

// buffer size of "buf" in "fget_input()"
#define BUFFER_SIZE 4096

// returns c-string, must be freed by the caller
// c-string points to NULL if and only if realloc error occurs

char *fget_input(FILE *fd)
{
    unsigned long len = 0, total_len = 0;
    char buf[BUFFER_SIZE];

    char *input_str = NULL;

    do {
        fgets(buf, BUFFER_SIZE, fd);

        len = strlen(buf);

        input_str = realloc(input_str, total_len + len);
        exit_if_memory_error(input_str);

        if (input_str == NULL) return input_str;

        strncpy(input_str + total_len, buf, len);

        total_len += len;

    } while (len == BUFFER_SIZE - 1 && buf[len - 1] != '\n');

    input_str[total_len - 1] = '\0';

    if (input_str[total_len - 2] == '\r') {
        input_str[total_len - 2] = '\0';
    }

    return input_str;
}

// wrapper for fget_input(), to get input from stdin
char *get_input()
{
    return fget_input(stdin);
}
