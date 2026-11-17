#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_set_priority(void)
{
  int pid;
  int priority;

  if(argint(0, &pid) < 0 || argint(1, &priority) < 0)
    return -1;
  return set_priority(pid, priority);
}

// Change write permission for len whole pages beginning at addr.
static int
protect_pages(uint addr, int len, int writable)
{
  struct proc *curproc = myproc();
  pte_t *pte;
  uint i;

  if(addr % PGSIZE != 0 || len <= 0)
    return -1;
  if(addr > curproc->sz || (uint)len > (curproc->sz - addr) / PGSIZE)
    return -1;

  // Validate the complete range before changing any PTE.
  for(i = 0; i < (uint)len; i++){
    pte = getpte(curproc->pgdir, (void*)(addr + i * PGSIZE));
    if(pte == 0 || (*pte & PTE_P) == 0)
      return -1;
  }

  for(i = 0; i < (uint)len; i++){
    pte = getpte(curproc->pgdir, (void*)(addr + i * PGSIZE));
    if(writable)
      *pte |= PTE_W;
    else
      *pte &= ~PTE_W;
  }
  lcr3(V2P(curproc->pgdir));
  return 0;
}

int
sys_mprotect(void)
{
  int addr;
  int len;

  if(argint(0, &addr) < 0 || argint(1, &len) < 0)
    return -1;
  return protect_pages((uint)addr, len, 0);
}

int
sys_munprotect(void)
{
  int addr;
  int len;

  if(argint(0, &addr) < 0 || argint(1, &len) < 0)
    return -1;
  return protect_pages((uint)addr, len, 1);
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
