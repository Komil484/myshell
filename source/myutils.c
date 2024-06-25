#include "myutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// helper function declarations
void realloc_if_full(char **args, unsigned long len, unsigned long *cap);
void replace_if_escape(char **write, char **read);


// returns null terminated array of c-strings
char** parse(char *str)
{
    unsigned long len = 2, cap = 16;
    char **args = calloc(16, sizeof(char*));
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

                realloc_if_full(args, len, &cap);
                *write++ = '\0';
                args[len - 1] = write; // write -> resulting str
                ++len;
                break;

            case '\"':
                ++read;
                c = *read;
                while(c != '\"' && c != '\0') {
                    replace_if_escape(&write, &read);
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

            default:
                *write++ = *read;
                ++read;
                break;
        }
    }

    *write = '\0';

    return args;
}

// replace common escape char sequences such as "\n" with respective char 0x0A (10)
// modifies *write and *read
void replace_if_escape(char **write, char **read)
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


void realloc_if_full(char **args, unsigned long len, unsigned long *cap)
{
    unsigned long local_cap = *cap;
    if (len < local_cap) return;

    args = realloc(args, sizeof(char*) * local_cap * 2);
    local_cap *= 2;
    for (unsigned long i = len; i < local_cap; ++i) {
        args[i] = NULL;
    }

    *cap = local_cap;
}


// returns str and len, str must be freed by the caller
// str points to NULL if and only if realloc error occurs

#define BUFFER_SIZE 4096

char* fget_input(FILE *fd)
{
    unsigned long len = 0, total_len = 0;
    char buf[BUFFER_SIZE];

    char *input_str = NULL;

    do {
        fgets(buf, BUFFER_SIZE, fd);

        len = strlen(buf);

        input_str = realloc(input_str, total_len + len);

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
char* get_input()
{
    return fget_input(stdin);
}
