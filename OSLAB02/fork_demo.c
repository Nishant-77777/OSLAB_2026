#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        printf("I am child, PID = %ld\n", (long)getpid());
        fflush(stdout);
        _exit(EXIT_SUCCESS);
    }

    // Waiting first makes the child's message appear before the parent's.
    if (waitpid(child_pid, NULL, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    printf("I am parent, PID = %ld\n", (long)getpid());
    return EXIT_SUCCESS;
}