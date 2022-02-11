#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cshelllib.h"

int exiting() {
    printf("Bye!\n");
    return 0;
}
void logging(command *list) {}

void printing(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        

    }
    printf("printing test");
}

void theming() {}

void adding_log(command *list, size_t *size, char *name, struct tm time, char *return_value)
{
    if (name!=NULL)
    {
        list[*size].name = (char*) malloc(sizeof(name) * sizeof(char));
        strcpy(list[*size].name, name);
    }

    list[*size].time = time;

    if (return_value != NULL)
    {
        list[*size].return_value = (char*) malloc(sizeof(return_value) * sizeof(char));
        strcpy(list[*size].return_value, return_value);
    }

    ++(*size);
};

void command_parsing(char *buffer, size_t *argc, char **argv)
{
    char *arg = strtok(buffer, " ");
    (*argc) = 0;
    while (arg != NULL)
    {
        strcpy(argv[(*argc)], arg);
        arg = strtok(NULL, " ");
        ++(*argc);
    }
    int n = strlen(argv[(*argc)-1]);
    for (int i=0;i<n;++i)
    {
        if (argv[(*argc)-1][i]=='\n') argv[(*argc)-1][i] = 0;
    }
}
