#include "types.h"
#include "stat.h"
#include "user.h"
#include "meminfo.h"

#define LAZY_BYTES (32 * 1024 * 1024)
#define MIDPOINT (16 * 1024 * 1024)
#define UNTOUCHED_PAGE (4096 * 100)

static void
show(char *tag)
{
  struct meminfo mi;
  getmeminfo(&mi);
  printf(1, "[%s] free=%d user_pages=%d pt=%d\n",
         tag, mi.free_pages, mi.user_pages, mi.pt_pages);
}

int
main(void)
{
  char *big;
  int pid;

  show("before 32MB sbrk");
  big = sbrk(LAZY_BYTES);
  if(big == (char*)-1){ printf(1, "sbrk failed\n"); exit(); }
  show("after 32MB sbrk (free ~unchanged = lazy!)");

  big[0] = 'A';
  big[MIDPOINT] = 'B';
  big[LAZY_BYTES - 1] = 'C';
  show("after touching 3 pages (free fell ~3)");
  printf(1, "read back: %c %c %c\n", big[0], big[MIDPOINT], big[LAZY_BYTES - 1]);

  write(1, big + UNTOUCHED_PAGE, 8);   /* kernel copyin faults in untouched page */
  printf(1, "<- 8 zero bytes via write() on untouched buffer (ok)\n");

  pid = fork();
  if(pid == 0){
    printf(1, "child sees touched value: %c (copyuvm skipped absent PTEs)\n",
           big[MIDPOINT]);
    exit();
  }
  wait();
  show("after fork+reap");

  pid = fork();
  if(pid == 0){
    printf(1, "child touching va == sz (invalid)...\n");
    big[LAZY_BYTES] = 'X';
    printf(1, "SHOULD NEVER PRINT\n");
    exit();
  }
  wait();
  printf(1, "parent + kernel alive after child was killed for bad access\n");
  printf(1, "lazytest done\n");
  exit();
}
