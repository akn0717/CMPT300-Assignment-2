#include <time.h>
#include <stdio.h>
#include "cshelllib.h"

int exiting() {
    printf("Bye!\n");
    return 0;
}
void logging(command *list) {}

void printing(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        

    }
    printf("printing test");
}

void theming() {}

void adding_log(command *list, size_t size, char *name, struct tm *time, char *return_value)
{
    list[size].name = name;
    list[size].time = *time;
    list[size].return_value = return_value;
    ++size;
};

void command_casting() {}
