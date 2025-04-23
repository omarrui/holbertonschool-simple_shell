#include "shell.h"

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

	if (command == NULL || *command == '\0')
		return (1);
	if (strchr(argv[0], '/') == NULL)
	{
		full_path = find_command(argv[0]);
		if (full_path != NULL)
		{
			tmp = strdup(full_path);
			if (tmp == NULL)
			{
				free(full_path);
				free(argv);
				return (1);
			}
			argv[0] = tmp;
			free(full_path);
		}
	}
	if (access(argv[0], X_OK) == -1)
	{
		fprintf(stderr, "%s: %d: %s: not found\n", progname, count, argv[0]);
		free(argv);
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		free(argv);
		perror("Error: fork failed");
		return (1);
	}
	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			fprintf(stderr, "%s: %d: %s: not found\n", progname, count, command);
			free(argv);
			exit(126);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
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
			printf("#cisfun$ ");
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

        if (strncmp(cmd, "exit", 4) == 0)
        {
            handle_exit(argv, line);
        }
		count++;
		execute_command(cmd, argv[0], count);
	}

	free(line);
	return (0);
}
