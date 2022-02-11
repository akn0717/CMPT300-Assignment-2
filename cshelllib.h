#ifndef CSHELLLIB_H
#define CSHELLLIB_H
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct command
{
    char *name;
    struct tm time;
    char *return_value;
} command;

typedef struct {
    char *name;
    char *value;
} EnvVar;

void run();

void variable_assigning(char *name, char *value);

int exiting();

void logging();

void printing(size_t argc, char **argv);

void theming();

void adding_log(command *list, size_t *size, char *name, struct tm time, char *return_value);

int command_parsing(char *buffer, size_t *argc, char **argv);

#endif /* CSHELLLIB_H */