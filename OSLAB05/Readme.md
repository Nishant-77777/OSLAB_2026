# Assignment 5: Process Synchronization using xv6

**Name:** Nishant  
**Roll Number:** 2403MC02  
**Branch:** Mathematics and Computing  

## 1. System Call Integration
To expose our custom synchronization primitives to user space, the xv6 system call interface was extended. The following architectural changes were made:
* **`user/user.h` & `user/usys.pl`:** Added user-space function prototypes and assembly stubs for `shm_get`, `sem_init`, `sem_wait`, and `sem_post`.
* **`kernel/syscall.h`:** Defined unique system call numbers (e.g., `SYS_shm_get`).
* **`kernel/syscall.c`:** Added external declarations and registered the new system call functions in the kernel's execution dispatch table.
* **`kernel/defs.h`:** Appended kernel-level prototypes for the functions defined in our custom `.c` files.
* **`Makefile`:** Appended `_peterson`, `_prodcons`, `_readwrite`, and `_dining` to the `UPROGS` compilation list.

## 2. Shared Memory Implementation (`kernel/shm.c`)
Because standard xv6 strictly isolates process memory, a shared page mechanism was required[cite: 1]. 
* **Allocation:** `shm_get()` uses a global kernel pointer. On its first invocation, it uses `kalloc()` to allocate exactly one new physical page and zeroes it out.
* **Mapping:** It calls `mappages()` to map this exact physical page to a fixed, high virtual address (`0x3FFFF000`) in the calling process's hardware page table. 
* **Usage:** Because the `fork()` syscall duplicates isolated memory spaces, both the parent and child must call `shm_get()` *after* forking to independently map that high virtual address to the shared physical page.

## 3. Semaphore Implementation (`kernel/semaphore.c`)
A lightweight counting semaphore array was engineered in the kernel using a custom struct containing an integer count and an xv6 `spinlock`.
* **`sem_init(id, max_val)`:** Initializes the spinlock for the requested semaphore ID and sets its starting count.
* **`sem_wait(id)`:** Acquires the spinlock. If the count is 0, it calls xv6's built-in `sleep()` function. This safely yields the CPU while using the semaphore's memory address as the specific "wait channel." Once woken up, it decrements the count.
* **`sem_post(id)`:** Acquires the spinlock, increments the count, and immediately calls `wakeup()` on the semaphore's wait channel, unblocking any sleeping processes.

## 4. User-Space Solutions
* **`peterson.c` (Mutual Exclusion):** Casts the shared memory page to a struct containing the `flag[2]` array and `turn` variable. Enforces mutual exclusion strictly through software-level busy-waiting[cite: 1].
* **`prodcons.c` (Producer-Consumer):** Creates a bounded circular buffer (size 5) over shared memory. It synchronizes access using three counting semaphores (`empty`, `full`, and `mutex`) to prevent buffer overflow/underflow[cite: 1].
* **`readwrite.c` (Readers-Writers):** Resolves the problem without writer starvation[cite: 1]. It introduces a turnstile semaphore (`SEM_ENTRY`). If a writer arrives, it waits at the turnstile, forcing any newly arriving readers to block. This guarantees the writer will eventually execute once the active readers finish.
* **`dining.c` (Dining Philosophers):** Avoids deadlock via an asymmetric resource acquisition strategy. Odd-indexed philosophers request their left fork first, while even-indexed philosophers request their right fork first[cite: 1]. This breaks the strict circular wait requirement for deadlocks.

## Build & Execution
1. Compile the kernel: `make clean` followed by `make qemu`.
2. Execute the programs: run `peterson`, `prodcons`, `readwrite`, or `dining` in the xv6 shell.
3. Exit QEMU: Press `Ctrl+A`, release, then press `X`.