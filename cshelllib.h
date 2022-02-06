typedef struct command
{
    char* name;
    struct tm time;
    char* return_value;
} command;

void exiting();

void logging();

void printing();

void theming();

void command_casting();

