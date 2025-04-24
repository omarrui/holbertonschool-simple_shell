#include "shell.h"

/**
 * execute_builtin - check if command is built-in and execute it
 *
 * @argv: array of command arguments
 * @progname: program name for error messages
 * @line: input line to free if exiting
 *
 * Return: 0 if built-in was executed, 1 if not a built-in
 */
int execute_builtin(char **argv, char *progname, char *line)
{
	int status = 0, i;
	char *dir = argv[1];

	if (!argv || !argv[0])
		return (1);

	if (strcmp(argv[0], "exit") == 0)
	{
		if (argv[1] != NULL)
			status = atoi(argv[1]);
		free(line);
		exit(status);
	}

	if (strcmp(argv[0], "env") == 0)
	{
		for (i = 0; environ[i]; i++)
			printf("%s\n", environ[i]);
		return (0);
	}

	if (strcmp(argv[0], "cd") == 0)
	{
		if (dir == NULL || strcmp(dir, "~") == 0)
			dir = _getenv("HOME");

		if (chdir(dir) == -1)
			fprintf(stderr, "%s: cd: %s: No such file or directory\n",
				progname, argv[1]);
		return (0);
	}
	return (1);
}

/**
 * execute_command - execute the command enter in our shell
 *
 * @command: command to read and execute
 * @progname: name of file
 * @count: nb command
 *
 * Return: 0 on success, 1 on failure
 */
int execute_command(char *command, char *progname, int count)
{
	pid_t pid;
	int status, i = 0, arg_count = 0;
	char **argv;
	char *token, *cmd_copy, *full_path, *tmp;

	while (*command && *command == ' ')
		command++;
	
	if (*command == '\0')
		return (0);

	cmd_copy = strdup(command);
	if (cmd_copy == NULL)
	{
		perror(progname);
		return (1);
	}
	token = strtok(cmd_copy, " \n");
	while (token != NULL)
	{
		arg_count++;
		token = strtok(NULL, " \n");
	}
	free(cmd_copy);
	argv = malloc((arg_count + 1) * sizeof(char *));
	if (argv == NULL)
	{
		perror(progname);
		return (1);
	}
	token = strtok(command, " \n");
	while (token != NULL && i < arg_count)
	{
		argv[i] = token;
		token = strtok(NULL, " \n");
		i++;
	}
	argv[i] = NULL;

	if (!execute_builtin(argv, progname, command))
	{
		free(argv);
		return (0);
	}

	if (command == NULL || *command == '\0')
		return (1);
	if (strchr(argv[0], '/') == NULL)
	{
		full_path = find_command(argv[0]);
		if (full_path != NULL)
		{
			tmp = strdup(full_path);
			free(full_path);
			if (tmp == NULL)
			{
				free(argv);
				return (1);
			}
			argv[0] = tmp;
		}
	}

	if (access(argv[0], X_OK) == -1)
	{
		fprintf(stderr, "%s: %d: %s: not found\n", progname, count, argv[0]);
		if (argv[0] != command)
			free(argv[0]);
		free(argv);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		if (argv[0] != command)
			free(argv[0]);
		free(argv);
		perror("Error: fork failed");
		return (1);
	}
	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			fprintf(stderr, "%s: %d: %s: not found\n", progname, count, command);
			if (argv[0] != command)
				free(argv[0]);
			free(argv);
			exit(126);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (argv[0] != command)
			free(argv[0]);
		free(argv);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
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
	char *cmd;
	int count = 0;

	size_t len = 0;
	ssize_t nread;
	(void)argc;

	signal(SIGINT, handle_signal);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("($) ");
		fflush(stdout);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			free(line);
			exit(0);
		}

		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		cmd = line;
		while (*cmd && *cmd == ' ')
			cmd++;

		if (*cmd == '\0')
			continue;

		count++;
		execute_command(cmd, argv[0], count);
	}

	free(line);
	return (0);
}
