#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  char *mem;
  int phys_addr;

  printf(1, "--- Virtual to Physical Translation Test ---\n");

  // 1. Allocate memory using sbrk
  mem = sbrk(4096); // Allocate exactly one page (4 KB)
  printf(1, "Allocated 1 page starting at virtual address: %p\n", mem);

  // 2. Test the starting address of the allocated page
  phys_addr = vtop((int)mem);
  printf(1, "Virtual Address: %p -> Physical Address: 0x%x\n", mem, phys_addr);

  // 3. Test an address somewhere inside the page (e.g., offset 100)
  // The physical address should just be 100 bytes higher than the previous one!
  phys_addr = vtop((int)mem + 100);
  printf(1, "Virtual Address: %p -> Physical Address: 0x%x\n", mem + 100, phys_addr);

  // 4. Test an invalid/unmapped address
  // A simple user program never uses addresses this high (just below kernel space)
  int invalid_addr = 0x7FFFF000;
  phys_addr = vtop(invalid_addr);
  
  printf(1, "Invalid Virtual Address: 0x%x -> ", invalid_addr);
  if(phys_addr == -1) {
    printf(1, "Unmapped or Invalid (Returned -1)\n");
  } else {
    printf(1, "Physical Address: 0x%x\n", phys_addr);
  }

  exit();
}