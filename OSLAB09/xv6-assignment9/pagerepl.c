#include "types.h"
#include "user.h"

#define MAXREF 32
#define MAXFRAMES 16

static void
showframes(int *frames, int nframes)
{
  int i;
  printf(1, "[");
  for(i = 0; i < nframes; i++){
    if(i)
      printf(1, " ");
    if(frames[i] < 0)
      printf(1, "-");
    else
      printf(1, "%d", frames[i]);
  }
  printf(1, "]");
}

static int
simulate(int *refs, int nrefs, int nframes, int lru, int verbose)
{
  int frames[MAXFRAMES], used[MAXFRAMES];
  int i, j, hit, slot, faults = 0, next = 0, oldest;

  for(i = 0; i < nframes; i++){
    frames[i] = -1;
    used[i] = -1;
  }
  for(i = 0; i < nrefs; i++){
    hit = 0;
    slot = -1;
    for(j = 0; j < nframes; j++){
      if(frames[j] == refs[i]){
        hit = 1;
        slot = j;
        break;
      }
      if(frames[j] < 0 && slot < 0)
        slot = j;
    }
    if(!hit){
      faults++;
      if(lru && slot < 0){
        oldest = 0;
        for(j = 1; j < nframes; j++)
          if(used[j] < used[oldest])
            oldest = j;
        slot = oldest;
      } else if(!lru && slot < 0){
        slot = next;
        next = (next + 1) % nframes;
      }
      frames[slot] = refs[i];
    }
    used[slot] = i;
    if(verbose){
      printf(1, "%s ref %d: ", lru ? "LRU" : "FIFO", refs[i]);
      showframes(frames, nframes);
      printf(1, " %s\n", hit ? "HIT" : "FAULT");
    }
  }
  return faults;
}

int
main(int argc, char **argv)
{
  int refs[MAXREF], nrefs, nframes, i, fifo_faults, lru_faults;

  if(argc < 3){
    printf(1, "usage: pagerepl frames ref ...\n");
    exit();
  }
  nframes = atoi(argv[1]);
  nrefs = argc - 2;
  if(nframes < 1 || nframes > MAXFRAMES || nrefs > MAXREF){
    printf(1, "pagerepl: invalid input\n");
    exit();
  }
  for(i = 0; i < nrefs; i++)
    refs[i] = atoi(argv[i + 2]);

  printf(1, "Q3: detailed run with %d frames and %d references\n",
         nframes, nrefs);
  fifo_faults = simulate(refs, nrefs, nframes, 0, 1);
  lru_faults = simulate(refs, nrefs, nframes, 1, 1);
  printf(1, "FIFO faults = %d, LRU faults = %d\n",
         fifo_faults, lru_faults);

  printf(1, "frames  FIFO faults  FIFO hit%%  LRU faults  LRU hit%%\n");
  for(nframes = 3; nframes <= 5; nframes++){
    fifo_faults = simulate(refs, nrefs, nframes, 0, 0);
    lru_faults = simulate(refs, nrefs, nframes, 1, 0);
    printf(1, "%d       %d           %d       %d           %d\n",
           nframes, fifo_faults, (nrefs - fifo_faults) * 100 / nrefs,
           lru_faults, (nrefs - lru_faults) * 100 / nrefs);
  }
  exit();
}
