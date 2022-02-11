#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "cshelllib.h"

int main(int argc, char* argv[])
{
    const int n = 100;
    size_t n_command = 0;
    command *list_command = (void*) malloc(n*sizeof(command));

    timer_t timer;

    while (1)
    {
        printf("cshell$ ");
        char *c = (void*) malloc(512 * sizeof(char));
        char *return_value = NULL;
        c[0] = '\0';
        scanf("%s",c);

        if (!strcmp(c,"exit"))
        {
            exiting();
            return 0;
        }
        else if (!strcmp(c, "log")) 
        {
            time(&timer);
        }
        else if (!strcmp(c, "print")) 
        {
            time(&timer);
        }
        else if (!strcmp(c, "theme"))
        {
            time(&timer);
        }
        else continue;
        adding_log(list_command, &n_command, c, NULL, return_value);
        
    }
    free(list_command);
    return 0;
    // added a comment
}