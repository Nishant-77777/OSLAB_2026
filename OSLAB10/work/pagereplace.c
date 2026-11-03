#include "types.h"
#include "stat.h"
#include "user.h"

int ref[20] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
int ref_len = 20;

// Helper function to print the current state of the frames
void print_frames(int *frames, int f) {
  printf(1, "[");
  for(int i = 0; i < f; i++) {
    if(frames[i] == -1) printf(1, " - ");
    else printf(1, " %d ", frames[i]);
  }
  printf(1, "] ");
}

void run_fifo(int f, int verbose, int *faults_out, int *hits_out) {
  int frames[10];
  for(int i=0; i<f; i++) frames[i] = -1;
  int faults = 0, hits = 0, replace_idx = 0;

  if(verbose) printf(1, "\n--- FIFO Page Replacement (%d frames) ---\n", f);
  for(int i = 0; i < ref_len; i++) {
    int page = ref[i];
    int hit = 0;
    
    for(int j = 0; j < f; j++) {
      if(frames[j] == page) { hit = 1; break; }
    }
    
    if(hit) {
      hits++;
      if(verbose) { printf(1, "Ref %d: ", page); print_frames(frames, f); printf(1, "-> Hit\n"); }
    } else {
      faults++;
      frames[replace_idx] = page;
      replace_idx = (replace_idx + 1) % f;
      if(verbose) { printf(1, "Ref %d: ", page); print_frames(frames, f); printf(1, "-> Fault\n"); }
    }
  }
  *faults_out = faults;
  *hits_out = hits;
  if(verbose) printf(1, "FIFO Total - Faults: %d, Hits: %d\n", faults, hits);
}

void run_lru(int f, int verbose, int *faults_out, int *hits_out) {
  int frames[10];
  int last_used[10]; // Tracks the "time" each frame was last used
  for(int i=0; i<f; i++) { frames[i] = -1; last_used[i] = 0; }
  int faults = 0, hits = 0, time = 0;

  if(verbose) printf(1, "\n--- LRU Page Replacement (%d frames) ---\n", f);
  for(int i = 0; i < ref_len; i++) {
    time++;
    int page = ref[i];
    int hit = 0;
    
    for(int j = 0; j < f; j++) {
      if(frames[j] == page) { 
        hit = 1; 
        last_used[j] = time; // Update the timestamp on hit
        break; 
      }
    }
    
    if(hit) {
      hits++;
      if(verbose) { printf(1, "Ref %d: ", page); print_frames(frames, f); printf(1, "-> Hit\n"); }
    } else {
      faults++;
      // Find the least recently used frame (smallest timestamp)
      int lru_idx = 0;
      for(int j = 1; j < f; j++) {
        if(frames[j] == -1) { lru_idx = j; break; } // Fill empty slots first
        if(last_used[j] < last_used[lru_idx]) lru_idx = j;
      }
      frames[lru_idx] = page;
      last_used[lru_idx] = time;
      if(verbose) { printf(1, "Ref %d: ", page); print_frames(frames, f); printf(1, "-> Fault\n"); }
    }
  }
  *faults_out = faults;
  *hits_out = hits;
  if(verbose) printf(1, "LRU Total - Faults: %d, Hits: %d\n", faults, hits);
}

void run_optimal(int f, int verbose, int *faults_out, int *hits_out) {
  int frames[10];
  for(int i=0; i<f; i++) frames[i] = -1;
  int faults = 0, hits = 0;

  if(verbose) printf(1, "\n--- Optimal Page Replacement (%d frames) ---\n", f);
  for(int i = 0; i < ref_len; i++) {
    int page = ref[i];
    int hit = 0;
    
    for(int j = 0; j < f; j++) {
      if(frames[j] == page) { hit = 1; break; }
    }
    
    if(hit) {
      hits++;
      if(verbose) { printf(1, "Ref %d: ", page); print_frames(frames, f); printf(1, "-> Hit\n"); }
    } else {
      faults++;
      int replace_idx = -1;
      
      // Look for an empty frame first
      for(int j = 0; j < f; j++) {
        if(frames[j] == -1) { replace_idx = j; break; }
      }
      
      // If full, look into the future to see which frame is needed LAST
      if(replace_idx == -1) {
        int farthest_use = -1;
        for(int j = 0; j < f; j++) {
          int next_use = 999; // Default to a huge number if never used again
          for(int k = i + 1; k < ref_len; k++) {
            if(ref[k] == frames[j]) { next_use = k; break; }
          }
          if(next_use > farthest_use) {
            farthest_use = next_use;
            replace_idx = j;
          }
        }
      }
      
      frames[replace_idx] = page;
      if(verbose) { printf(1, "Ref %d: ", page); print_frames(frames, f); printf(1, "-> Fault\n"); }
    }
  }
  *faults_out = faults;
  *hits_out = hits;
  if(verbose) printf(1, "Optimal Total - Faults: %d, Hits: %d\n", faults, hits);
}

int main(int argc, char *argv[]) {
  int frames = 3; // Default to 3 frames as requested
  int faults, hits;

  // If the user types a number after the command (e.g., "pagereplace 4"), use that instead
  if(argc > 1) {
    frames = atoi(argv[1]);
  }

    printf(1, "Reference string: 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1\n");
    printf(1, "Running detailed simulation with %d frames\n", frames);
    run_fifo(frames, 1, &faults, &hits);
    run_lru(frames, 1, &faults, &hits);
    run_optimal(frames, 1, &faults, &hits);

    printf(1, "\n--- Comparison for 3, 4 and 5 frames ---\n");
    printf(1, "Algorithm Frames Faults Hits Hit%% Fault%%\n");
    for(frames = 3; frames <= 5; frames++) {
      run_fifo(frames, 0, &faults, &hits);
      printf(1, "FIFO     %d      %d      %d    %d    %d\n", frames, faults, hits,
        (hits * 100) / ref_len, (faults * 100) / ref_len);
      run_lru(frames, 0, &faults, &hits);
      printf(1, "LRU      %d      %d      %d    %d    %d\n", frames, faults, hits,
        (hits * 100) / ref_len, (faults * 100) / ref_len);
      run_optimal(frames, 0, &faults, &hits);
      printf(1, "Optimal  %d      %d      %d    %d    %d\n", frames, faults, hits,
        (hits * 100) / ref_len, (faults * 100) / ref_len);
    }

  exit();
}