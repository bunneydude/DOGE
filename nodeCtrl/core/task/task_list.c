#include "task_list.h"
uint8_t print_task(struct taskSlot* s)
{
	return s->counter + 1;
}

/*

#define NUMBER_TASKS 8;
volatile static struct taskSlot taskList[NUMBER_TASKS];

// Invalidate all entries (linkFlag = period = 0)
init()
{
	uint8_t i = 0;
	for(i=0; i<NUMBER_TASKS; i++){
		taskList[i].linkFlag = 0;
		taskList[i].counter = 0;
	}
}
timer_isr()
{
   uint8_t i = 0;
   for(i=0; i<NUMBER_TASKS; i++){
      if(taskList[i].counter > 1){taskList[i].counter--;}
   }
}*/
/*
void reset_task(uint8_t initialIndex)
{
   uint8_t i = initialIndex;
   do{
      taskList[i].counter = taskList[i].period;
      i--;
   }while( (i < initialIndex) && (taskList[i].linkFlag == 1));
}

uint8_t get_next_task_group(uint8_t index)
{
   do{
      if(taskList[index].linkFlag == 0){
         return (index+1) % NUMBER_TASKS; //could still return 0, but that's fine
      }else{
         index++;
      }
   }while(index != NUMBER_TASKS);

   return 0;
}

static uint8_t taskReadyQueue[NUMBER_TASKS/2];
static uint8_t readyQueuePtr = 0;

uint8_t run_active_tasks()
{ //currently assumes no 'wait' events
   uint8_t i = 0;
   uint8_t linked = 0;
   uint8_t next = 0;

   //disable sw timer
   do{
      i = next;

      if( (taskList[i].counter == 1) || (linked == 1)){         
         taskList[i].counter = 0; //disable this task until it's reset

         if(taskList[i].command[OPCODE] == WAIT_EVENT){
            //determine start of next group
            next = get_next_task_group(i); //returns 0 if there are no more groups
            taskList[i].command[WAIT_INDEX] = i; //when this WAIT event is complete, it will write this value into the ready queue
            //store 
         }else{
            next = (i+1) % NUMBER_TASKS;
            //update linked status
            linked = taskList[i].linkFlag //update
               if((linked == 0) && (taskList[i].reload == 1)){
                  // last task in this group (or not linked at all). Start reset if needed
                  reset_task(i);
               }
         }
         
         execute_command(&taskList[i].command); //run this task

      }else{
         next = (i+1) % NUMBER_TASKS;
      }
   }while(next != 0);
   //enable sw timer
}

//start timer and interrupts

//wake up from sleep for radios

run_active_tasks();

*/
