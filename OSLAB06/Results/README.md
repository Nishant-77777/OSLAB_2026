# Assignment 6 Results

These are expected-output examples for Assignment 6. I did not run xv6 in this environment, so these files show the kind of output produced by each program and the result that should be checked in QEMU.

## Programs

- `bankers_output.svg`: safe-state check, safe sequence, and a resource request.
- `deadlockdetect_output.svg`: one cyclic wait-for graph and one acyclic graph.
- `orderedlocks_output.svg`: five processes using resource ordering.
- `syncdeadlock_output.svg`: five processes completing multiple resource-sharing cycles.

Run the programs inside xv6 with `bankers`, `deadlockdetect`, `orderedlocks`, and `syncdeadlock` to obtain the actual terminal output.
