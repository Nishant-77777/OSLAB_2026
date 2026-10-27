# Operating Systems Lab 8
**Student:** Nishant | **Roll No:** 2403MC02 | **Course:** MA3104

## What this submission covers

This submission explores four memory-management changes in xv6. Each part is
kept in its own directory, with its kernel changes, user-level check program,
patch notes, and supporting output kept together.

- **Q1 - Address translation:** a user-visible `va2pa` call reports how a
  virtual address is resolved and demonstrates that a forked process receives
  a different physical page.
- **Q2 - Memory accounting:** `getmeminfo` reports free frames, total frames,
  user-space pages, and page-table pages while the process grows and forks.
- **Q3 - Page-table inspection:** `vmprint` displays the user mappings,
  identifies the protected stack guard page, and summarizes kernel mappings.
- **Q4 - Lazy heap allocation:** heap growth reserves virtual space first;
  physical pages are created only when the process touches them.

## How I checked the work

The four test programs use separate, descriptive checkpoints so that changes
can be followed in the xv6 console:

1. `q1/vatest.c` compares global, stack, heap, and unmapped addresses.
2. `q2/memtest.c` compares the free-page snapshot before fork with the value
   after the child has been reaped.
3. `q3/vmprinttest.c` prints the address space before growth, after growth,
   and after one page is released.
4. `q4/lazytest.c` touches selected locations in a large reserved region and
   exercises fork, kernel buffer copying, and an invalid access.

Run each question from its own directory with the xv6 toolchain:

```text
make clean && make qemu-nox
```

The PNG files are retained as terminal evidence and are intentionally not
modified.
