#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

extern char **environ;

int execute_command(char *command, char *progname, int count);
void handle_signal(int sig);
char *_getenv(const char *name);
char *find_command(char *command);

#endif
