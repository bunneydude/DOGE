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
#define NUMBER_TASKS 1

//Eventually will be pulled into a protocol file
#define OPCODE 0
#define WAIT_INDEX 1

#define WAIT_EVENT 2

struct taskSlot{
   uint8_t counter;
};
/*
struct taskSlot{
   uint8_t command[1]; //opcode, dest, misc
   uint8_t period; //resolution might be determined by RDC period
   uint8_t counter;
   uint8_t linkflag; 
   uint8_t reload;
};*/
//tie into memory map
volatile static struct taskSlot taskList;
//static uint8_t taskReadyQueue[NUMBER_TASKS/2];


uint8_t print_task(struct taskSlot* s);
void timer_isr();
#endif
