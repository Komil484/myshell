#include "myutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

char empty_str[] = "";

char *safe_getenv(char *path)
{
    char *value = getenv(path);
    if (value) return value;
    return (value = empty_str);
}

void print_debug_info(unsigned long arg_c, char **args)
{
    printf("arg count: %ld\n", arg_c);
    for (unsigned long i = 0; args[i] != NULL; ++i) { 
        printf("[%s] ", args[i]);
    }
    printf("\n\n");
}

void print_help()
{
    printf("TODO: Print proper help message\n");
}

void run_process(char **args)
{
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed");
        exit(1);
    } else if (pid == 0) {
        execvp(args[0], args);
        perror("mysh");
        exit(1);
    } else {
        wait(NULL);
    }
}

void handle_env_var_operations(unsigned long arg_c, char **args) {
    switch (arg_c) {
        case 1:
            fprintf(stderr, "var <variable> [value] [no-overwrite]\n");
            fprintf(stderr, "if [value] is not provided, variable will be removed\n");
            fprintf(stderr, "if [no-overwrite] is not provided, it will overwrite\n");
            fprintf(stderr, "if [no-overwrite] is set to anything, it won't overwrite\n");
            break;
        case 2:
            if (unsetenv(args[1])) perror("mysh");
            break;
        case 3:
            if (setenv(args[1], args[2], 1)) perror("mysh");
            break;
        case 4:
            if (getenv(args[1])) fprintf(stderr, "variable %s already exists\n", args[1]);
            if (setenv(args[1], args[2], 0)) perror("mysh");
            break;
    }
}

int main(void)
{
    while (1) {
        char *user = safe_getenv("USER");
        char *name = safe_getenv("NAME");
        char cwd_buf[PATH_MAX];
        getcwd(cwd_buf, sizeof(cwd_buf));
        exit_if_memory_error(cwd_buf);
    
        char *home = safe_getenv("HOME");
        unsigned long home_len = strlen(home);
        char *cwd = cwd_buf;
        if (strncmp(home, cwd, home_len) == 0) {
            cwd += home_len - 1;
            *cwd = '~';
        }

        printf("%s@%s:%s$ ", user, name, cwd);

        char *input = get_input();
        char **args = parse(input);
        free(input);

        unsigned long arg_c = get_arg_count(args);
        
        switch (get_command(args[0])) {
            case MYSH_NULL:
                break;
            case MYSH_CHDR:
                char *path = (arg_c > 1) ? args[1] : getenv("HOME");
                if (path == NULL || chdir(path)) perror("mysh");
                break;
            case MYSH_EVAR:
                handle_env_var_operations(arg_c, args);
                break;
            case MYSH_EXEC:
                run_process(args);
                break;
            case MYSH_HELP:
                print_help();
                break;
            case MYSH_EXIT:
                exit(0);
                break;
            default:
                printf("mysh: unknown command code\n");
                exit(1);
                break;
        }

        free_parsed(args);
    }

    return 0;
}   
