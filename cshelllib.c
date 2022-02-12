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

void printing(size_t argc, char** argv) {
    for (int i = 1; i < (int) argc; i++) {
        printf("%s ", argv[(i)]);
    }
    printf("\n");
}

void theming(char * colour)
{
    if (!strcmp(colour,"red"))
    {
        printf("\033[0;31m");
        printf("%s \n", "theme changed to red");
    }
    else if (!strcmp(colour,"green"))
    {
        printf("\033[0;32m");
        printf("%s \n", "theme changed to green");
    }
    else if (!strcmp(colour,"blue"))
    {
        printf("\033[0;34m");
        printf("%s \n", "theme changed to blue");
    }
    else
    {
        printf("%s: %s \n", "Unsupported theme", colour);
    }
}

void run() {
    pid_t pid = fork();
    if (pid==0)
    {

    }
}

char *find_variable_value(EnvVar *variable_list, char *name, int counter) 
{
    for(int i=0; i<counter; i++)
    {
        if(!strcmp(name, variable_list[i].name))
        {
            return variable_list[i].value;
        }
    }
    return NULL;
}

EnvVar *find_variable(EnvVar *variable_list, size_t varl_size, char *name) 
{

}

void variable_assigning(EnvVar **variable_list, size_t varl_size, char *name, char *value)
{
    //EnvVar *p = find_variable(variable_list, varl_size, name);
    //if(p == NULL)
    //{
        ++varl_size;
        EnvVar *my_struct;
        my_struct = (EnvVar *) malloc(sizeof(EnvVar));
        my_struct->name = (char *) malloc(sizeof(char)*MAX_STRING_LENGTH);
        my_struct->value = (char *) malloc(sizeof(char)*MAX_STRING_LENGTH);
        strcpy(my_struct->name, name);
        strcpy(my_struct->value, value);
        variable_list[varl_size]=my_struct;
    /* }
    else
    {
        p->value = value;
    } */

}

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


void red () {
  printf("\033[0;31m");
}