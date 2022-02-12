#ifndef CSHELLLIB_H
#define CSHELLLIB_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512
#define MAX_N_COMMAND 10
#define MAX_N_ARGUMENTS 50

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

int run(char *PATH, char **args);

void variable_assigning(char *name, char *value);

int exiting();

void logging();

void printing(size_t argc, char **argv);

void theming();

void adding_log(command *list, size_t *size, char *name, struct tm time, char *return_value);

int command_parsing(char *buffer, size_t *argc, char **argv);

#endif /* CSHELLLIB_H */