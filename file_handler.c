#include "file_handler.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

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

int make_file(char* name) {
    FILE *fptr;
    fptr = fopen(name, "w");
    fclose(fptr);
    return 1;
}

int make_dir(char* name) {
    if (!name){
        return -1;
    }
    int result = mkdir(name, 0777);
    return result;
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

void get_current_path(char* path, size_t path_size){
    getcwd(path, path_size);
}