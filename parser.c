#include "parser.h"
#include <stdio.h>
#include <string.h>

void parse_commands(char* words[], char* commands) {
    // Parse commands for usage by internal functions
    int wordCount = 0;

    char* word = strtok(commands, " ");
    while(word != NULL && wordCount < 255) {
        words[wordCount++] = strdup(word);
        word = strtok(NULL, " ");
    }

    words[wordCount] = NULL;

    if (wordCount == 0) {
        printf("No operation provided\n");
    }
}