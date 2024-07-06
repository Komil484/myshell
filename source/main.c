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
            case MYSH_SETV:
                printf("doing env stuff\n");
                break;
            case MYSH_EXEC:
                run_process(args);
                break;
            case MYSH_EXIT:
                printf("exiting shell\n");
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
