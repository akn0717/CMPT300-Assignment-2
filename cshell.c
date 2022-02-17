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
            fprintf (stderr, "Unable to read the script file %s \n" , argv[1]) ;
            return 1;
        }
        flag = 1;
    }

    char *buffer = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));
    size_t buffer_size = MAX_STRING_LENGTH;

    char **command_argv = (char **) malloc(MAX_N_ARGUMENTS * sizeof(char*));
    size_t command_argc = 0;
    for (int i=0;i<MAX_N_ARGUMENTS;++i) command_argv[i] = NULL;
    
    command **comm_list = (command**) malloc(MAX_N_COMMAND * sizeof(command*));
    size_t comm_list_size = 0;

    EnvVar **variable_list = (EnvVar **) malloc(sizeof(EnvVar *)* MAX_N_VARIABLE);
    size_t varl_size = 0;


    int return_value = 0;

    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);

    char colour_name[15] = "white";

    while (1)
    {
        for (int i=0;i<command_argc;++i)
        {
            if (command_argv[i]!=NULL) free(command_argv[i]);
            command_argv[i] = NULL;
        }
        buffer[0] = '\0';
        if (flag==0)
        {
            printf("cshell$ ");
            theming("white");
            getline(&buffer, &buffer_size, stdin);
            theming(colour_name);
        }
        else
        {
            if (fgets (buffer, buffer_size, fptr ) == NULL)
            {
                fclose ( fptr );
                exiting(comm_list, comm_list_size, variable_list, varl_size, command_argv, command_argc, buffer);
                return 0;
            }
        }
        int parsing_error = command_parsing(buffer, &command_argc, command_argv);
        time(&raw_time);
        if (parsing_error)
        {
            fprintf(stderr, "Incorrect syntax!\n");
            continue;
        }
        else if (!strcmp(command_argv[0],"exit"))
        {
            exiting(comm_list, comm_list_size, variable_list, varl_size, command_argv, command_argc, buffer);
            return 0;
        }
        else if (command_argv[0][0] == '$')
        {
            return_value = variable_assigning(variable_list, &varl_size, command_argv[0], command_argv[1]);
            strcat(command_argv[0], "=");
            strcat(command_argv[0], command_argv[1]);
            command_argv[1] = NULL;
        }
        else if (!strcmp(command_argv[0], "log"))
        {
            return_value = logging(comm_list, comm_list_size);
        }
        else if (!strcmp(command_argv[0], "print"))
        {
            return_value = printing(variable_list, varl_size, command_argc, command_argv);
            if (return_value==1)
            {
                printf("Non existing variable name!\n");
            }
        }
        else if (!strcmp(command_argv[0], "theme"))
        {
            return_value = theming(command_argv[1]);
            if(return_value == 0)
            {
                strcpy(colour_name, command_argv[1]);
            }
        }
        else if (!strcmp(command_argv[0], "uppercase"))
        {
            return_value = print_uppercase(command_argv[1]);
        }
        else run(command_argv[0], command_argv);

        time_info = localtime(&raw_time);
        adding_log(comm_list, &comm_list_size, command_argv[0], *time_info, return_value);
    }
    return 0;
}