#include "cshelllib.h"

int main(int argc, char* argv[])
{
    size_t buffer_size = MAX_N_COMMAND;
    char *buffer = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));
    command *list_command = (command*) malloc(MAX_N_COMMAND * sizeof(command));

    char **command_argv = (char **) malloc(MAX_N_ARGUMENTS * sizeof(char*));

    EnvVar **variable_list = (EnvVar **) malloc(sizeof(EnvVar *)* MAX_N_VARIABLE);
    size_t varl_size = 0;

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
            variable_assigning(variable_list, command_argc, command_argv[0], command_argv[1]);
        }
        else if (!strcmp(command_argv[0], "log")) 
        {
            logging();
        }
        else if (!strcmp(command_argv[0], "print"))
        {
            time(&raw_time);
            printing(command_argc, command_argv);
        }
        else if (!strcmp(command_argv[0], "theme"))
        {
            theming(command_argv[1]);
            time(&raw_time);
        }
        else run(command_argv[0], command_argv);

        time_info = localtime(&raw_time);
        adding_log(list_command, &n_commands, command_argv[0], *time_info, return_value);

    }
    return 0;
}