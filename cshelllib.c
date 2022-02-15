#include "cshelllib.h"

int exiting(command **comm_list, size_t comm_list_size, char **command_argv, size_t n_commands, char* buffer) {
    for (int i=0;i<comm_list_size;++i)
    {
        free(command_argv[i]);
        command_argv[i] = NULL;
    }
    free(command_argv);
    command_argv = NULL;
    for (int i=0;i<n_commands;++i)
    {
        free(comm_list[i]->name);
        comm_list[i]->name = NULL;
        free(comm_list[i]);
        comm_list[i] = NULL;
    }
    free(comm_list);
    free(buffer);
    printf("Bye!\n");
    return 0;
}
int logging(command **comm_list, size_t comm_list_size) {
    for (int i = 0; i < comm_list_size; i++)
    {
        printf("%s %s %d\n", asctime(&(comm_list[i]->time)), comm_list[i]->name, comm_list[i]->return_value);
    }
    return 0;
}

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

int theming(char * colour)
{
    if (!strcmp(colour,"red"))
    {
        printf("\033[0;31m");
    }
    else if (!strcmp(colour,"green"))
    {
        printf("\033[0;32m");
    }
    else if (!strcmp(colour,"blue"))
    {
        printf("\033[0;34m");
    }
    else if (!strcmp(colour,"white"))
    {
        printf("\033[0m");
    }
    else
    {
        printf("%s: %s \n", "Unsupported theme", colour);
        return 1;
    }
    return 0;
}

void reset_colour()
{
    printf("\033[0m");
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

int variable_assigning(EnvVar **variable_list, size_t * varl_size, char *name, char *value)
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
    return 0;
}

void adding_log(command **comm_list, size_t *comm_list_size, char *name, struct tm time, int return_value)
{
    comm_list[*comm_list_size] = (command*) malloc(sizeof(command));
    if (name!=NULL)
    {
        comm_list[*comm_list_size]->name = strdup(name);
    }

    comm_list[*comm_list_size]->time = time;

    comm_list[*comm_list_size]->return_value = return_value;
    ++(*comm_list_size);
};


int command_parsing(char *buffer, size_t *argc, char **argv)
{

    for (int i=0;i<(*argc);++i)
    {
        if (argv[i]!=NULL) free(argv[i]);
    }
    (*argc) = 0;

    int n = strlen(buffer);
    if (buffer[n-1]=='\n') buffer[n-1] = '\0';

    char *token = strtok(buffer, " =");
    int flag = 0;
    while (token!=NULL)
    {
        n = strlen(token);
        if (argv[(*argc)]==NULL)
        {
            argv[(*argc)] = strdup(token);
        } else
        {
            strcat(argv[(*argc)],token);
        }
        for (int i=0;i<n;++i)
        {
            if (token[i]=='"') 
            {
                flag = !flag;
            }
        }
        token = strtok(NULL," =");
        if (flag == 0) ++(*argc);
    }
    if (flag == 1) return 1;
    argv[(*argc)] = NULL;
    return 0;
}
