#include <stdint.h>
#include "task/task.h"

void task_create(TCB_t *tcb, void (*entry)(void), uint8_t task_id)
{
    tcb->entry = entry;
    tcb->task_id = task_id;
    tcb->task_state = READY;

    uint32_t *sp = &(tcb->stack[255]);
    
    *(sp--) = 0x01000000;        
    *(sp--) = (uint32_t) entry;  
    *(sp--) = 0xFFFFFFF9;        
    *(sp--) = 0;                 
    *(sp--) = 0;                 
    *(sp--) = 0;                
    *(sp--) = 0;                
    *(sp--) = 0;                 

    tcb->stack_pointer = sp + 1;
}