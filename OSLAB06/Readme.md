# Assignment 6: Deadlock Handling in xv6

**Name:** Nishant  
**Roll Number:** 2403MC02  
**Branch:** Mathematics and Computing  

I copied my Assignment 5 xv6 code into this folder and added the programs required for Assignment 6. The shared-memory and counting-semaphore system calls from the previous assignment are used by the programs that create multiple processes.

## Question 1: Banker's Algorithm

`user/bankers.c` contains the standard five-process, three-resource example. It stores the Allocation, Maximum, and Available arrays, calculates Need as `Maximum - Allocation`, and prints a safe sequence. It also tests a request from P1. The request is temporarily allocated and the safety check is run; the allocation is kept only if the resulting state is safe.

## Question 2: Deadlock Detection

`user/deadlockdetect.c` stores an allocation matrix and a request matrix. It builds a wait-for graph by adding an edge from a process waiting for a resource to every process holding that resource. A depth-first search detects a cycle and prints the cycle when one is found. The hardcoded scenario contains a cycle P0 -> P1 -> P2 -> P0.

## Question 3: Resource Ordering

`user/orderedlocks.c` creates five xv6 processes and uses the three semaphore IDs as resource pools. Every process sorts its two requested resources and always acquires the lower-numbered resource first. Since all processes follow the same ordering, circular wait is prevented. Each process completes three work cycles and releases both resources.

## Question 4: Combined Synchronization and Deadlock Avoidance

`user/syncdeadlock.c` creates five processes. Each process needs two of the three shared resources, and the resource pools have limited instances. The same resource-ordering strategy is used: resources are acquired in increasing order, work is performed, and both resources are released. The output shows each request, grant, work period, and release. All five processes complete three cycles without permanent blocking.

## Files Added or Updated

* Added `user/bankers.c`, `user/deadlockdetect.c`, `user/orderedlocks.c`, and `user/syncdeadlock.c`.
* Updated the `UPROGS` list in `Makefile` so all four programs are included in the xv6 file system image.
* Kept the Assignment 5 programs and synchronization system calls because Questions 3 and 4 use the semaphore implementation.

## Build and Run

```text
make clean
make qemu
```

Inside the xv6 shell, run:

```text
bankers
deadlockdetect
orderedlocks
syncdeadlock
```

To exit QEMU, press `Ctrl+A`, release it, and then press `X`.