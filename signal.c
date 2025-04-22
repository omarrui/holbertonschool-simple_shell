#include "shell.h"

/**
 * handle_signal - signal management
 *
 * @sig: number of signal
 *
 * Return: nothing
 */
void handle_signal(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n#cisfun$ ", 10);
}