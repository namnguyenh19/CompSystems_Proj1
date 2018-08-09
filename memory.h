#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "list.h"
#include "scheduler.h"

typedef struct {
  int start_index;
  int size;
  Process *process;
  int time_on;
} Block;

typedef struct {
  int used;
  int capacity;
  List *free;
  int num_block;
  Block *on_memory;
} Memory;

void swap(Memory *mem, Process *p, Schedule *schedule, List *on_disk, Block* (*find_fit)(List freeList, Process *p));

void swapOut(Memory *mem, List *on_disk, Schedule *schedule);

Block toSwapOut(Memory *mem, Schedule *schedule);

void remove(Memory *mem, int index);

void findFree(Memory *mem);

void putBlock(Memory *mem, Block *free_block, Process *p);

void rearrange(Memory *mem, Block to_put);

void add(Memory *mem, Block to_put, int index);

Block* fit_holes(List *freeList, Process *p);

Block* findFirst(List *freeList, Process *p);

Block* findBest(List *freeList, Process *p);

Block* findWorst(List *freeList, Process *p);

int compare_size (const void *a, const void *b);

int compare_size_desc (const void *a, const void *b);

int max(int* arr, int size);
