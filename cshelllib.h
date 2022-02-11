#ifndef CSHELLLIB_H
#define CSHELLLIB_H

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

int exiting();

void logging();

void printing(size_t argc, char **argv);

void theming();

void adding_log(command *list, size_t *size, char *name, struct tm time, char *return_value);

void command_parsing(char *buffer, size_t *argc, char **argv);

#endif /* CSHELLLIB_H */