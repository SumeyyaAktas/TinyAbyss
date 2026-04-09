.syntax unified
.cpu cortex-m3
.thumb

.global PendSV_Handler
.thumb_func
PendSV_Handler:
    MRS r0, PSP
    STMDB r0!, {r4-r11}

    LDR r1, =current_task_index
    LDRB r2, [r1]

    LDR r3, =task_table
    LDR r3, [r3, r2, LSL #2]     
    STR r0, [r3]                  

    PUSH {LR}
    BL scheduler_next
    POP {LR}

    LDR r1, =current_task_index
    LDRB r2, [r1]

    LDR r3, =task_table
    LDR r3, [r3, r2, LSL #2]     
    LDR r0, [r3]                  

    LDMIA r0!, {r4-r11}
    MSR PSP, r0

    BX LR