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
	int i, j;

	char *compare;

	char *value;

	if (name == NULL)
		return (NULL);

	compare = malloc(strlen(name) + 1);
	if (compare == NULL)
		return (NULL);

	i = 0;
	while (environ[i])
	{
		compare = malloc(strlen(environ[i]) + 1);
		if (compare == NULL)
			return (NULL);

		j = 0;
		while (environ[i][j] != '=' && environ[i][j] != '\0')
		{
			compare[j] = environ[i][j];
			j++;
		}
		compare[j] = '\0';

		/* si les deux chaîne sont pareil */
		if (strcmp(compare, name) == 0)
		{
			/* dupliquer dans value, le contenue de environ[i] a partir de j + 1 */
			value = strdup(environ[i] + j + 1);
			free(compare);
			return (value);
		}
		i++;
	}
	free(compare);
	return (NULL);
}
