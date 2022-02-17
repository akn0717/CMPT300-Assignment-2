#ifndef CSHELLLIB_H
#define CSHELLLIB_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512
#define MAX_N_COMMAND 50    //maximum number of command in the log list
#define MAX_N_VARIABLE 50   //maximum number of variable in the variable list
#define MAX_N_ARGUMENTS 50  //maximum number of argument in a command

//number uppercase system call
#define SYS_UPCASE 440

typedef struct command
{
    char *name;
    struct tm time;
    int return_value;
} command;

typedef struct {
    char *name;
    char *value;
} EnvVar;

//input a string from userspace and uppercase all the letter 
int print_uppercase(char *str);

//free all the memory in this program
void free_memory(command **comm_list, size_t comm_list_size, EnvVar **variable_list, size_t varl_size, char **command_argv, size_t n_commands, char* buffer);

//print log
int logging(command **comm_list, size_t comm_list_size);

//print bult-in function
int printing(EnvVar ** var_list, size_t varl_size, size_t argc, char ** argv);

//theme bult-in funtion
int theming(char * colour);

//add new command log into the log list
void adding_log(command **comm_list, size_t *comm_list_size, char *name, struct tm time, int return_value);

//parse the command arguments into multiple variable strings
int command_parsing(char *buffer, size_t *argc, char **argv);

//find memory address of an environment variable
EnvVar *find_variable(EnvVar **variable_list, size_t varl_size, char *name);

//assign value to a variable if exists, else add new variable into the environment variable list
int variable_assigning(EnvVar **variable_list, size_t * varl_size, char *name, char *value);

#endif /* CSHELLLIB_H */