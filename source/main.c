#include "myutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void print_debug_info(unsigned long arg_c, char **args)
{
    printf("arg count: %ld\n", arg_c);
    for (unsigned long i = 0; args[i] != NULL; ++i) { 
        printf("[%s] ", args[i]);
    }
    printf("\n\n");
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
        char *input = get_input();
        char **args = parse(input);
        free(input);

        unsigned long arg_c = get_arg_count(args);
        
        switch (get_command(args[0])) {
            case MYSH_CHDR:
                char *path = (arg_c > 1) ? args[1] : getenv("HOME");
                if (path == NULL) perror("mysh");
                if (chdir(path)) {
                    perror("mysh");
                }
                break;
            case MYSH_EVAR:
                handle_env_var_operations(arg_c, args);
                break;
            case MYSH_EXEC:
                run_process(args);
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
