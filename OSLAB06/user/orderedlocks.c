#include "kernel/types.h"
#include "user/user.h"

#define RESOURCE_COUNT 3
#define CHILDREN 5
#define CYCLES 3

int
main(void)
{
  int pid;
  int child;
  int first;
  int second;
  int pairs[CHILDREN][2] = {{0, 1}, {1, 2}, {0, 2}, {0, 1}, {1, 2}};

  for(int resource = 0; resource < RESOURCE_COUNT; resource++)
    sem_init(resource, 1);

  for(child = 0; child < CHILDREN; child++){
    pid = fork();
    if(pid < 0){
      printf("fork failed\n");
      exit(1);
    }
    if(pid == 0){
      first = pairs[child][0];
      second = pairs[child][1];
      if(first > second){
        int temp = first;
        first = second;
        second = temp;
      }
      for(int cycle = 0; cycle < CYCLES; cycle++){
        printf("P%d requests R%d then R%d\n", child, first, second);
        sem_wait(first);
        sem_wait(second);
        printf("P%d is working\n", child);
        pause(2);
        sem_signal(second);
        sem_signal(first);
        printf("P%d released resources\n", child);
      }
      exit(0);
    }
  }

  for(int i = 0; i < CHILDREN; i++)
    wait(0);
  printf("All ordered-lock processes completed without deadlock\n");
  exit(0);
}
