#include "types.h"
#include "stat.h"
#include "user.h"

int shared_value = 42;

int
main(void)
{
  int stack_value = 7;
  void *heap = malloc(64);
  int pid;

  printf(1, "== parent pid %d ==\n", getpid());
  printf(1, "global: va=%x pa=%x\n", (int)&shared_value, va2pa((int)&shared_value));
  printf(1, "stack : va=%x pa=%x\n", (int)&stack_value, va2pa((int)&stack_value));
  printf(1, "heap  : va=%x pa=%x\n", (int)heap, va2pa((int)heap));
  printf(1, "unmapped 0x7fffffff -> %d (expect -1)\n", va2pa(0x7FFFFFFF));

  pid = fork();
  if(pid == 0){
    printf(1, "== child pid %d ==\n", getpid());
    printf(1, "child  global: va=%x pa=%x\n", (int)&shared_value, va2pa((int)&shared_value));
    exit();
  }
  printf(1, "parent global: va=%x pa=%x\n", (int)&shared_value, va2pa((int)&shared_value));
  wait();
  printf(1, "same VA but different PA in parent/child => fork copies pages\n");
  exit();
}
