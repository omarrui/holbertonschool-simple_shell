#include "shell.h"

/**
 * execute_command - execute the command enter in our shell
 *
 * @command: command to read and execute
 * @progname: name of file
 *
 * Return: 0 on success, 1 on failure
 */
int execute_command(char *command, char *progname)
{
	pid_t pid;
	int status;
	char **argv;

	/* Check for empty command */
	if (command == NULL || *command == '\0')
		return (1);
	if (access(command, X_OK) == -1)
	{
		fprintf(stderr, "%s: No such file or directory\n", progname);
		return (127);
	}
	argv = malloc(2 * sizeof(char *));
	if (argv == NULL)
	{
		perror(progname);
		return (1);
	}
	argv[0] = command;
	argv[1] = NULL;
	/* Create child process */
	pid = fork();
	if (pid == -1)
	{
		free(argv);
		perror(progname);
		return (1);
	}
	if (pid == 0)
	{
		/* Child process */
		if (execve(command, argv, environ) == -1)
		{
			perror(progname);
			free(argv);
			exit(127);
		}
	}
	else
	{
		/* Parent process */
		wait(&status);
		free(argv);
	}
	return (WEXITSTATUS(status));
}

/**
 * main - call another function to read command
 *
 * @argc: nb arguments
 * @argv: list of arguments
 *
 * Return: 0 or -1 on failure
 */
int main(int argc, char **argv)
{
	char *line = NULL;

	size_t len = 0;
	ssize_t nread;
	(void)argc;

	/* Setup signal handler */
	signal(SIGINT, handle_signal);

	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("#cisfun$ ");
		fflush(stdout);

		/* Get command from user */
		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			free(line);
			exit(0);
		}

		/* Remove newline */
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Skip empty lines */
		if (*line == '\0')
			continue;
		
		if (strcmp(line, "exit") == 0)
		{
			free(line);
			exit(0);
		}

		/* Execute command */
		execute_command(line, argv[0]);
	}

	free(line);
	return (0);
}
