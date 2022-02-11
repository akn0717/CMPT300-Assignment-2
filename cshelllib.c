#include "cshelllib.h"

int exiting(command *list_command, size_t N_command_args, char **command_argv, size_t n_commands) {
    for (int i=0;i<N_command_args;++i)
    {
        free(command_argv[i]);
    }
    free(command_argv);
    for (int i=0;i<n_commands;++i)
    {
        free(list_command[i].name);
        free(list_command[i].return_value);
    }
    free(list_command);
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

void run() {
    pid_t pid = fork();
    if (pid==0)
    {

    }
}

void variable_assigning(char *name, char *value) {}

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


int command_parsing(char *buffer, size_t *argc, char **argv)
{
    char *arg = strtok(buffer, " ");
    (*argc) = 0;
    while (arg != NULL)
    {
        if (arg[0]=='$' && (*argc)==0)
        {
            int i;
            for (i = 0;i<strlen(arg) && arg[i]!='=';++i)
            {
                argv[(*argc)][i] = arg[i];
            }
            argv[(*argc)][i] = '\0';
            ++(*argc);
            if (arg[i]=='=')
            {
                ++i;
                int j = 0;
                while (arg[i] != '\0')
                {
                    argv[(*argc)][j] = arg[i];
                    ++i;
                    ++j;
                }
                argv[(*argc)][j] = '\0';
                ++(*argc);
                break;
            }
        }
        strcpy(argv[(*argc)], arg);
        arg = strtok(NULL, " ");
        ++(*argc);
    }
    int n = strlen(argv[(*argc)-1]);
    if (argv[(*argc)][n-2]=='\n') argv[n-2] = '\0';
    return 0;
}
