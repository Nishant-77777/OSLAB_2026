#include "kernel/types.h"
#include "user/user.h"

#define PROCESSES 5
#define RESOURCES 3

int allocation[PROCESSES][RESOURCES] = {
  {1, 0, 0},
  {0, 1, 0},
  {0, 0, 1},
  {0, 0, 0},
  {0, 0, 0}
};

int request[PROCESSES][RESOURCES] = {
  {0, 1, 0},
  {0, 0, 1},
  {1, 0, 0},
  {1, 0, 0},
  {0, 1, 0}
};

int available[RESOURCES] = {0, 0, 0};

void
build_wait_graph(int graph[PROCESSES][PROCESSES])
{
  for(int from = 0; from < PROCESSES; from++)
    for(int to = 0; to < PROCESSES; to++)
      graph[from][to] = 0;

  for(int process = 0; process < PROCESSES; process++)
    for(int resource = 0; resource < RESOURCES; resource++)
      if(request[process][resource] > available[resource])
        for(int holder = 0; holder < PROCESSES; holder++)
          if(allocation[holder][resource] > 0)
            graph[process][holder] = 1;
}

int
visit(int process, int graph[PROCESSES][PROCESSES], int state[PROCESSES], int path[PROCESSES], int depth)
{
  state[process] = 1;
  path[depth] = process;

  for(int next = 0; next < PROCESSES; next++){
    if(!graph[process][next])
      continue;
    if(state[next] == 1){
      printf("Deadlock cycle: ");
      int i = depth;
      while(i >= 0 && path[i] != next)
        i--;
      for(; i <= depth; i++)
        printf("P%d -> ", path[i]);
      printf("P%d\n", next);
      return 1;
    }
    if(state[next] == 0 && visit(next, graph, state, path, depth + 1))
      return 1;
  }

  state[process] = 2;
  return 0;
}

int
has_cycle(int graph[PROCESSES][PROCESSES])
{
  int state[PROCESSES] = {0, 0, 0, 0, 0};
  int path[PROCESSES];
  for(int process = 0; process < PROCESSES; process++)
    if(state[process] == 0 && visit(process, graph, state, path, 0))
      return 1;
  return 0;
}

int
main(void)
{
  int graph[PROCESSES][PROCESSES];
  build_wait_graph(graph);

  printf("Deadlock detection using a wait-for graph\n");
  printf("Edges: ");
  for(int from = 0; from < PROCESSES; from++)
    for(int to = 0; to < PROCESSES; to++)
      if(graph[from][to])
        printf("P%d -> P%d  ", from, to);
  printf("\n");

  if(has_cycle(graph))
    printf("Deadlock detected\n");
  else
    printf("No deadlock detected\n");

  for(int from = 0; from < PROCESSES; from++)
    for(int to = 0; to < PROCESSES; to++)
      graph[from][to] = 0;
  graph[0][1] = 1;
  graph[1][2] = 1;

  printf("Acyclic scenario edges: P0 -> P1  P1 -> P2\n");
  if(has_cycle(graph))
    printf("Deadlock detected\n");
  else
    printf("No deadlock detected\n");
  exit(0);
}
