#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * handle_exit - exits the shell with optional status
 * @args: the parsed arguments (argv)
 * @line: the original input line (to be freed)
 */
void handle_exit(char **args, char *line)
{
	int status = 0;

	if (args[1])
		status = atoi(args[1]);

	free(line);
	exit(status);
}
