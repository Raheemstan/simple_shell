#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

char *read_command(void);
void tokenize_command(char *command, char *args[MAX_ARGS]);
void execute_command(char *args[MAX_ARGS]);
void check_exit_command(char *args[MAX_ARGS]);
void print_environment(void);

/**
 * main - simple shell program
 *
 * Return: Always 0.
 */
int main(void)
{
    char *command;
    char *args[MAX_ARGS];

    while (1)
    {
        printf("#cisfun$ ");
        command = read_command();
        if (command == NULL)
        {
            printf("\n");
            break;
        }

        tokenize_command(command, args);

        if (args[0] == NULL)
        {
            continue;
        }

        check_exit_command(args);

        if (strcmp(args[0], "env") == 0)
        {
            print_environment();
            continue;
        }

        execute_command(args);

        free(command);
    }

    return (0);
}

/**
 * read_command - Read the user's command from stdin.
 *
 * Return: Pointer to the command string.
 */
char *read_command(void)
{
    char command[MAX_COMMAND_LENGTH];

    if (fgets(command, sizeof(command), stdin) == NULL)
    {
        return NULL;
    }

    command[strcspn(command, "\n")] = '\0';
    return strdup(command);
}

/**
 * check_exit_command - Check for the exit built-in command.
 * @args: An array of strings representing the command and its arguments.
 */
void check_exit_command(char *args[MAX_ARGS])
{
    if (strcmp(args[0], "exit") == 0)
    {
        printf("Exiting the shell.\n");
        exit(EXIT_SUCCESS);
    }
}

/**
 * print_environment - Print the current environment variables.
 */
void print_environment(void)
{
    extern char **environ;
    char **env = environ;

    while (*env != NULL)
    {
        printf("%s\n", *env);
        env++;
    }
}

/**
 * tokenize_command - Tokenize the command and its arguments.
 * @command: The input command string.
 * @args: An array of strings to store the command and its arguments.
 */
void tokenize_command(char *command, char *args[MAX_ARGS])
{
    int arg_count = 0;
    args[arg_count] = strtok(command, " ");

    while (args[arg_count] != NULL)
    {
        arg_count++;
        args[arg_count] = strtok(NULL, " ");
    }
}

/**
 * execute_command - Execute the command using execvp.
 * @args: An array of strings representing the command and its arguments.
 */
void execute_command(char *args[MAX_ARGS])
{
	char *path = getenv("PATH");
	pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        /* Child process */
        args[MAX_ARGS - 1] = NULL; /* Set the last element to NULL for execvp */

        /* If the command starts with '/', it's already a full path */
        if (args[0][0] == '/')
        {
            execvp(args[0], args);
        }
        else
        {
            /* Check if the command is executable in each directory in the PATH */
            if (path != NULL)
            {
                char buffer[MAX_COMMAND_LENGTH];
                char *token = strtok(path, ":");

                while (token != NULL)
                {
                    /* Create the full path to the command */
                    snprintf(buffer, sizeof(buffer), "%s/%s", token, args[0]);

                    /* Attempt to execute the command */
                    execvp(buffer, args);

                    token = strtok(NULL, ":");
                }
            }
        }

        /* If the command was not found in any directory in the PATH, print an error */
        printf("Command not found: %s\n", args[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        /* Parent process */
        int status;
        waitpid(pid, &status, 0);
    }
}
