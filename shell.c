#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include "file_handler.h"   // includes functions to handler functions
#include "parser.h"
#include "pcb.h"
#include "scheduler.h"

#define CORES 8
#define MAX_JOBS_QUEUED 256

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

void handle_system_commands(char* commands, scheduler* thread_runner) {
    // Parse commands for usage by internal functions
    char* words[256];
    parse_commands(words, commands);

    if (!strcmp(words[0], "status")) {
        print_scheduler_status(thread_runner);
    } else if (!strcmp(words[0], "start")){
        pcb* thread = pcb_init(atoi(words[1]), atoi(words[2]), atoi(words[3]), atoi(words[4]));
        int result = add_job(thread_runner, thread);
        if (result == 2) {
            printf("Thread %d added successfully. Scheduler status is:\n", thread->uid);
            print_scheduler_status(thread_runner);
        }
    } else if (!strcmp(words[0], "kill")) {
        int result = kill_job(thread_runner, atoi(words[1]));
        if (result == 1) {
            printf("Thread %d killed", atoi(words[1]));
        } else {
            printf("Thread %d was not killed", atoi(words[1]));
        }
    } else {
        printf("command not recognized");
    }
}

void process_command(char* type, char* commands, scheduler* thread_runner) {
    printf("operation: %s\n", type);
    if (strcmp(type, "file") == 0) {
        handle_file_commands(commands);
    } else if (strcmp(type, "system") == 0) {
        handle_system_commands(commands, thread_runner);
    } else {
        printf("INVALID COMMAND CALL\n");
    }
}

void shell_loop(scheduler* thread_runner){
    char* input = malloc(256 * sizeof(char));
    size_t len = 0;     // size_t is always positive, used to represent size of objects

    while (1) {
        // update the state of all processes
        update_scheduler(thread_runner);

        printf("\n\nPOSSIBLE COMMANDS:\nfile {list, print, enter, makefile, makedir},\nsystem {status, start, kill}\nEnter \"END\" to exit program\n");
        getline(&input, &len, stdin);      // use this instead of scanf to read whole line at once
        input[strcspn(input, "\n")] = 0;   // get rid of newline

        if (strcmp(input, "END") == 0) {
            printf("ENDING PROGRAM\n");
            break;
        } else {
            char* type = strtok(input, " ");
            char* commands = strtok(NULL, "\n");
            process_command(type, commands, thread_runner);
        }
    }

    free(input);
}

int main(int argc, char* argv[]){
    // LOAD TOOLS
    scheduler* thread_runner = create_scheduler(CORES, MAX_JOBS_QUEUED);

    // RUN EXECUTION LOOP
    shell_loop(thread_runner);

    // DESTROY TOOLS
    destroy_scheduler(thread_runner);

    return 0;
}

