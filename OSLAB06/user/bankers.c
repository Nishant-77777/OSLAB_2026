#include "kernel/types.h"
#include "user/user.h"

#define PROCESSES 5
#define RESOURCES 3

int allocation[PROCESSES][RESOURCES] = {
  {0, 1, 0},
  {2, 0, 0},
  {3, 0, 2},
  {2, 1, 1},
  {0, 0, 2}
};

int maximum[PROCESSES][RESOURCES] = {
  {7, 5, 3},
  {3, 2, 2},
  {9, 0, 2},
  {2, 2, 2},
  {4, 3, 3}
};

int available[RESOURCES] = {3, 3, 2};

int
is_safe(void)
{
  int work[RESOURCES];
  int finish[PROCESSES] = {0, 0, 0, 0, 0};
  int safe_sequence[PROCESSES];
  int completed = 0;

  for(int resource = 0; resource < RESOURCES; resource++)
    work[resource] = available[resource];

  while(completed < PROCESSES){
    int found = 0;
    for(int process = 0; process < PROCESSES; process++){
      int can_finish = 1;
      if(finish[process])
        continue;
      for(int resource = 0; resource < RESOURCES; resource++){
        int need = maximum[process][resource] - allocation[process][resource];
        if(need > work[resource])
          can_finish = 0;
      }
      if(can_finish){
        for(int resource = 0; resource < RESOURCES; resource++)
          work[resource] += allocation[process][resource];
        finish[process] = 1;
        safe_sequence[completed++] = process;
        found = 1;
      }
    }
    if(!found)
      return 0;
  }

  printf("Safe sequence: ");
  for(int i = 0; i < PROCESSES; i++)
    printf("P%d%s", safe_sequence[i], i == PROCESSES - 1 ? "\n" : " -> ");
  return 1;
}

int
request_resources(int process, int request[RESOURCES])
{
  for(int resource = 0; resource < RESOURCES; resource++){
    int need = maximum[process][resource] - allocation[process][resource];
    if(request[resource] > need || request[resource] > available[resource])
      return 0;
  }

  for(int resource = 0; resource < RESOURCES; resource++){
    available[resource] -= request[resource];
    allocation[process][resource] += request[resource];
  }

  if(!is_safe()){
    for(int resource = 0; resource < RESOURCES; resource++){
      available[resource] += request[resource];
      allocation[process][resource] -= request[resource];
    }
    return 0;
  }
  return 1;
}

int
main(void)
{
  int request[RESOURCES] = {1, 0, 2};

  printf("Banker's Algorithm\n");
  if(is_safe())
    printf("Initial state: SAFE\n");
  else
    printf("Initial state: UNSAFE\n");

  printf("Request from P1: (%d, %d, %d)\n",
         request[0], request[1], request[2]);
  if(request_resources(1, request))
    printf("Request granted: resulting state is SAFE\n");
  else
    printf("Request denied: resulting state would be UNSAFE\n");

  exit(0);
}
