#include "types.h"
#include "user.h"

#define PGSIZE 4096

int
main(void)
{
  int i;
  char *base;

  printf(1, "Q1: initial page table\n");
  vmprint();
  base = sbrk(4 * PGSIZE);
  if(base == (char*)-1){
    printf(1, "pttest: sbrk failed\n");
    exit();
  }
  // The integrated submission uses lazy allocation, so touch each page first.
  for(i = 0; i < 4; i++)
    base[i * PGSIZE] = (char)i;
  printf(1, "Q1: after sbrk(4 * 4096) and four touches\n");
  vmprint();
  exit();
}
