Name:-Nishant
Roll no:-2403MC02
Set:-2

Q1==>
    (a)
    => vm.c exports getpte(), a small wrapper around xv6's static walkpgdir().
    It returns an existing page-table entry without allocating a page table.
    uint *
        getpte(pde_t *pgdir, void *va)
        {
        return walkpgdir(pgdir, va, 0);
        }

    (b)
    => sysproc.c implements protect_pages() validation and update
    logic. mprotect() clears PTE_W and munprotect() sets PTE_W for each page.
    The full range is checked before any PTE is changed, so an invalid range
    cannot leave a partial protection update.

    (c)
    => A call fails with -1 when the address is not page-aligned, len is not
    positive, the range exceeds the process size, or a page is absent.
    Successful changes reload CR3 with the current process page directory to
    flush the TLB.

    prottest obtains one page with sbrk(4096), writes A, and verifies that:

    - mprotect() still permits reading;
    - a child write causes the kernel to kill the child;
    - the parent still reads A;
    - munprotect() permits writing C;
    - unaligned, zero-length, and out-of-range mprotect() calls return -1.

Q2==>
    Tasks 
    (a) => added int field named priority inside proc struct
    struct proc {
    ....
    struct trapframe *tf;        // Trap frame for current syscall
    struct context *context;     // swtch() here to run process
    int priority;                // Process priority....

    => initialized each process with priority 10 in allocproc
        static struct proc*
        allocproc(void)
        {
        struct proc *p;
        char *sp;

        acquire(&ptable.lock);

        for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
            if(p->state == UNUSED)
            goto found;

        release(&ptable.lock);
        return 0;

        found:
        p->state = EMBRYO;
        p->pid = nextpid++;
        p->priority = 10;....

    (b) => implemented syscall inside sysproc accuring lock and setting priority 
        int
        sys_set_priority(void)
        {
        int pid;
        int priority;

        if(argint(0, &pid) < 0 || argint(1, &priority) < 0)
            return -1;
        return set_priority(pid, priority);
        }
        =======
        int
        set_priority(int pid, int priority)
        {
        struct proc *p;
        int oldpriority;

        if(priority < 1 || priority > 20)
            return -1;

        acquire(&ptable.lock);
        for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
            if(p->pid == pid && p->state != UNUSED){
            oldpriority = p->priority;
            p->priority = priority;
            release(&ptable.lock);
            return oldpriority;
            }
        }
        release(&ptable.lock);
        return -1;
        }

    (c)=> now the scheduler first serches for highest priority process to execute 
            // Choose the runnable process with the highest priority.
            acquire(&ptable.lock);
            chosen = 0;
            for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
            if(p->state != RUNNABLE)
                continue;
            if(chosen == 0 || p->priority < chosen->priority)
                chosen = p;
            }

            if(chosen != 0){
            // Switch to chosen process.  It is the process's job
            // to release ptable.lock and then reacquire it
            // before jumping back to us.
            c->proc = chosen;
            switchuvm(chosen);
            chosen->state = RUNNING;

            swtch(&(c->scheduler), chosen->context);
            switchkvm();

            // Process is done running for now.
            // It should have changed its p->state before coming back.
            c->proc = 0;
            }
            release(&ptable.lock);

        }
        }

    (d) => implemented exactly as told in Question created 3 child process with priority 15 , 10 and 5 respctively and in output we can see the exectution completed with
        $ schedtest
        Child priority 5 finished
        Child priority 10 finished
        Child priority 15 finished
        Done


