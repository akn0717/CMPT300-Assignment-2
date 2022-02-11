typedef struct command
{
    char *name;
    struct tm time;
    char *return_value;
} command;

typedef struct {
    char *name;
    char *value;
} EnvVar;


int exiting();

void logging();

void printing();

void theming();

void adding_log(command *list, size_t size, char *name, struct tm *time, char *return_value);

void command_casting();

