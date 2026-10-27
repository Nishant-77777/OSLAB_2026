# Q4 - Allocating heap pages on demand

**Student:** Nishant (2403MC02)

## Design

Positive `sbrk()` requests now extend the process's virtual limit without
immediately reserving physical frames. When the process accesses an
unmapped address in that heap range, the page-fault handler rounds the
address down, allocates a zeroed frame, and maps it with user read/write
permissions.

Shrinking still releases pages that have actually been created. The fault
path rejects addresses outside the heap, addresses below the valid user
range, protected guard-page accesses, and allocation failures by killing only
the faulty process. `copyuvm`, `deallocuvm`, and the kernel copy helpers also
understand that a reserved page may not have a PTE yet.

## Test plan

`lazytest.c` reserves a 32 MB region, touches three widely separated
locations, and reads them back. It then uses an untouched buffer with
`write()`, forks a process whose lazy pages are incomplete, and finally
attempts an access at the heap limit in a child. The printed memory snapshots
make the difference between reserved virtual space and allocated frames
visible.

## Run

```text
make clean && make qemu-nox
lazytest
```

The Q4 screenshot records the demand-allocation, fork, and invalid-access
checks.
