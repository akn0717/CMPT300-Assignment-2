#include "cshelllib.h"

int main(int argc, char* argv[])
{
    size_t buffer_size = 512;
    char *buffer = (char*) malloc(buffer_size * sizeof(char));
    size_t N_commands = 512;
    command *list_command = (command*) malloc(N_commands * sizeof(command));

    int N_command_args = 10;
    char **command_argv = (char **) malloc(N_command_args * sizeof(char*));
    for (int i=0;i<10;++i)
    {
        command_argv[i] = (char *) malloc(buffer_size * sizeof(char));
    }

    size_t command_argc;
    size_t n_commands = 0;

    char *return_value = NULL;

    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);

    while (1)
    {
        printf("cshell$ ");
        
        buffer[0] = '\0';
        getline(&buffer, &buffer_size, stdin);
        
        int parsing_error = command_parsing(buffer, &command_argc, command_argv);
        printf("%s %s", command_argv[0],command_argv[1]);
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
            //variable_assigning();
        }
        else if (!strcmp(command_argv[0], "log")) 
        {
            logging();
        }
        else if (!strcmp(command_argv[0], "print"))
        {
        }
        else if (!strcmp(command_argv[0], "theme"))
        {
        }
        else 
        {
            printf("Missing keyword or command, or permission problem\n");
            continue;
        }
        time_info = localtime(&raw_time);
        adding_log(list_command, &n_commands, command_argv[0], *time_info, return_value);
    }
    return 0;
}