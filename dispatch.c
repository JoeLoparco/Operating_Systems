/**
 * @file dispatch.c
 * @provides create, newpid, userret
 * COSC 3250 Assignment 5
 * @author Maxwell Steffen, Joseph Loparco, ChatGPT4
 * Instructor Brylow
 * TA-BOT:MAILTO maxwell.steffen@marquette.edu joseph.loparco@marquette.edu
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */


#include <xinu.h>
#include <interrupt.h>

/**
 * 
 * @ingroup process
 * Dispatch the trap or exception handler, called via interrupt.S
 * @param cause  The value of the scause register 
 * @param stval  The value of the stval register  
 * @param frame  The stack pointer of the process that caused the interupt 
 * @param program_counter  The value of the sepc register 
 */

void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter) {
    ulong swi_opcode;
    pcb *ppcb = &proctab[currpid];
    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;

     /**
      * Note: sawp area = the virtual adress where pur process is stored in the O/S
                * TODO:
                * 
                * Check to ensure the trap is an environment call from U-Mode
                */
                if (cause != E_ENVCALL_FROM_UMODE){
                //* If the trap is not an environment call from U-Mode call xtrap
                        xtrap(ppcb->swaparea, cause, val, program_counter);
                }
                //* Find the system call number that's triggered
                ulong syscall_num = ppcb->swaparea[CTX_A7]; // this is where I think the syscall number is but I'm not sure will ask brylow tomorrow.
                //* Pass the system call number and any arguments into syscall_dispatch. Make sure to set the return value in the appropriate spot.
                ppcb->swaparea[CTX_A0] = syscall_dispatch(syscall_num, ppcb->swaparea); //
                //* Update the program counter appropriately with set_sepc
                set_sepc(program_counter + 4);
    }else {
    	cause = cause << 1;
    	cause = cause >> 1;
    	uint irq_num;

    	volatile uint *int_sclaim = (volatile uint *)(PLIC_BASE + 0x201004);
    	irq_num = *int_sclaim;

    	if(cause == I_SUPERVISOR_EXTERNAL) {
        	interrupt_handler_t handler = interruptVector[irq_num];

        	*int_sclaim = irq_num;
        if (handler)
        {
            (*handler) ();
        } else {
            kprintf("ERROR: No handler registered for interrupt %u\r\n",
                    irq_num);

            while (1)
                ;
        }
    }
  }
}
