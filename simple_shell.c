#include "shell.h"

/**
* execute_command - execute the command enter in our shell
*
* @command: command to read and execute
*
* Return: 0 on success, 1 on failure
*/
int execute_command(char *command, char *progname)
{
	pid_t pid;
	int status;
	char *newline = strchr(command, '\n');

	if (command == NULL)
		return (1);
	if (newline)
		*newline = '\0';
	if (strchr(command, ' ') != NULL)
	{
		fprintf(stderr, "%s: No such file or directory\n", progname);
		return (1);
	}
	if (command[0] != '/' && command[0] != '.')
	{
		fprintf(stderr, "%s: No such file or directory\n", progname);
		return (1);
	}
	pid = fork();
	if (pid == 0)
	{
		char *argv[2];

		argv[0] = command;
		argv[1] = NULL;

		if (execve(command, argv, environ) == -1)
		{
			fprintf(stderr, "%s: No such file or directory\n", progname);
			exit(1);
		}
	}
	else if (pid > 0)
		wait(&status);
	else
	{
		perror("fork");
		return (1);
	}
	return (0);
}

/**
* main - Simple shell program
*
* Description: This program continuously prompts the user for a command,
*              reads the input, and executes it using execve.
* @argc: nb arguments
* @argv: list of arguments
*
* Return: Always 0 (Success)
*/
int main(int argc, char **argv)
{
	char *input = NULL; /* Input buffer */
	size_t len = 0;     /* Length of input */
	ssize_t nread;      /* Number of bytes read */
	(void)argc;

	/* Infinite loop to continuously read and execute commands */
	while (1)
	{
		printf("#cisfun$ ");
		fflush(stdout); /* Task 1: Display the prompt */
		/* Read input from stdin */
		nread = getline(&input, &len, stdin);
		/* Task 1: Handle End of File (Ctrl+D) */
		if (nread == -1)
		{
			free(input);
			printf("\n");
			exit(0); /* Exit the shell */
		}

		/* Task 1: Skip empty lines */
		if (input[0] == '\n')
		{
			continue;
		}

		execute_command(input, argv[0]);
	}
	free(input); /* Task 1: Free input buffer before exiting */
	return (0);
}
