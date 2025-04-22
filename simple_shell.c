#include "shell.h"

/**
* execute_command - execute the command enter in our shell
* @command: command to read and execute
* @progname: name of the program
* Return: 0 on success, 1 on failure
*/
int execute_command(char *command, char *progname)
{
	pid_t pid;
	int status;

	char *argv[2];

	char *newline = strchr(command, '\n');

	if (command == NULL)
		return (1);
	if (newline)
		*newline = '\0';
	if (strchr(command, ' ') != NULL || (command[0] != '/' && command[0] != '.'))
	{
		write(2, progname, strlen(progname));
		write(2, ": No such file or directory\n", 28);
		return (1);
	}
	pid = fork();
	if (pid == 0)
	{
		argv[0] = command;
		argv[1] = NULL;
		execve(command, argv, environ);
		write(2, progname, strlen(progname));
		write(2, ": No such file or directory\n", 28);
		_exit(1);
	}
	if (pid < 0)
		return (1);
	wait(&status);
	return (0);
}

/**
* main - Simple shell program
* @argc: number of arguments
* @argv: array of arguments
* Return: Always 0 (Success)
*/
int main(int argc, char **argv)
{
	char *input = NULL;

	size_t len = 0;
	ssize_t nread;

	(void)argc;
	while (1)
	{
		write(1, "#cisfun$ ", 9);
		nread = getline(&input, &len, stdin);
		if (nread == -1)
		{
			write(1, "\n", 1);
			free(input);
			exit(0);
		}
		if (input[0] == '\n')
			continue;
		execute_command(input, argv[0]);
	}
	return (0);
}
