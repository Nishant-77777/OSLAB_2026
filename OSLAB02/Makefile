CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c11 -pedantic

PROGRAMS = fork_demo exec_execl exec_execlp exec_execvp runcmd

.PHONY: all clean

all: $(PROGRAMS)

fork_demo: fork_demo.c
	$(CC) $(CFLAGS) $< -o $@

exec_execl: exec_execl.c
	$(CC) $(CFLAGS) $< -o $@

exec_execlp: exec_execlp.c
	$(CC) $(CFLAGS) $< -o $@

exec_execvp: exec_execvp.c
	$(CC) $(CFLAGS) $< -o $@

runcmd: runcmd.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMS)