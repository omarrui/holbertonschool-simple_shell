#include "shell.h"

/**
* find_command - search command in PATH
* @command: command to find
* Return: full path if found, NULL if not found
*/
char *find_command(char *command)
{
	char *path = _getenv("PATH");

	char *path_copy, *dir, *cmd_path;

	struct stat st;

	if (!path)
		return (NULL);

	path_copy = strdup(path);
	free(path);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		cmd_path = malloc(strlen(dir) + strlen(command) + 2);
		if (!cmd_path)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(cmd_path, "%s/%s", dir, command);

		if (stat(cmd_path, &st) == 0 && access(cmd_path, X_OK) == 0)
		{
			free(path_copy);
			return (cmd_path);
		}
		free(cmd_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
