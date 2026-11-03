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


int sys_memstats(void) {
  extern uint total_allocated_pages;
  extern uint total_freed_pages;
  extern uint available_pages;

  cprintf("\n--- Memory Statistics ---\n");
  cprintf("Total allocated pages: %d\n", total_allocated_pages);
  cprintf("Total freed pages: %d\n", total_freed_pages);
  
  // The kernel starts with a fixed amount of memory. 
  // 'Available pages' goes up on free and down on alloc. 
  // We can calculate currently active allocations by looking at what's missing from the free pool.
  // (Assuming roughly 57,344 total pages in xv6's 224MB default RAM limit)
  cprintf("Currently allocated pages: %d\n", 57344 - available_pages);
  
  cprintf("Available physical memory pages: %d\n", available_pages);
  cprintf("-------------------------\n");
  
  return 0;
}

int sys_vtop(void) {
  int virtual_address;
  
  // argint(0, ...) fetches the very first argument passed to the system call
  // If it fails to read it (returns less than 0), we return an error (-1)
  if(argint(0, &virtual_address) < 0) {
    return -1;
  }
  
  // Now we pass the fetched address to the real vtop function we wrote in vm.c
  return vtop(virtual_address);
}

int sys_pageinfo(void) {
  int virtual_address;
  
  if(argint(0, &virtual_address) < 0) {
    return -1;
  }
  
  return pageinfo(virtual_address);
}

int sys_setreadonly(void) {
  int virtual_address;
  
  if(argint(0, &virtual_address) < 0) {
    return -1;
  }
  
  return setreadonly(virtual_address);
}