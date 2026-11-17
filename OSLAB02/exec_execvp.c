#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *arguments[] = {"ls", "-l", NULL};

    execvp(arguments[0], arguments);

    // execvp returns only when the command could not be started.
    perror("execvp");
    return EXIT_FAILURE;
}