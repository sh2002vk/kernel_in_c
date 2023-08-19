#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include "file_handler.h"   // includes functions to handler functions
#include "parser.h"

#define CORES 8
#define MAX_JOBS 8
#define MAX_JOBS_QUEUED 256

void handle_system_commands(char* commands) {
    // Parse commands for usage by internal functions
    char* words[256];
    parse_commands(words, commands);
}

void handle_file_commands(char* commands){
    char path[PATH_MAX];
    get_current_path(path, sizeof(path));
    if (path[0] == '\0') {
        printf("unable to find current path\n");
        return;
    }

    // Parse commands for usage by internal functions
    char* words[256];
    parse_commands(words, commands);

    // Handle the operations
    if (!strcmp(words[0], "list")) {
        list_directory(path);
    } else if (!strcmp(words[0], "print")) {
        printf("current path: %s\n", path);
    } else if (!strcmp(words[0], "enter")) {
        if (enter(path, words[1]) != 0) {  // words [1] is the directory we're going into
            printf("failed to enter directory %s\n", words[1]);
        } else {
            get_current_path(path, sizeof(path));   // update the current path
            printf("changed to directory: %s\n",path);
        }
    } else if (!strcmp(words[0], "makedir")) {
        if (make_dir(words[1]) != -1) {
            printf("made directory: %s\n", words[1]);
        } else {
            printf("Failed to make directory\n");
        }
    } else if (!strcmp(words[0], "makefile")) {
        if (make_file(words[1]) != -1) {
            printf("made file: %s\n", words[1]);
        } else {
            printf("Failed to make file\n");
        }
    } else {
        printf("command not recognized\n");
    }
}


void process_command(char* type, char* commands) {
    printf("operation: %s\n", type);
    if (strcmp(type, "file") == 0) {
        handle_file_commands(commands);
    } else if (strcmp(type, "system") == 0) {
        handle_system_commands(commands);
    } else {
        printf("INVALID COMMAND CALL\n");
    }
}

void shell_loop(){
    char* input = malloc(256 * sizeof(char));
    size_t len = 0;                        // size_t is always positive, used to represent size of objects

    while (1) {
        printf("\n\nPOSSIBLE COMMANDS:\nfile {list, print, enter, makefile, makedir},\nsystem {}\nEnter \"END\" to exit program\n");
        getline(&input, &len, stdin);      // use this instead of scanf to read whole line at once
        input[strcspn(input, "\n")] = 0;   // get rid of newline

        if (strcmp(input, "END") == 0) {
            printf("ENDING PROGRAM\n");
            break;
        } else {
            char* type = strtok(input, " ");
            char* commands = strtok(NULL, "\n");
            process_command(type, commands);
        }
    }

    free(input);
}

int main(int argc, char* argv[]){
    // TODO: LOAD FILES

    // RUN EXECUTION LOOP
    shell_loop();

    // TODO: SHUTDOWN COMMANDS

    return 0;
}

