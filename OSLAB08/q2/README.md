# Q2 - Tracking physical memory usage

**Student:** Nishant (2403MC02)

## Implementation summary

The allocator maintains free-frame and total-frame counters while holding
`kmem.lock`. A `meminfo` structure is filled by `getmeminfo`, which also
reports the calling process's virtual-page count and the page-table pages
needed by its address space.

`memtest.c` records a baseline, grows the heap by eight pages, and then forks.
The parent and child observations show the extra allocation made by the
child. The important check is performed after `wait()`: the free count is
compared with the snapshot taken immediately before `fork()`. Equal values
indicate that the child's pages were returned correctly.

## Run

```text
make clean && make qemu-nox
memtest
```

The expected sequence and measured values are documented in the Q2 screenshot
and in the allocator-related patch notes.
