# Operating Systems Assignment 10 Report

## Overview

This submission extends the xv6 kernel with four memory-management and page-replacement exercises:

1. Physical-page allocation statistics.
2. Virtual-to-physical address translation.
3. Page-table inspection and read-only protection.
4. FIFO, LRU, and Optimal page-replacement simulation.

The `work` directory contains only the source files needed to reproduce the assignment changes and the corresponding user programs. Generated object files, binaries, disk images, and unrelated xv6 files are intentionally excluded.

## Q1: Physical Memory Allocation Statistics

### Files

- `kalloc.c`
- `defs.h`
- `sysproc.c`
- `syscall.c`
- `syscall.h`
- `user.h`
- `usys.S`
- `Makefile`
- `memtest.c`

### Implementation

Three kernel counters were added to `kalloc.c`:

- `total_allocated_pages`: number of successful page allocations.
- `total_freed_pages`: number of pages returned to the allocator.
- `available_pages`: current number of pages on the allocator free list.

`kalloc()` removes one page from the free list and updates the allocation and available-page counters only when a page is actually available. `kfree()` returns a page to the free list and increments the freed and available-page counters.

The allocator's existing spinlock is used when locking is enabled, so counter updates occur in the same protected section as free-list changes. This keeps the statistics consistent with allocator state.

The `memstats()` system call is exposed through the normal xv6 system-call path:

`memtest.c` -> `user.h`/`usys.S` -> `syscall.h`/`syscall.c` -> `sysproc.c`

The system call prints total allocations, total frees, currently allocated pages, and available physical pages. The user program calls it before allocation, after requesting five pages with `sbrk(5 * 4096)`, and after releasing those pages with `sbrk(-5 * 4096)`.

### Expected behavior

The first statistics report establishes the initial state. The second shows the five-page increase caused by the allocation, and the final report shows the free-page count restored after release while cumulative allocation/free counters reflect the operations performed.

## Q2: Virtual-to-Physical Address Translation

### Files

- `vm.c`
- `defs.h`
- `sysproc.c`
- `syscall.c`
- `syscall.h`
- `user.h`
- `usys.S`
- `Makefile`
- `vttest.c`

### Implementation

The `vtop()` kernel helper in `vm.c` translates a user virtual address using the current process page directory. It uses `walkpgdir()` to locate the page-table entry, verifies that the entry is present, extracts the physical page-frame address with `PTE_ADDR`, and preserves the original page offset with `va % PGSIZE`.

The final physical address is therefore:

`physical frame base + virtual page offset`

Invalid, unmapped, or unavailable addresses return `-1` instead of being translated.

`sys_vtop()` obtains the user argument and forwards it to `vtop()`. `vttest.c` checks both the beginning of an allocated page and an address 100 bytes into that page. The two physical addresses should differ by 100, demonstrating that page translation preserves the offset. It also checks an invalid high virtual address and reports the `-1` result.

## Q3: Page Information and Read-Only Protection

### Files

- `vm.c`
- `defs.h`
- `sysproc.c`
- `syscall.c`
- `syscall.h`
- `user.h`
- `usys.S`
- `Makefile`
- `pagetest.c`

### Implementation

`pageinfo()` locates the page-table entry for a supplied virtual address and reports:

- the physical frame address,
- whether the page is present (`P`),
- whether it is writable (`W`), and
- whether it is user accessible (`U`).

`setreadonly()` locates the same PTE, clears `PTE_W`, and reloads the current page directory with `lcr3()` so that the processor observes the updated permissions. The operation returns an error for an invalid or unmapped address.

The system calls are exposed as `pageinfo()` and `setreadonly()` through the same user stub and syscall-number mechanism used for Q1 and Q2.

`pagetest.c` allocates one page, prints its initial page information, marks it read-only, prints the information again, and writes to the protected page. The expected result is that the writable flag changes from enabled to disabled and the attempted write causes a protection fault before the success message can execute.

## Q4: Page-Replacement Algorithms

### File

- `pagereplace.c`

### Reference string

`7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1`

### Implementation

The program implements three simulations:

- **FIFO:** replaces pages in cyclic arrival order using a replacement index.
- **LRU:** records the most recent use time for every frame and replaces the least recently used page.
- **Optimal:** searches future references and replaces the page whose next use is farthest away, or a page that is never used again.

For each reference, the program prints the frame contents and whether the reference was a hit or fault. It first runs a detailed simulation for the requested frame count, defaulting to three frames. It then prints a comparison for three, four, and five frames with faults, hits, hit percentage, and fault percentage.

### Supplied Q4 results

The final xv6 output supplied for this report produced these comparison values:

| Algorithm | Frames | Faults | Hits | Hit % | Fault % |
|---|---:|---:|---:|---:|---:|
| FIFO | 3 | 15 | 5 | 25 | 75 |
| LRU | 3 | 12 | 8 | 40 | 60 |
| Optimal | 3 | 9 | 11 | 55 | 45 |
| FIFO | 4 | 10 | 10 | 50 | 50 |
| LRU | 4 | 8 | 12 | 60 | 40 |
| Optimal | 4 | 8 | 12 | 60 | 40 |
| FIFO | 5 | 9 | 11 | 55 | 45 |
| LRU | 5 | 7 | 13 | 65 | 35 |
| Optimal | 5 | 7 | 13 | 65 | 35 |

The detailed 3-frame run reported 15 FIFO faults, 12 LRU faults, and 9 Optimal faults. As expected, the Optimal algorithm performs best for three frames. Increasing the number of frames reduces faults for FIFO and LRU on this reference string; with five frames, LRU and Optimal both produce seven faults.

## Build and run commands

From the complete `xv6-public` directory, rebuild the image and start xv6 using the repository's normal commands. At the xv6 shell, run:

```text
memtest
vttest
pagetest
pagereplace 3
pagereplace 4
pagereplace 5
```

`pagetest` is expected to terminate at the intentional write-protection fault. The Q4 evidence image records the supplied runs and comparison table.

## Submission contents

The `work` directory includes the changed kernel and interface files, the four user-space test programs, `README.md`, `REPORT.md`, and the Q4 result image. Build artifacts and unrelated xv6 files are omitted so the directory is suitable for uploading to GitHub.