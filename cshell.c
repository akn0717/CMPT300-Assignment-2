#include "cshelllib.h"

int main(int argc, char* argv[])
{
    FILE *fptr;
    int flag = 0;

    // open the script file and read from the file if the 
    // file name is mentioned when running the program (./cshell myscript.txt)
    if (argc > 1)
    {
        fptr = fopen(argv[1],"r");
        if ( fptr == NULL )
        {
            // error if file name is incorrectly spelled.
            fprintf (stderr, "Unable to read the script file %s \n" , argv[1]) ;
            return 1;
        }
        flag = 1;
    }

    // Initializing different variables

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

    //Initializing timer variables
    time_t raw_time;
    struct tm *time_info;
    time(&raw_time);

    // keeping the default cshell colour white. user input will always show up in white.
    char colour_name[15] = "white";

    // loop to run the cshell until the exit command is received.
    while (1)
    {
        memset(buffer,0,buffer_size);
        if (flag==0)
        {
            printf("cshell$ ");
            // change colour to white for the user input as shown in the assignment requirements.
            theming("white");
            getline(&buffer, &buffer_size, stdin);
            // change the colour back to what it was before taking the user input
            theming(colour_name);
        }
        else
        {
            // read through each line of myscript.txt and exit after the last line
            if (fgets (buffer, buffer_size, fptr ) == NULL)
            {
                fclose ( fptr );
                break;
            }
        }
        int parsing_error = command_parsing(buffer, &command_argc, command_argv);
        time(&raw_time);

        // error if syntax is incorrect during new variable assignment
        if (parsing_error==1)
        {
            printf("Incorrect syntax for assigning a variable. \n");
            continue;
        }
        else if (parsing_error==2)
        {
            continue;
        }

        // exit function
        else if (!strcmp(command_argv[0],"exit"))
        {
            break;
        }

        // function to assign variable value
        else if (command_argv[0][0] == '$')
        {
            return_value = variable_assigning(variable_list, &varl_size, command_argv[0], command_argv[1]);

            //push the value variable into the first args for easier logging
            strcat(command_argv[0], "=");
            strcat(command_argv[0], command_argv[1]);
        }
        else if (!strcmp(command_argv[0], "log"))
        {
            //function to print log
            return_value = logging(comm_list, comm_list_size);
        }
        else if (!strcmp(command_argv[0], "print"))
        {
            //function to print
            return_value = printing(variable_list, varl_size, command_argc, command_argv);
            if (return_value==1)
            {
                printf("Non-existing variable name!\n");
            }
        }
        else if (!strcmp(command_argv[0], "theme"))
        {
            //function to change theme
            return_value = theming(command_argv[1]);
            if(return_value == 0)
            {
                strcpy(colour_name, command_argv[1]);
            }
        }
        else if (!strcmp(command_argv[0], "uppercase"))
        {
            //function to call syscall 440 (uppercase systemcall)
            return_value = print_uppercase(command_argv[1]);
        }
        else 
        {
            //create pipe
            int fds[2];
            int error = pipe(fds);
            //catch error if creating pipe has problem
            if (!error)
            {
                //fork into 2 processes
                pid_t pid = fork();

                //if current process is a child
                if (pid==0)
                {
                    //close the reading pipe
                    close(fds[0]);
                    //redirect stdout_fileno and stderr_fileno to the writing pipe
                    dup2(fds[1], STDOUT_FILENO);
                    dup2(fds[1], STDERR_FILENO);

                    //execute non built-in command
                    execvp(command_argv[0] , command_argv);
                    
                    //case if the command is not available
                    //print error into the pipe for parent to catch
                    fprintf(stderr, "Missing keyword or command, or permission problem\n");

                    //close writing pipe
                    close(fds[1]);

                    //close the script file pointer in child that forked from the parent
                    fclose(fptr);

                    //free all heap memory copied from the parent
                    free_memory(comm_list, comm_list_size, variable_list, varl_size, command_argv, command_argc, buffer);
                    
                    //exit the child with error
                    exit(EXIT_FAILURE);
                }
                else if (pid>0) //current process is parent
                {
                    char data[2] = "";
                    //close writing side of pipe
                    close(fds[1]);
                    //parent reads from pipe
                    while (read(fds[0], data, 1) == 1)
                    {
                        printf("%s", data);
                    }

                    //wait for child
                    wait(&error);
                    //close pipe
                    close(fds[0]);
                }
                //error when child process cannot run or incorrect commands
                if (error) error = 2;
            } else error = 1;   //error if cannot create pipe

            return_value = error;
            if (return_value == 1)
            {
                printf("Can't create pipe!\n");
            }
        }

        //change timer to local time
        time_info = localtime(&raw_time);
        //add into the log list
        adding_log(comm_list, &comm_list_size, command_argv[0], *time_info, return_value);
    }

    //free all allocated variables
    free_memory(comm_list, comm_list_size, variable_list, varl_size, command_argv, command_argc, buffer);
    printf("Bye!\n");
    return 0;
}