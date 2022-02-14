#include "cshelllib.h"

int main(int argc, char* argv[])
{
    FILE *fptr;
    int flag = 0;

    if (argc > 1)
    {
        fptr = fopen(argv[1],"r");
        if ( fptr == NULL )
        {
            printf ( "Unable to read the script file %s \n" , argv[1]) ;
            return 1;
        }
        flag = 1;
    }

    size_t buffer_size = MAX_N_COMMAND;
    char *buffer = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));
    command *comm_list = (command*) malloc(MAX_N_COMMAND * sizeof(command));

    char **command_argv = (char **) malloc(MAX_N_ARGUMENTS * sizeof(char*));

    EnvVar **variable_list = (EnvVar **) malloc(sizeof(EnvVar *)* MAX_N_VARIABLE);
    size_t varl_size = 0;

    size_t command_argc;
    size_t comm_list_size = 0;

    char *return_value = NULL;

    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);

    while (1)
    {
        buffer[0] = '\0';
        if (flag==0)
        {
            printf("cshell$ ");
            getline(&buffer, &buffer_size, stdin);
        }
        else
        {
            if (fgets (buffer, buffer_size, fptr ) == NULL)
            {
                exiting();
                fclose ( fptr );
                return 0;
            }
        }
        int parsing_error = command_parsing(buffer, &command_argc, command_argv);
        //printf("%s %s", command_argv[0],command_argv[1]);
        time(&raw_time);
        if (parsing_error)
        {
            printf("ERROR: Incorrect format for variable name!");
            continue;
        }
        if (!strcmp(command_argv[0],"exit"))
        {
            exiting();
            return 0;
        }
        else if (command_argv[0][0] == '$')
        {
            variable_assigning(variable_list, &varl_size, command_argv[0], command_argv[1]);
            strcat(command_argv[0], "=");
            strcat(command_argv[0], command_argv[1]);
        }
        else if (!strcmp(command_argv[0], "log")) 
        {
            logging(comm_list, comm_list_size);
        }
        else if (!strcmp(command_argv[0], "print"))
        {
            time(&raw_time);
            printing(variable_list, varl_size, command_argc, command_argv);
        }
        else if (!strcmp(command_argv[0], "theme"))
        {
            theming(command_argv[1]);
            time(&raw_time);
        }
        else run(command_argv[0], command_argv);

        time_info = localtime(&raw_time);
        adding_log(comm_list, &comm_list_size, command_argv[0], *time_info, return_value);

    }
    return 0;
}