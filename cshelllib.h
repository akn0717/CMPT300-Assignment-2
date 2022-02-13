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
#define MAX_N_COMMAND 50
#define MAX_N_VARIABLE 50
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

int exiting();

void logging();

int printing(EnvVar ** var_list, size_t varl_size, size_t argc, char ** argv);

void theming();

void adding_log(command *list, size_t *size, char *name, struct tm time, char *return_value);

int command_parsing(char *buffer, size_t *argc, char **argv);

char *variable_accessing(EnvVar *variable_list, char *name, int counter);

EnvVar *find_variable(EnvVar **variable_list, size_t varl_size, char *name);

void variable_assigning(EnvVar **variable_list, size_t * varl_size, char *name, char *value);

#endif /* CSHELLLIB_H */