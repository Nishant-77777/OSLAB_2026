# Operating Systems Lab Assignment 2

These programs demonstrate process creation, waiting, and the three requested
`exec` variants.

## Build

Run these commands in Linux or WSL:

```bash
make
```

## Run

```bash
./fork_demo
./exec_execl
./exec_execlp
./exec_execvp
./runcmd echo hello
./runcmd sleep 1
```

`fork_demo` waits for the child before printing the parent's message, so the
child output is consistently shown first. The three `exec` programs all run
`ls -l`; they differ only in how the command and its arguments are supplied.

`runcmd` expects exactly two arguments after its program name: a command and
one argument. For example, `./runcmd ls -l` runs `ls -l` in the child process.

## Clean compiled programs

```bash
make clean
```