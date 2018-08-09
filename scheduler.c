#include <stdlib.h>
#include <assert.h>

#include "main_header.h"

void schedule(Schedule *sch, Process *p){
  if (sch->running == NULL){
    push(sch->running, p);
    sch->pros_start = getTime(sch);
  }
  else{
    if (p!= NULL){
      insert(p, &sch->running);
    }
    next(sch);
  }
}

void update(Schedule *schedule){
  schedule->cur_time++;
  ((Process*)(schedule->running)->data)->time_remaining--;
}

void next(Schedule *schedule){
  Process *last_run = pop(&schedule->running);
  insert(last_run, &schedule->running);
}

int getTime(Schedule *schedule){
  return schedule->cur_time;
}

bool e3(Schedule *sch, Memory *mem){
  Process *cur_pros = (Process*)(sch->running)->data;
  int remain = cur_pros->time_remaining;
  if (remain <= 0){
    pop(&sch->running);
    int index = findProcess(mem, cur_pros);
    assert(index);
    delete(mem, index);
    return true;
  }
  else{
    return false;
  }
}

bool e2(Schedule *schedule, int quantum){
  if (schedule->cur_time - schedule->pros_start == quantum){
    return true;
  }
  else{
    return false;
  }
}

bool e1(Memory *mem, List *on_disk){
  if (on_disk!=NULL && mem->num_block == 0){
    return true;
  }
  else{
    return false;
  }
}
