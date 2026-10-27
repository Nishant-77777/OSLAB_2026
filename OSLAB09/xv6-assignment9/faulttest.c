#include "types.h"
#include "user.h"

#define PGSIZE 4096

int
main(void)
{
  int i, child;
  char *base;
  volatile char *bad;
  int before;

  base = sbrk(32 * PGSIZE);
  if(base == (char*)-1){
    printf(1, "faulttest: sbrk failed\n");
    exit();
  }
  before = getfaults();
  for(i = 0; i < 5; i++)
    base[i * 3 * PGSIZE] = (char)i;
  printf(1, "Q2: handled faults = %d (expected 5)\n",
         getfaults() - before);

  child = fork();
  if(child == 0){
    bad = base + 32 * PGSIZE;
    *bad = 1;
    printf(1, "faulttest: invalid access unexpectedly survived\n");
    exit();
  }
  if(child > 0){
    wait();
    printf(1, "Q2: parent survived child invalid-access test\n");
  } else {
    printf(1, "faulttest: fork failed\n");
  }
  exit();
}
