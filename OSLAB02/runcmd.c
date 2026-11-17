#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <command> <argument>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        char *command_arguments[] = {argv[1], argv[2], NULL};
        execvp(command_arguments[0], command_arguments);

        perror("execvp");
        _exit(EXIT_FAILURE);
    }

    int status;
    if (waitpid(child_pid, &status, 0) < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("Command successfully completed\n");
        return EXIT_SUCCESS;
    }

    fprintf(stderr, "Command failed\n");
    return EXIT_FAILURE;
}