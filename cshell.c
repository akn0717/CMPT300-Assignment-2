#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "cshelllib.h"

int main(int argc, char* argv[])
{
    size_t buffer_size = 512;
    char *buffer = (char*) malloc(buffer_size * sizeof(char));
    size_t N_commands = 512;
    command *list_command = (command*) malloc(buffer_size * sizeof(command));

    int N_command_args = 10;
    char **command_argv = (char **) malloc(N_command_args * sizeof(char*));
    for (int i=0;i<10;++i)
    {
        command_argv[i] = (char *) malloc(sizeof(buffer) * sizeof(char));
    }

    size_t command_argc = 1;

    char *return_value = NULL;

    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);

    while (1)
    {
        printf("cshell$ ");
        
        
        buffer[0] = '\0';
        getline(&buffer, &buffer_size, stdin);
        
        command_parsing(buffer, &command_argc, command_argv);
        printf("%s",command_argv[0]);

        if (!strcmp(command_argv[0],"exit"))
        {
            time(&raw_time);
            exiting();
            return 0;
        }
        else if (!strcmp(command_argv[0], "log")) 
        {
            time(&raw_time);
            logging();
        }
        else if (!strcmp(command_argv[0], "print")) 
        {
            time(&raw_time);

        }
        else if (!strcmp(command_argv[0], "theme"))
        {
            time(&raw_time);
        }
        else continue;
        time_info = localtime(&raw_time);
        adding_log(list_command, &N_commands, command_argv[0], *time_info, return_value);
        
    }
    free(list_command);
    return 0;
}