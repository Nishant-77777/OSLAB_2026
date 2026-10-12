# 2403MC02_asg_07_MA3104 — OS Lab Assignment 7 (xv6)

> **Course:** MA3104 (Operating Systems)
> **Student:** Nishant | 2403MC02
> **Platform:** xv6 on QEMU

## Overview

Four kernel-level experiments implemented directly in xv6:

| # | Topic | Implementation |
| --- | --- | --- |
| **Q1** | Threads | `clone()` and `join()` syscalls for shared-address-space threading. |
| **Q2** | Synchronization | Counting semaphores (`sem_init/wait/signal`) solving race conditions and producer-consumer. |
| **Q3** | Deadlock | User-space detection, Banker's avoidance algorithm, and a real pipe deadlock resolution. |
| **Q4** | Scheduling | Priority-preemptive scheduler with aging (replaced default round-robin). |

## Repository Layout

* `q1/` — Threads (`threadtest.c`, kernel diffs, logs)
* `q2/` — Semaphores (`syntest.c`, diffs, logs)
* `q3/` — Deadlock (`deadlock.c`, matrices, diffs, logs)
* `q4/` — Priority Scheduler (`priotest.c`, diffs, logs)
* `REPORT.md` — Consolidated design, results, and implementation report.

## Quick Start

Navigate to the target question folder and run:

```bash
make clean && make qemu-nox
# Run the specific test: threadtest | syntest | deadlock | priotest

```

## Results Summary

* **Q1:** 3 threads modifying a shared counter prove the shared address space works (unsynchronized).
* **Q2:** Semaphores guarantee exact counter synchronization (always hits 300,000) and prevent producer-consumer buffer faults.
* **Q3:** Accurately identifies deadlocked test matrices, denies unsafe resource requests, and successfully breaks a real A<->B circular pipe wait.
* **Q4:** Execution order strictly follows priority levels. Starvation is successfully prevented as aging dynamically boosts delayed processes.