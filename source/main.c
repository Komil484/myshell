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

    for (unsigned long i = 0; parsed[i] != NULL; ++i) { 
        printf("[%s] ", parsed[i]);
    }
    printf("\n");
    

    return 0;
}   
