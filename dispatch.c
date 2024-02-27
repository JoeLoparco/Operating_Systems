/**
 * @file dispatch.c
 * @provides create, newpid, userret
 *
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

    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;

        /**
                * TODO:
                * 
                * Check to ensure the trap is an environment call from U-Mode
                */
                if (cause != E_ENVCALL_FROM_UMODE){
                //* If the trap is not an environment call from U-Mode call xtrap
                        xtrap(frame, cause, val, program_counter);

                }
                //* Find the system call number that's triggered
                ulong syscall_num = frame[CTX_A7]; // this is where I think the syscall number is but I'm not sure will ask brylow tomorrow.
                //* Pass the system call number and any arguments into syscall_dispatch. Make sure to set the return value in the appropriate spot.
                frame[CTX_A0] = syscall_dispatch(syscall_num, &frame[CTX_A0]); // Not sure where exactly on the frame the arguments for syscall are stored.
                //* Update the program counter appropriately with set_sepc
                set_sepc(program_counter + 4);
    }
}
