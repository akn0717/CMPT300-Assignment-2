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
    
    command **comm_list = (command**) malloc(MAX_N_COMMAND * sizeof(command*));
    size_t comm_list_size = 0;

    EnvVar **variable_list = (EnvVar **) malloc(sizeof(EnvVar *)* MAX_N_VARIABLE);
    size_t varl_size = 0;

    for (int i=0;i<MAX_N_ARGUMENTS;++i) command_argv[i] = NULL;
    for (int i=0;i<MAX_N_COMMAND;++i) comm_list[i] = NULL;
    for (int i=0;i<MAX_N_VARIABLE;++i) variable_list[i] = NULL;

    int return_value = 0;

    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);

    char colour_name[15] = "white";

    while (1)
    {
        memset(buffer,0,buffer_size);
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
                break;
            }
        }
        int parsing_error = command_parsing(buffer, &command_argc, command_argv);
        time(&raw_time);
        if (parsing_error)
        {
            fprintf(stderr, "Variable value expected \n");
            continue;
        }
        else if (!strcmp(command_argv[0],"exit"))
        {
            break;
        }
        else if (command_argv[0][0] == '$')
        {
            return_value = variable_assigning(variable_list, &varl_size, command_argv[0], command_argv[1]);
            strcat(command_argv[0], "=");
            strcat(command_argv[0], command_argv[1]);
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
                printf("Non-existing variable name!\n");
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
        else 
        {
            int fds[2];
            int error = pipe(fds);
            if (!error)
            {
                pid_t pid = fork();
                if (pid==0)
                {
                    close(fds[0]);
                    dup2(fds[1], STDOUT_FILENO);
                    dup2(fds[1], STDERR_FILENO);
                    execvp(command_argv[0] , command_argv);
                    fprintf(stderr, "Missing keyword or command, or permission problem\n");
                    close(fds[1]);
                    fclose(fptr);
                    free_memory(comm_list, comm_list_size, variable_list, varl_size, command_argv, command_argc, buffer);
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
                if (error) error = 2;
            } else error = 1;
            return_value = error;
            if (return_value == 1)
            {
                printf("Can't create pipe!\n");
            }
        }

        time_info = localtime(&raw_time);
        adding_log(comm_list, &comm_list_size, command_argv[0], *time_info, return_value);
    }
    for (int i=0;i<command_argc;++i)
    {
        if (command_argv[i]!=NULL) free(command_argv[i]);
        command_argv[i] = NULL;
    }
    command_argc = 0;
    free_memory(comm_list, comm_list_size, variable_list, varl_size, command_argv, command_argc, buffer);
    printf("Bye!\n");
    return 0;
}