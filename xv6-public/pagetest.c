#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  char *mem;

  printf(1, "--- Page Protection Test ---\n");

  // 1. Allocate a page
  mem = sbrk(4096);
  printf(1, "Allocated 1 page at virtual address: %p\n", mem);

  // 2. Display the initial page table information (Writable should be 1)
  printf(1, "\nPage info before protecting:\n");
  pageinfo((int)mem);

  // 3. Change the page to read-only
  printf(1, "\nSetting page to read-only...\n");
  setreadonly((int)mem);

  // 4. Display the page information again to prove Writable is now 0
  printf(1, "\nPage info after protecting:\n");
  pageinfo((int)mem);

  // 5. Attempt an illegal write operation
  printf(1, "\nAttempting to write to protected page (this should crash)...\n");
  
  // This next line tries to put the letter 'X' into the memory we just locked.
  mem[0] = 'X'; 

  // The CPU hardware will physically stop the program before this line ever runs.
  printf(1, "If you see this, the protection failed!\n");

  exit();
}