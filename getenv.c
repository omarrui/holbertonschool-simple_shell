#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
* _getenv - get the value of env variable
*
* @name: name of env variable
*
* Return: pointer to the value in the environment, or NULL
*/
char *_getenv(const char *name)
{
	int i;

	char *value;

	size_t name_len;

	if (name == NULL)
		return (NULL);

	name_len = strlen(name);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
		{
			value = strdup(environ[i] + name_len + 1);
			return (value);
		}
	}
	return (NULL);
}
