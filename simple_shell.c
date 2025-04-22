#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
    char *input = NULL;  /* Input buffer */
    size_t len = 0;      /* Length of input */
    ssize_t nread;       /* Number of bytes read */
    char *token;         /* Tokenized input */
    char *argv[64];      /* Arguments array */
    int i;               /* Loop counter */

    /* Infinite loop to continuously read and execute commands */
    while (1)
    {
        printf("$ ");
        fflush(stdout);  /* Task 1: Display the prompt */

        /* Read input from stdin */
        nread = getline(&input, &len, stdin);

        /* Task 1: Handle End of File (Ctrl+D) */
        if (nread == -1)
        {
            free(input);
            printf("\n");
            exit(0);  /* Exit the shell */
        }

        /* Task 1: Skip empty lines */
        if (input[0] == '\n')
        {
            continue;
        }

        /* Task 3: Tokenize input into command and arguments */
        token = strtok(input, " \n");
        i = 0;
        while (token != NULL)
        {
            argv[i] = token;
            token = strtok(NULL, " \n");
            i++;
        }
        argv[i] = NULL;  /* Null-terminate the argument list */

        /* Task 3: Fork a child process to execute the command */
        if (fork() == 0)  /* Child process */
        {
            extern char **environ;
            if (execve(argv[0], argv, environ) == -1)  /* Task 3: Execute command */
            {
                perror("Error");
                exit(1);
            }
        }
        else  /* Parent process */
        {
            wait(NULL);  /* Task 3: Wait for child process to finish */
        }

        /* Task 2: Placeholder for Task 2 functionality */
        /* Task 2 should handle:
           - Command error handling
           - Displaying error messages
           - Command execution in the context of a simple shell
           - Managing end-of-file (Ctrl+D) properly
           */
    }

    free(input);  /* Task 1: Free input buffer before exiting */
    return (0);
}
