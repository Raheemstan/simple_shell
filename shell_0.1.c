#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

/**
* get_command - function to handle user command
* @command: user command
*/

void get_command(char *command)
{
	printf("#cisfun$ ");/* Display the prompt */
	if (fgets(command, sizeof(command), stdin) == NULL)
	{
		/* Handle the "end of file" condition (Ctrl+D)	*/
		printf("\n");
		exit(EXIT_SUCCESS);
	}
	/* Remove the newline character from the input */
	command[strcspn(command, "\n")] = '\0';
}

/**
 * tokenize_command - Tokenize the command and its arguments
 * @command: The command string
 * @args: An array of strings to store the arguments
 * @arg_count: Pointer to store the argument count
*/

void tokenize_command(char *command, char **args, int *arg_count)
{
	*arg_count = 0;

	args[*arg_count] = strtok(command, " ");

	while (args[*arg_count] != NULL)
	{
		(*arg_count)++;
		args[*arg_count] = strtok(NULL, " ");
	}
	args[*arg_count] = NULL;/* Set the last element to NULL for execvp */
}


/**
* exec_command - function to execute user command
* @args: An array of strings containing the command and its arguments
*/

void exec_command(char **args)
{
	if (execvp(args[0], args) == -1)
	{
		/* If an executable cannot be found, print an error message */
		perror(args[0]);
		exit(EXIT_FAILURE);
	}
}

/**
 * main	- simple shell program
 * version:0.1
 * Return: return is 0
 */

int main(void)
{
	char command[MAX_COMMAND_LENGTH];
	char *args[MAX_ARGS];
	int arg_count, status;
	pid_t pid;

	while (1)
	{
		get_command(command);

		tokenize_command(command, args, &arg_count);

		if (arg_count == 0)
		{
			/* Empty command */
			continue;
		}

		pid = fork();

		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			/* child process */
			exec_command(args);
		}
		else
		{
			/* parent process */
			waitpid(pid, &status, 0);
		}
	}
	return (0);
}
