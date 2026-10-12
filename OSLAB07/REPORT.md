# Consolidated Report — Assignment 7

**xv6: Threads, Synchronization, Deadlock, Scheduling**
**Student:** Nishant | 2403MC02 | MA3104
**Environment:** xv6 on QEMU

## 1. Kernel-Supported Threads (clone / join)

* **Design:** `clone()` creates a thread that shares the parent's page directory (`pgdir`) but utilizes a private kernel stack. It duplicates the open-file table (`filedup`) to allow standard I/O. `join()` reaps thread zombies and frees the stack without destroying the shared address space.
* **Result:** Three threads incrementing a shared counter successfully prove the shared address space works. Without synchronization, concurrent updates are lost, leading to unpredictable, incorrect final values.

## 2. Counting Semaphores

* **Design:** Implemented a kernel-level array of semaphores (`sems[10]`). `sem_wait` blocks via `sleep()` while the value is $\le 0$. `sem_signal` increments the value and calls `wakeup()`. A `while` loop re-check safely handles xv6's default "wakeup-all" behavior.
* **Result:**
* **Mutual Exclusion:** Adding semaphores to the Q1 threads guarantees the counter hits exactly 300,000 every single run.
* **Producer-Consumer:** A bounded buffer (size 5) successfully processes 20 items in strict FIFO order. The producer blocks when full (preventing overflow) and the consumer blocks when empty (preventing underflow).



## 3. Deadlock Detection & Avoidance

* **Design:** Implemented a work-vector scan to detect deadlocks by simulating resource allocation and identifying processes that cannot finish.
* **Result:**
* **Matrix Testing:** Accurately identifies safe states (T1), flags deadlocked sets (T2: {P0, P1}), and denies unsafe resource requests using Banker's Algorithm (T3).
* **Real Deadlock:** Successfully modeled an actual A$\leftrightarrow$B circular wait using pipes. The detector flagged the deadlock, and it was broken by sending a `kill()` signal to unblock the pipe readers.



## 4. Priority Preemptive Scheduling with Aging

* **Design:** Added `priority` (1 = highest, 20 = lowest) and `waitticks` to `struct proc`. The scheduler always selects the runnable process with the lowest priority number, using round-robin for ties.
* **Result:** Execution order strictly correlates with priority (Priority 1 finishes in ~100 ticks, Priority 20 in ~300 ticks). Processes waiting >30 ticks are dynamically aged (priority boosted), successfully preventing the lowest-priority processes from starving.

## 5. Challenges & Resolutions

* **Silent Threads:** Threads initially failed to print to the console because `clone()` did not share file descriptors. **Fix:** Duplicated the open-file table via `filedup()` inside `clone()`.
* **Lost Wakeups:** xv6 wakes all sleeping processes simultaneously, causing semaphore races. **Fix:** Wrapped the sleep condition in a `while` loop to force re-evaluation upon waking.
* **Masked Races:** Single-CPU QEMU serialized operations, artificially hiding race conditions. **Fix:** Widened race windows and utilized SMP execution to expose lost updates prior to adding semaphores.
* **Unbreakable Deadlocks:** The real pipe deadlock could not unwind naturally. **Fix:** Leveraged `kill()`, as xv6's `piperead` cleanly returns -1 for killed readers, allowing children to exit.
* **Process Starvation:** Strict priority scheduling indefinitely stalled low-priority tasks. **Fix:** The 30-tick aging rule demonstrably boosts delayed processes until they are executed.