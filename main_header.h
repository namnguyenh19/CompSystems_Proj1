#include <stdio.h>
#include <stdlib.h>

#include "list.h"

typedef struct{
	int time_created;
	int process_id;
	int memory;
	int job_time;
	int time_remaining;
} Process;

typedef struct {
  int start_index;
  int size;
  Process *process;
  int time_on;
} Block;

typedef struct {
  int used;
  int capacity;
  List free;
  int num_block;
  Block *on_memory;
} Memory;

typedef struct {
  List running;
  int pros_start;
  int cur_time;
} Schedule;

void schedule(Schedule *sch, Process *p);

void update(Schedule *schedule);

void next(Schedule *schedule);

int getTime(Schedule *schedule);

bool e3(Schedule *sch, Memory *mem);

bool e2(Schedule *schedule, int quantum);

bool e1(Memory *mem, List *on_disk);

void swap(Memory *mem, Process *p, Schedule *schedule, List *on_disk, Block* (*find_fit)(List freeList, Process *p));

void swapOut(Memory *mem, List *on_disk, Schedule *schedule);

Block toSwapOut(Memory *mem, Schedule *schedule);

void delete(Memory *mem, int index);

void findFree(Memory *mem);

int findProcess(Memory *mem, Process *p);

void putBlock(Memory *mem, Block *free_block, Process *p);

void rearrange(Memory *mem, Block *to_put);

void add(Memory *mem, Block *to_put, int index);

Block* fit_holes(List freeList, Process *p);

Block* findFirst(List freeList, Process *p);

Block* findBest(List freeList, Process *p);

Block* findWorst(List freeList, Process *p);

int compare_size (const void *a, const void *b);

int compare_size_desc (const void *a, const void *b);

int max(int* arr, int size);
