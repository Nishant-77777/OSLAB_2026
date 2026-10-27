# Q3 - Printing a process page table

**Student:** Nishant (2403MC02)

## Implementation summary

`vmprint` walks the two-level xv6 page-table structure for the current
process. Each valid directory entry is printed with its page-table frame and
flags. User-space page-table entries are then shown with indentation,
including their virtual address, physical frame, and P/W/U bits. Kernel
entries are counted in a compact summary instead of being expanded.

The test calls the routine three times. The first call captures the initial
layout, the second follows an eight-page heap increase, and the third follows
the release of one page. The guard page created by `exec()` remains mapped but
has user access cleared, so it can be distinguished from ordinary user
pages.

## Run

```text
make clean && make qemu-nox
vmprinttest
```

The three labelled dumps are intended to be read alongside the Q3 terminal
evidence.
