#ifndef TASK_LIST_H
#define TASK_LIST_H

#include <stdint.h>
struct taskSlot{
   uint8_t command[4]; //opcode, dest, misc
   uint8_t period; //resolution might be determined by RDC period
   uint8_t counter;
   uint8_t linkflag; 
   uint8_t reload;
};

uint8_t print_task(struct taskSlot* s);
#endif
