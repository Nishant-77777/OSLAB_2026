# Assignment 9 Report

**Student:** Nishant  
**Roll number:** 2403MC02

## Question 1 - Process page-table printing

`vmprint()` is exposed through a system call and walks the calling process's
two-level x86 page table. It reports each present user mapping with virtual
address, physical frame, and P/W/U flags, while ignoring mappings at or above
`KERNBASE`. It also reports the number of valid user pages and page-table
pages. `pttest` prints the initial mapping and then grows the heap by four
pages, touches each new page, and prints the mapping again. Exactly four
additional user mappings are expected.

## Question 2 - Lazy allocation and page faults

Positive `sbrk()` now reserves virtual address space by increasing `proc->sz`
without allocating frames. The page-fault handler allocates and zeroes a page
only when the faulting address is inside the reserved user range. Each
successful lazy allocation increments `proc->faults`, returned by
`getfaults()`. `deallocuvm()` and `copyuvm()` tolerate absent pages.
`faulttest` touches five different pages and expects five handled faults. A
child then accesses an address above the allocation; the kernel reports the
fault and kills only that child, while the parent continues.

## Question 3 - FIFO and LRU replacement

`pagerepl` implements FIFO and LRU in user space. It prints each reference,
the frame contents, and hit/fault status. For the required 20-reference
string with three frames, the expected totals are FIFO = 15 faults and
LRU = 12 faults. It also prints fault counts and integer hit percentages for
three, four, and five frames.

## Validation and evidence

The host-side source syntax checks passed for all modified kernel and user
sources (with only the original xv6 host-libc warnings). The replacement
algorithm was independently checked against the required reference string;
its totals are 15/12 for three frames and 10/8 and 9/7 for four and five
frames. The local machine does not provide the xv6 cross-compiler, GNU make,
or QEMU, so emulator execution was unavailable. The reproducible commands and
expected transcript are in `OSLAB09/README.md` and
`OSLAB09/results/terminal-output.txt`. SVG terminal evidence for Q1, Q2, and
Q3 is provided in `OSLAB09/results/q1-page-table.svg`,
`OSLAB09/results/q2-lazy-allocation.svg`, and
`OSLAB09/results/q3-page-replacement.png`. The PNG files use the plain black
xv6 console style from the provided Assignment 8 reference image. Each image
contains expected output rather than a live emulator screenshot because QEMU
is unavailable on the preparation machine.
