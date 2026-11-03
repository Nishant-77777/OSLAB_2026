# xv6 Operating Systems Assignment 10

This folder contains the submission files for Assignment 10. The work adds kernel support and user-space demonstrations for memory statistics, virtual-to-physical translation, page protection, and page-replacement algorithms.

## Contents

### Kernel and system-call changes

- `kalloc.c`: allocation, free, and available-page counters.
- `vm.c`: virtual-address translation, page information, and read-only protection.
- `sysproc.c`: system-call implementations.
- `syscall.c`, `syscall.h`: syscall registration and dispatch.
- `defs.h`: kernel declarations.
- `user.h`, `usys.S`: user-visible declarations and syscall stubs.
- `Makefile`: xv6 user-program inclusion.

### User programs

- `memtest.c`: Q1 allocator statistics demonstration.
- `vttest.c`: Q2 address translation demonstration.
- `pagetest.c`: Q3 page flags and write-protection demonstration.
- `pagereplace.c`: Q4 FIFO, LRU, and Optimal simulation.

### Documentation and evidence

- `REPORT.md`: detailed explanation of every question, implementation path, and result.
- `Q4-results.png`: consolidated Q4 output screenshot containing the detailed run and 3/4/5-frame comparison.

## Q4 result summary

The supplied Q4 execution used the reference string:

```text
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
```

| Algorithm | 3 frames | 4 frames | 5 frames |
|---|---:|---:|---:|
| FIFO faults | 15 | 10 | 9 |
| LRU faults | 12 | 8 | 7 |
| Optimal faults | 9 | 8 | 7 |

The detailed percentages and hit counts are documented in `REPORT.md`.

## Running the programs

These files are intended to be copied into the complete xv6-public source tree, or used from the completed tree that contains the corresponding kernel changes. Rebuild xv6, boot it, and execute:

```text
memtest
vttest
pagetest
pagereplace 3
pagereplace 4
pagereplace 5
```

The write attempted by `pagetest` is intentional and should generate a page-protection fault.

## GitHub upload note

Upload the contents of this `work` folder as the assignment submission. Generated xv6 binaries, object files, disk images, and unrelated source files are not included.