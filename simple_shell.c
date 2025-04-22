#include "shell.h"

/**
 * execute_command - execute the command enter in our shell
 *
 * @command: command to read and execute
 *
 * Return: 0 on success, 1 on failure
 */
int execute_command(char *command)
{
	char *argv[64];     /* Arguments array */
	char *token;        /* Tokenized input */
	int i;              /* Loop counter */
	/* Task 3: Tokenize input into command and arguments */
	token = strtok(command, " \n");
	i = 0;
	while (token != NULL)
	{
		argv[i] = token;
		token = strtok(NULL, " \n");
		i++;
	}
	argv[i] = NULL; /* Null-terminate the argument list */
	/* Task 3: Fork a child process to execute the command */
	if (fork() == 0) /* Child process */
	{
		if (execve(argv[0], argv, environ) == -1) /* Task 3: Execute command */
		{
			perror("./shell");
			exit(1);
		}
	}
	else /* Parent process */
	{
		wait(NULL); /* Task 3: Wait for child process to finish */
	}
		/* Task 2: Placeholder for Task 2 functionality */
		/**
		 * Task 2 should handle:
		 * show not found error with correct format
		 * handle path (ls pwd...)
		 * use access() to test commands
		 * keep track of line count for error message
		 */
	return (0);
}

/**
 * main - Simple shell program
 *
 * Description: This program continuously prompts the user for a command,
 *              reads the input, and executes it using execve.
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char *input = NULL; /* Input buffer */
	size_t len = 0;     /* Length of input */
	ssize_t nread;      /* Number of bytes read */

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

		execute_command(input);
	}
	free(input); /* Task 1: Free input buffer before exiting */
	return (0);
}
