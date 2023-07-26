#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64
/**
 * main	-	simple	shell	program	
 * version:	0.1	
 * 
 * Return:	return	is	0
 */
int main(void)
{
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];
    while (1)
    {
        printf("#cisfun$	"); /*	Display	the	prompt	*/
        if (fgets(command, sizeof(command), stdin) == NULL)
        { /*	Handle	the	"end	of	file"	condition	(Ctrl+D)	*/
            printf("\n");
            break;
        }                                       /*	Remove	the	newline	character	from	the	input	*/
        command[strcspn(command, "\n")] = '\0'; /*	Tokenize	the	command	and	its	arguments	*/
        int arg_count = 0;
        args[arg_count] = strtok(command, "	");
        while (args[arg_count] != NULL)
        {
            arg_count++;
            args[arg_count] = strtok(NULL, "	");
        }
        if (arg_count == 0)
        { /*	Empty	command	*/
            continue;
        }
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {                           /*	Child	process	*/
            args[arg_count] = NULL; /*	Set	the	last	element	to	NULL	for	execvp	*/
            if (execvp(args[0], args) == -1)
            { /*	If	an	executable	cannot	be	found,	print	an	error	message	*/
                perror(args[0]);
                exit(EXIT_FAILURE);
            }
        }
        else
        { /*	Parent	process	*/
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}