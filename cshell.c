#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "cshelllib.h"

int main(int argc, char* argv[])
{
    while (1)
    {
        printf("cshell$ ");
        char c[512] = "";
        scanf("%s",c);

        if (!strcmp(c,"exit"))
        {
            exiting();
            return 0;
        }
        else if (!strcmp(c, "log")) {}
        else if (!strcmp(c, "print")) {}
        else if (!strcmp(c, "theme")) {}
    }
    return 0;
    // added a comment
}