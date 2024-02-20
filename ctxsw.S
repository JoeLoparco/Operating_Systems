/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <riscv.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldstack, &newstack)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the program counter for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
	.func ctxsw
ctxsw:
    addi    sp, sp, -128          # Reserve stack space (TODO: Save process state)

    # TODO: Save process state
    sd      ra, 120(sp)           # Save return address
    sd      s0, 112(sp)           # Save callee-saved registers (frame pointer)
    sd      s1, 104(sp)
    # Continue saving s2-s11, adjust offsets accordingly

    # Switch stacks (Address of old & new stack provided in a0 & a1)
    ld      t0, 0(a0)             # Load old stack pointer address
    sd      sp, 0(t0)             # Save current stack pointer to old stack
    ld      sp, 0(a1)             # Load new stack pointer from new stack

    # TODO: Restore process state
    ld      ra, 120(sp)           # Restore return address
    ld      s0, 112(sp)           # Restore callee-saved registers (frame pointer)
    ld      s1, 104(sp)
    # Continue restoring s2-s11, adjust offsets accordingly
    addi    sp, sp, 128           # Restore original stack pointer

    ret                           # Return to new context (TODO: Jump to next function)

// ASSIGNMENT 5 TODO:
// Note: the following code is for the next assignment.  DO NOT uncomment it until you believe your trap handler is working in assignment
// Determine which mode we privilege mode the process should be in
//	ld t0, CTX_PC*8(**REGISTER THAT HOLDS PROCESS CONTEXT**)
//	beq t0, ra, switch
//	li t4, SSTATUS_S_MODE
//	csrc sstatus, t4
//	csrw sepc, t0
//
//	sret
//switch:
//	ret

	.end ctxsw
