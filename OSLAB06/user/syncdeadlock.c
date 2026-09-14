#include "kernel/types.h"
#include "user/user.h"

#define PROCESS_COUNT 5
#define RESOURCE_COUNT 3
#define CYCLES 3

int
main(void)
{
  int needs[PROCESS_COUNT][2] = {{0, 1}, {1, 2}, {0, 2}, {0, 1}, {1, 2}};
  int pid;

  for(int resource = 0; resource < RESOURCE_COUNT; resource++)
    sem_init(resource, resource == 1 ? 1 : 2);

  for(int process = 0; process < PROCESS_COUNT; process++){
    pid = fork();
    if(pid < 0){
      printf("fork failed\n");
      exit(1);
    }
    if(pid == 0){
      int first = needs[process][0];
      int second = needs[process][1];
      for(int cycle = 0; cycle < CYCLES; cycle++){
        printf("P%d requests R%d\n", process, first);
        sem_wait(first);
        printf("P%d granted R%d and requests R%d\n", process, first, second);
        sem_wait(second);
        printf("P%d starts work with R%d and R%d\n", process, first, second);
        pause(2);
        sem_signal(second);
        sem_signal(first);
        printf("P%d releases R%d and R%d\n", process, first, second);
      }
      exit(0);
    }
  }

  for(int process = 0; process < PROCESS_COUNT; process++)
    wait(0);
  printf("All 5 processes completed all work cycles\n");
  printf("Strategy: every process acquires resources in increasing order\n");
  exit(0);
}
