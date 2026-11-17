#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  char *page;
  int pid;

  page = sbrk(4096);
  if(page == (char*)-1){
    printf(1, "prottest: sbrk failed\n");
    exit();
  }
  *page = 'A';

  if(mprotect(page, 1) < 0){
    printf(1, "prottest: mprotect failed\n");
    exit();
  }
  printf(1, "Read after mprotect: %c\n", *page);

  pid = fork();
  if(pid < 0){
    printf(1, "prottest: fork failed\n");
    exit();
  }
  if(pid == 0){
    *page = 'B';
    printf(1, "prottest: child write unexpectedly succeeded\n");
    exit();
  }
  wait();
  printf(1, "Parent still reads: %c\n", *page);

  if(munprotect(page, 1) < 0){
    printf(1, "prottest: munprotect failed\n");
    exit();
  }
  *page = 'C';
  printf(1, "Write after munprotect: %c\n", *page);

  printf(1, "Unaligned mprotect: %d\n", mprotect(page + 1, 1));
  printf(1, "Zero-length mprotect: %d\n", mprotect(page, 0));
  printf(1, "Out-of-range mprotect: %d\n", mprotect(page, 2));
  exit();
}
