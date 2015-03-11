#ifndef TASK_LIST_H
#define TASK_LIST_H

#include <stdint.h>

#ifdef LINUX
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#endif

// enforce that it's divisible by 2
#define NUMBER_TASKS 2

//Eventually will be pulled into a protocol file
#define OPCODE 0
#define WAIT_INDEX 1

#define WAIT_EVENT 2

struct taskSlot{
   uint8_t command[2];
   uint8_t period;
   uint8_t counter;
   uint8_t linkflag;
   uint8_t reload;
};
//param counter
//	A value of 1 means this task is ready to execute. 0 means this task is not currently active. This can occur when a task has executed but it hasn't been reloaded with the period

//param period
//	How long until the task will execute. Units are most likely going to be in milliseconds. 

//param reload
//	This field determines what happens after a task's counter has expired (ie. reached 0 after being executed). If 'reload' is 1, then the period value will be loaded into counter. Otherwise counter remains 0 and this task will not run again until 

//tie into memory map
struct taskSlot taskList[NUMBER_TASKS];
//static uint8_t taskReadyQueue[NUMBER_TASKS/2];

void timer_isr();
#endif
