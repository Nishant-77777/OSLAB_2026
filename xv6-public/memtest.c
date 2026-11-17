#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  printf(1, "Initial memory state:\n");
  memstats();

  printf(1, "\nAllocating 5 pages (5 * 4096 bytes)...\n");
  // sbrk increases or decreases the memory allocated to the process
  sbrk(5 * 4096); 
  
  printf(1, "Memory state after allocation:\n");
  memstats();

  printf(1, "\nReleasing 5 pages...\n");
  // Calling sbrk with a negative number releases the memory
  sbrk(-5 * 4096); 
  
  printf(1, "Memory state after release:\n");
  memstats();

  exit();
}