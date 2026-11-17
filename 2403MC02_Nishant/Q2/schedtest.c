#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int pid;
  int i;
  int priorities[3] = {5, 10, 15};

  set_priority(getpid(), 1);

  for(i = 0; i < 3; i++){
    pid = fork();
    if(pid < 0){
      printf(1, "schedtest: fork failed\n");
      exit();
    }
    if(pid == 0){
      long count;

      for(count = 0; count < 50000000L; count++)
        ;
      printf(1, "Child priority %d finished\n", priorities[i]);
      exit();
    }
    set_priority(pid, priorities[i]);
  }

  for(i = 0; i < 3; i++)
    wait();
  printf(1, "Done\n");
  exit();
}
