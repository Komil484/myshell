#include "myutils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char *input = get_input();
    unsigned long input_len = strlen(input);
    char *input_copy = malloc(input_len + 1);
    input_copy = strcpy(input_copy, input);

    char **parsed = parse(input); // modifies input
    
    
    printf("Input: %s\n", input_copy);

    printf("Resulting: ");
    for (unsigned long i = 0; parsed[i] != NULL; ++i) { 
        printf("[%s] ", parsed[i]);
    }
    printf("\n");
    
    
    printf("Memory: ");
    for (unsigned long i = 0; i < input_len + 1; ++i) {
        char c = parsed[0][i];
        if (c) {
            printf("%c", c);
        } else {
            printf("0");
        }
    }
    printf("\n");

    return 0;
}   
