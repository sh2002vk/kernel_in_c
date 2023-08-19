#ifndef file_hander_H
#define file_handler_H
#include <stdio.h>

int list_directory(char* path);
int make_file(char* name);
int make_dir(char* name);
int enter(char* path, char* directory);
void get_current_path(char* path, size_t path_size);

#endif