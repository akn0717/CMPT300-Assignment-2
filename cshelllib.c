#include "cshelllib.h"

int print_uppercase(char *str)
{
    int n = strlen(str);
    if (!syscall(SYS_UPCASE, str, n))   //call the system call 440
    {
        printf("%s\n", str);
        return 0;
    }
    return 1;
}

void free_memory(command **comm_list, size_t comm_list_size, EnvVar** variable_list, size_t varl_size, char **command_argv, size_t n_commands, char* buffer)
{

    //free the command argument list
    for (int i=0;i<n_commands;++i)
    {
        free(command_argv[i]);
        command_argv[i] = NULL;
    }
    free(command_argv);
    command_argv = NULL;

    //free the log list
    for (int i=0;i<comm_list_size;++i)
    {
        free(comm_list[i]->name);
        comm_list[i]->name = NULL;
        free(comm_list[i]);
        comm_list[i] = NULL;
    }
    free(comm_list);
    comm_list = NULL;

    //free the enviroment variable list
    for (int i=0;i<varl_size;++i)
    {
        free(variable_list[i]->name);
        variable_list[i]->name = NULL;
        free(variable_list[i]->value);
        variable_list[i]->value = NULL;
        free(variable_list[i]);
        variable_list[i] = NULL;
    }
    free(variable_list);
    variable_list = NULL;

    //free the reading input buffer
    free(buffer);
    buffer = NULL;
}

int logging(command **comm_list, size_t comm_list_size) {
    //go through the log list and print
    for (int i = 0; i < comm_list_size; i++)
    {
        printf("%s %s %d\n", asctime(&(comm_list[i]->time)), comm_list[i]->name, comm_list[i]->return_value);
    }
    return 0;
}

int printing(EnvVar ** var_list, size_t varl_size, size_t argc, char ** argv) {
    for (int i = 1; i < (int) argc; i++) {

        //case if the argument is a variable
        if(argv[i][0] == '$')
        {
            //find the pointer point to the variable
            EnvVar * var = find_variable(var_list, varl_size, argv[i]);

            //if exists
            if(var != NULL)
            {
                printf("%s ", var->value);
            }
            else return 1;//if not exists
        }
        else //print directly if not a variable
        {
            printf("%s ", argv[(i)]);
        }
    }
    printf("\n");
    return 0;
}

int theming(char * colour)
{
    if (colour == NULL) colour = "";
    if (!strcmp(colour,"red"))  //red theme
    {
        printf("\033[0;31m");
    }
    else if (!strcmp(colour,"green"))   //green theme
    {
        printf("\033[0;32m");
    }
    else if (!strcmp(colour,"blue"))    //blue theme
    {
        printf("\033[0;34m");
    }
    else if (!strcmp(colour,"white"))   //while theme
    {
        printf("\033[0m");
    }
    else    //unsupported theme
    {
        printf("%s: %s \n", "Unsupported theme", colour);
        return 1;
    }
    return 0;
}


EnvVar *find_variable(EnvVar **variable_list, size_t varl_size, char *name) 
{
    for(int i=0; i<varl_size; i++)  //go through the variable list and check existing
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
    //find the environment variable memory address
    EnvVar *p = find_variable(variable_list, *varl_size, name);
    if(p == NULL) //if it does not exists, create new variable and append to the variable list
    {
        p = (EnvVar *) malloc(sizeof(EnvVar));

        p->name = (char *) malloc(sizeof(char)*MAX_STRING_LENGTH);
        strcpy(p->name, name);

        p->value = (char *) malloc(sizeof(char)*MAX_STRING_LENGTH);
        strcpy(p->value, value);

        variable_list[*varl_size] = p;

        ++(*varl_size);
    }
    else //if it already exists, just update the value
    {
        strcpy(p->value, value);
    }
    return 0;
}

void adding_log(command **comm_list, size_t *comm_list_size, char *name, struct tm time, int return_value)
{
    //create a new memory for the command log
    comm_list[*comm_list_size] = (command*) malloc(sizeof(command));

    if (name!=NULL)
    {
        ////keep log of the name
        comm_list[*comm_list_size]->name = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));
        strcpy(comm_list[*comm_list_size]->name, name);
    }

    //keep log of the execution time
    comm_list[*comm_list_size]->time = time;

    //keep log of the return value
    comm_list[*comm_list_size]->return_value = return_value;
    ++(*comm_list_size);
};


int command_parsing(char *buffer, size_t *argc, char **argv)
{
    //free all the argument variable if there is
    for (int i=0;i<(*argc);++i)
    {
        if (argv[i]!=NULL) free(argv[i]);
        argv[i] = NULL;
    }
    (*argc) = 0;

    //remove the end line character if there is
    int n = strlen(buffer);
    if (buffer[n-1]=='\n') buffer[n-1] = '\0';

    //variable to check if there are double quotations
    int flag = 0;
    
    //replace all the space in the double quotations with '\n' characters
    for (int i=0;i<n;++i)
    {
        if (buffer[i]=='\"' || buffer[i]=='\'')
        {
            flag = !flag;
            continue;
        }
        if (flag==1 && buffer[i]==' ') buffer[i]='\n';
    }
    
    //if the there exists opening quotation but no ending quotation, return error
    if (flag == 1) return 1;

    //split command into multiple argument by space character
    char *token = strtok(buffer, " ");
    while (token!=NULL)
    {
        n = strlen(token);
        argv[(*argc)] = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));
        strcpy(argv[(*argc)],token);
        token = strtok(NULL," ");
        ++(*argc);
        if (*argc == 1) //case if the command is variable assignment command
        {
            if (argv[0][0] == '$')
            {
                if (token!=NULL) return 1;  //if there is no space in the variable assignment command
                else
                {
                    char temp[MAX_STRING_LENGTH];
                    strcpy(temp, argv[0]);  //making a back up of variable assignment command

                    token = strtok(temp,"=");   //split by =
                    if (token==NULL) return 1;  //check if the variable name correct syntax
                    strcpy(argv[0], token);

                    token = strtok(NULL,"=");   //find the value portion
                    if (token==NULL) return 1;  //check if the variable value correct syntax

                    argv[(*argc)] = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));   //create new memory space for value
                    strcpy(argv[(*argc)],token);

                    //increment the number of arguments
                    ++(*argc);
                    break;
                }
            }
        }
    }
    
    //change all the '\n' in the double quotation by the space back
    for (int i=0;i<(*argc);++i)
    {
        n = strlen(argv[i]);
        for (int j=0;j<n;++j)
        {
            if (argv[i][j] == '\n') argv[i][j] = ' ';
        }
    }

    //mark there is no argument after
    argv[(*argc)] = NULL;
    return 0;
}
