#include "myutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char *input = get_input();
    char **parsed = parse(input);
    free(input);
       
    unsigned long arg_c = get_arg_count(parsed);

    printf("arg count: %ld\n", arg_c);
    for (unsigned long i = 0; parsed[i] != NULL; ++i) { 
        printf("[%s] ", parsed[i]);
    }
    printf("\n");
    
    free_parsed(parsed);

    return 0;
}   
