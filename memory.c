#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "main_header.h"

void swap(Memory *mem, Process *p, Schedule *schedule, List *on_disk, Block* (*find_fit)(List freeList, Process *p)){
  findFree(mem);

  if (p!= NULL){
    Block* free_block = find_fit(mem->free, p);

    while (free_block!=NULL) {
      swapOut(mem, on_disk, schedule);
      findFree(mem);
      Block* free_block = find_fit(mem->free, p);
    }

    putBlock(mem, free_block, p);
  }
}

void swapOut(Memory *mem, List *on_disk, Schedule *schedule){
  Block out = toSwapOut(mem, schedule);

  insert(&out, on_disk);
}

Block toSwapOut(Memory *mem, Schedule *schedule){
  int cur_time = schedule->cur_time;
  int i;

  int *time_on_mem = NULL;
  for(i = 0; i < mem->num_block; i++){
    time_on_mem[i] = cur_time - (mem->on_memory+i)->time_on;
  }

  int longestStay = max(time_on_mem, mem->num_block);
  Block out = mem->on_memory[longestStay];

  delete(mem, longestStay);

  return out;
}

int findProcess(Memory *mem, Process *p){
    int i;
    int pid = p->process_id;

    for(i=0; i< mem->num_block; i++){
        int cur_pid = ((mem->on_memory+i)->process)->process_id;

        if (cur_pid == pid){
            return i;
        }
    }

    return NULL;
}

void delete(Memory *mem, int index){

  Block* first_half = (Block*) malloc(sizeof(Block)*(index));
  Block* second_half = (Block*) malloc(sizeof(Block)*(mem->num_block-index));
  int out_size = (mem->on_memory+index)->size;

  memcpy(first_half, mem->on_memory, index);
  memcpy(second_half, mem->on_memory+index+1, mem->num_block-index-1);

  mem->num_block--;

  mem->on_memory = realloc(mem->on_memory, mem->num_block);

  mem->on_memory = first_half;
  memcpy((mem->on_memory)+index, second_half, mem->num_block-index-1);

  mem->used = mem->used - out_size;
}

void findFree(Memory *mem){
  Block* running = mem->on_memory;
  int num_block = mem->num_block;
  List freeList = NULL;
  
  if (num_block > 0){
    int i;
    for (i = 0; i < num_block; i++) {

        int pros_start = (running+i)->start_index;
        int pros_size = (running+i)->size;
        int pros_end = pros_start + pros_size;
        int free_size;

        if (i+1 == num_block){
          free_size = mem->capacity - pros_size;
        }
        else{
          int nextpros_start = (running+i+1)->start_index;
          free_size = nextpros_start - pros_end;
        }

        if (free_size > 0){
          Block *free_block = malloc(sizeof(Block)*1);
          free_block->start_index = pros_end;
          free_block->size = free_size;
          free_block->process = NULL;
          freeList = push(freeList, free_block);
        }
    }
    mem->free = freeList;
  }
  else{
    Block *free_block = malloc(sizeof(Block)*1);
    free_block->start_index = 0;
    free_block->size = mem->capacity;
    free_block->process = NULL;
    freeList = push(freeList, free_block);
    mem->free = freeList;
  }

}

void putBlock(Memory *mem, Block *free_block, Process *p){
  int free_end_index = free_block->size + free_block->start_index;

  int p_size = p->memory;

  Block* to_put = malloc(sizeof(Block)*1);
  assert(to_put);
  to_put->size = p_size;
  to_put->start_index = free_end_index-p_size;
  to_put->process = p;

  rearrange(mem, to_put);
}

void rearrange(Memory *mem, Block* to_put){
  int index;
  int to_put_start = to_put->start_index;

  if (mem->on_memory != NULL){
    for(index=0; index < mem->num_block; index++){
      int cur_start_index = (mem->on_memory+index)->start_index;

      // if to_put block should come before current block
      if (cur_start_index > to_put_start){
        break;
      }
    }

  add(mem, to_put, index);
  }
  else{
    mem->on_memory = to_put;
    mem->num_block++;
  }

}

void add(Memory *mem, Block*  to_put, int index){
  Block* first_half = (Block*) malloc(sizeof(Block)*(index));
  Block* second_half = (Block*) malloc(sizeof(Block)*(mem->num_block-index));

  memcpy(first_half, mem->on_memory, index);
  memcpy(second_half, mem->on_memory+index, mem->num_block-index);

  mem->num_block++;

  mem->on_memory = realloc(mem->on_memory, mem->num_block);

  mem->on_memory = first_half;
  memcpy((mem->on_memory)+index, to_put, 1);
  memcpy((mem->on_memory)+index+1, second_half, mem->num_block-index-1);
}

Block* fit_holes(List freeList, Process *p){
  int needed = p->memory;
  Block *fit = NULL;
  int count = 0;
  while (freeList->next != NULL) {
    Block *cur_block = freeList->data;
    if (cur_block->size >= needed){
      memcpy(fit+count, cur_block, 1);
      count++;
    }
    freeList = freeList->next;
  }

  return fit;
}

Block* findFirst(List freeList, Process *p){

  return fit_holes(freeList, p);
}

Block* findBest(List freeList, Process *p){
  Block* fit = fit_holes(freeList, p);
  if (fit!=NULL){
    qsort(fit, sizeof(fit)/sizeof(Block), sizeof(Block), compare_size);
  }
  return fit;
}

Block* findWorst(List freeList, Process *p){
  Block* fit = fit_holes(freeList, p);
  if (fit!=NULL){
    qsort(fit, sizeof(fit)/sizeof(Block), sizeof(Block), compare_size_desc);
  }

  return fit;
}

int compare_size (const void *a, const void *b) {
  const int *s1 = (const int *) a;
  const int *s2 = (const int *) b;

  return (*s1 > *s2) - (*s1 < *s2);
}

int compare_size_desc (const void *a, const void *b) {
  const int *s1 = (const int *) a;
  const int *s2 = (const int *) b;

  return (*s1 < *s2) - (*s1 > *s2);
}

int max(int* arr, int size){
  int i, max, max_index;
  max = arr[0];
  max_index = 0;

  for(i=0; i < size; i++){
    if (arr[i] > max){
      max_index = i;
      max = arr[i];
    }
  }

  return max_index;
}
