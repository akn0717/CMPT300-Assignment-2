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

int printing(EnvVar ** var_list, size_t varl_size, size_t argc, char ** argv) {
    for (int i = 1; i < (int) argc; i++) {
        if(argv[i][0] == '$')
        {
            EnvVar * var = find_variable(var_list, varl_size, argv[i]);
            if(var != NULL)
            {
                printf("%s ", var->value);
            }
            else return 1;
        }
        else
        {
            printf("%s ", argv[(i)]);
        }
    }
    printf("\n");
    return 0;
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

int run(char *PATH, char **args) {
    int fds[2];
    int error = pipe(fds);
    if (error)
    {
        return error;
    }
    pid_t pid = fork();
    if (pid==0)
    {
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        dup2(fds[1], STDERR_FILENO);
        execvp(PATH , args);
        fprintf(stderr, "Missing keyword or command, or permission problem\n");
        exit(EXIT_FAILURE);
    }
    else if (pid>0)
    {
        char buffer[2] = "";
        close(fds[1]);
        while (read(fds[0], buffer, 1) == 1)
        {
            printf("%s", buffer);
        }
        wait(&error);
        close(fds[0]);
    }
    return error;
}


EnvVar *find_variable(EnvVar **variable_list, size_t varl_size, char *name) 
{
    for(int i=0; i<varl_size; i++)
    {
        if(!strcmp(name, variable_list[i]->name))
        {
            return variable_list[i];
        }
    }
    return NULL;
}

void variable_assigning(EnvVar **variable_list, size_t * varl_size, char *name, char *value)
{
    EnvVar *p = find_variable(variable_list, *varl_size, name);
    if(p == NULL)
    {
        EnvVar *my_struct;
        my_struct = (EnvVar *) malloc(sizeof(EnvVar));
        my_struct->name = (char *) malloc(sizeof(char)*MAX_STRING_LENGTH);
        my_struct->value = (char *) malloc(sizeof(char)*MAX_STRING_LENGTH);
        strcpy(my_struct->name, name);
        strcpy(my_struct->value, value);
        variable_list[*varl_size]=my_struct;
        ++(*varl_size);
    }
    else
    {
        strcpy(p->value, value);
    }

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
            argv[*argc] = strdup(arg);
            for (i = 0;i<strlen(arg) && arg[i]!='=';++i)
            {
                argv[(*argc)][i] = arg[i];
            }
            argv[(*argc)][i] = '\0';
            ++(*argc);
            if (arg[i]=='=')
            {
                argv[*argc] = strdup(arg);
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
        argv[*argc] = strdup(arg);
        arg = strtok(NULL, " ");
        ++(*argc);
    }
    int n = strlen(argv[(*argc)-1]);
    if (argv[(*argc)-1][n-1]=='\n') argv[(*argc)-1][n-1] = '\0';
    argv[(*argc)] = NULL;
    return 0;
}


void red () {
  printf("\033[0;31m");
}