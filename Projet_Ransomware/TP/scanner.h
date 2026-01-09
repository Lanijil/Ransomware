#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_PATH 4096
#define MAX_FILES 10000

int scan_directory(const char *path, char files[][MAX_PATH], int max_files);
int scan_recursive(const char *path, char files[][MAX_PATH], int max_files, int current_count);
int should_exclude(const char *filename);

#endif
