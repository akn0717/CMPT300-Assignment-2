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

    char *buffer = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));
    size_t buffer_size = MAX_STRING_LENGTH;

    char **command_argv = (char **) malloc(MAX_N_ARGUMENTS * sizeof(char*));
    size_t command_argc = 0;
    
    command **logcom_list = (command**) malloc(MAX_N_COMMAND * sizeof(command*));
    size_t logcom_list_size = 0;

    EnvVar **variable_list = (EnvVar **) malloc(sizeof(EnvVar *)* MAX_N_VARIABLE);
    size_t varl_size = 0;


    int return_value = 0;

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
                exiting(logcom_list, logcom_list_size, command_argv, command_argc, buffer);
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
            return_value = exiting(logcom_list, logcom_list_size, command_argv, command_argc, buffer);
            return 0;
        }
        else if (command_argv[0][0] == '$')
        {
            return_value = variable_assigning(variable_list, &varl_size, command_argv[0], command_argv[1]);
            strcat(command_argv[0], "=");
            strcat(command_argv[0], command_argv[1]);
        }
        else if (!strcmp(command_argv[0], "log")) 
        {
            return_value = logging(logcom_list, logcom_list_size);
        }
        else if (!strcmp(command_argv[0], "print"))
        {
            return_value = printing(variable_list, varl_size, command_argc, command_argv);
        }
        else if (!strcmp(command_argv[0], "theme"))
        {
            return_value = theming(command_argv[1]);
        }
        else run(command_argv[0], command_argv);

        time_info = localtime(&raw_time);
        adding_log(logcom_list, &logcom_list_size, command_argv[0], *time_info, return_value);

    }
    return 0;
}