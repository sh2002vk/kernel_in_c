#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

void get_current_path(char* path, size_t path_size){
    getcwd(path, path_size);
}

int enter(char* path, char* directory) {
    if (!directory) {
        printf("Error: Directory name is NULL.\n");
        return 1;
    }

    size_t path_len = strlen(path);
    if (path_len == 0 || path[path_len - 1] != '/') {
        strcat(path, "/");
    }
    strcat(path, directory);

    if (chdir(path) != 0) {
        perror("Failed to change directory");
        return 1;
    }
    return 0;
}

int make_dir(char* name) {
    if (!name){
        return -1;
    }
    int result = mkdir(name, 0777);
    return result;
}

int make_file(char* name) {
    FILE *fptr;
    fptr = fopen(name, "w");
    fclose(fptr);
    return 1;
}

int list_directory(char* path){
    DIR *d;             // a directory object
    struct dirent *dir; // used to navigate directories

    d = opendir(path);
    printf("STARTING DIRECTORY\n\n---");
    if (d) {
        printf("---\n");
        while ((dir = readdir(d)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                printf("%s\n", dir->d_name);
            }
        }
        printf("---\n");
        closedir(d);
    }
    printf("\n\nENDING DIRECTORY\n");
    return 0;
}


void handle_system_commands(char* commands) {
    // Parse commands for usage by internal functions
//    char* words[256];
//    int wordCount = 0;
//
//    char* word = strtok(commands, " ");
//    while(word != NULL) {
//        words[wordCount++] = word;
//        word = strtok(NULL, " ");
//    }
//
//    if (wordCount == 0) {
//        printf("No operation provided\n");
//        return;
//    }

    // TO BE IMPLEMENTED
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
    int wordCount = 0;

    char* word = strtok(commands, " ");
    while(word != NULL) {
        words[wordCount++] = word;
        word = strtok(NULL, " ");
    }

    if (wordCount == 0) {
        printf("No operation provided\n");
        return;
    }

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

