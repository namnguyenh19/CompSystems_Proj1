#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "memory.h"
#include "process.h"

typedef struct {
  List *running;
  int pros_start;
  int cur_time;
} Schedule;

void schedule(Schedule *sch, Memory *mem, Process *p, int quantum);

void update(Schedule *schedule);

void next(Schedule *schedule);

int getTime(Schedule *schedule);

bool e3(Schedule *sch);

bool e2(Schedule *schedule, int quantum);

bool e1(Memory *mem, List *on_disk);
