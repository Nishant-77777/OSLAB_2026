#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    execlp("ls", "ls", "-l", (char *)NULL);

    // execlp searches for ls in the PATH environment variable.
    perror("execlp");
    return EXIT_FAILURE;
}