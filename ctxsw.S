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

// TODO: Save process state, including at least the 
//	callee-save ("non-volatile") registers.
	
	sd ra, CTX_A1*8(sp) #saves register x1 to sp offset by 8(1*8)
	sd sp, CTX_A2*8(sp) #saves register x2 to sp offset by 16(2*8)
	sd s0, CTX_A3*8(sp) #saves register x3 to sp offset by 24(3*8)
	sd s1, CTX_A4*8(sp) #saves register x4 to sp offset by 32(4*8)
	sd s2, CTX_A5*8(sp) #saves register x5 to sp offset by 40(5*8)
        sd s3, CTX_A6*8(sp) #saves register x6 to sp offset by 48(6*8)
        sd s4, CTX_A7*8(sp) #saves register x7 to sp offset by 56(7*8)
        sd s5, CTX_A8*8(sp) #saves register x8 to sp offset by 64(8*8)
 	sd s6, CTX_A9*8(sp) #saves register x9 to sp offset by 72(9*8)
        sd s7, CTX_A10*8(sp) #saves register x10 to sp offset by 80(10*8)
        sd s8, CTX_A11*8(sp) #saves register x11 to sp offset by 88(11*8)
        sd s9, CTX_A12*8(sp) #saves register x12 to sp offset by 96(12*8)
        sd s10, CTX_A13*8(sp) #saves register x13 to sp offset by 104(13*4)
 	sd s11, CTX_A14*8(sp) #saves register x14 to sp offset by 112(14*8)
        sd gp, CTX_A15*8(sp) #saves register x15 to sp offset by 120(15*8)
	sd tp, CTX_A16*8(sp) # Save register x16
        /*sd x17, CTX_A17*8(sp) # Save register x17
        sd x18, CTX_A18*8(sp) # Save register x18
        sd x19, CTX_A19*8(sp) # Save register x19
        sd x20, CTX_A20*8(sp) # Save register x20
        sd x21, CTX_A21*8(sp) # Save register x21
        sd x22, CTX_A22*8(sp) # Save register x22
        sd x23, CTX_A23*8(sp) # Save register x23
        sd x24, CTX_A24*8(sp) # Save register x24
        sd x25, CTX_A25*8(sp) # Save register x25
        sd x26, CTX_A26*8(sp) # Save register x26
        sd x27, CTX_A27*8(sp) # Save register x27
        sd x28, CTX_A28*8(sp) # Save register x28
        sd x29, CTX_A29*8(sp) # Save register x29
        sd x30, CTX_A30*8(sp) # Save register x30
        sd x31, CTX_A31*8(sp) # Save register x31
	*/
// TODO: Restore process state, including at least the
//      callee-save ("non-volatile") registers.

	ld tp, CTX_A16*8(sp) # loads register x1 from sp offset by 8(1*8)
        ld gp, CTX_A15*8(sp) # loads register x2 from sp offset by 16(2*8)
        ld s11, CTX_A14*8(sp) # loads register x3 from sp offset by 24(3*8)
        ld s10, CTX_A13*8(sp) # loads register x4 from sp offset by 31(4*8)
        ld s9, CTX_A12*8(sp) # loads register x5 from sp offset by 40(5*8)
        ld s8, CTX_A11*8(sp) # loads register x6 from sp offset by 48(6*8)
        ld s7, CTX_A10*8(sp) # loads register x7 from sp offset by 56(7*8)
        ld s6, CTX_A9*8(sp) # loads register x8 from sp offset by 64(8*8)
        ld s5, CTX_A8*8(sp) # loads register x9 from sp offset by 72(9*8)
        ld s4, CTX_A7*8(sp) # loads register x10 from sp offset by 80(10*8)
        ld s3, CTX_A6*8(sp) # loads register x11 from sp offset by 88(11*8)
        ld s2, CTX_A5*8(sp) # loads register x12 from sp offset by 96(12*8)
        ld s1., CTX_A4*8(sp) # loads register x13 from sp offset by 104(13*8)
        ld s0, CTX_A3*8(sp) # loads register x14 from sp offset by 112(14*8)
        ld sp, CTX_A2*8(sp) # loads register x15 from sp offset by 120(15*8)
	ld ra, CTX_A1*8(sp) # loads register x1 from sp offset by 8(1*8)
        /*ld x17, CTX_A17*8(a1) # loads register x2 from sp offset by 16(2*8)
        ld x18, CTX_A18*8(a1) # loads register x3 from sp offset by 24(3*8)
        ld x19, CTX_A19*8(a1) # loads register x4 from sp offset by 31(4*8)
        ld x20, CTX_A20*8(a1) # loads register x5 from sp offset by 40(5*8)
        ld x21, CTX_A21*8(a1) # loads register x6 from sp offset by 48(6*8)
        ld x22, CTX_A22*8(a1) # loads register x7 from sp offset by 56(7*8)
        ld x23, CTX_A23*8(a1) # loads register x8 from sp offset by 64(8*8)
        ld x24, CTX_A24*8(a1) # loads register x9 from sp offset by 72(9*8)
        ld x25, CTX_A25*8(a1) # loads register x10 from sp offset by 80(10*8)
        ld x26, CTX_A26*8(a1) # loads register x11 from sp offset by 88(11*8)
        ld x27, CTX_A27*8(a1) # loads register x12 from sp offset by 96(12*8)
        ld x28, CTX_A28*8(a1) # loads register x13 from sp offset by 104(13*8)
        ld x29, CTX_A29*8(a1) # loads register x14 from sp offset by 112(14*8)
        ld x30, CTX_A30*8(a1) # loads register x15 from sp offset by 120(15*8)
	ld x31, CTX_A31*8(a1) 
	*/
//      These call and restore segments must match the register data
//      layout you choose in create().

// TODO: Jump to next function

	ld t5, 0(a1)
	
	jalr x0, 0(t5)
//	Note: there are two important values at play here --
//	the address of the process's function that we want to end up in
//	once we leave this ctxsw(), and the value we want to have in the
//	return address register.  Those are almost always the same value, except
//	for the very first time we ctxsw() to a new process, when they
//	are different.  See create() for more details.

// Note: This ret line immediately returns from whence we came.  This
//	is here as a placeholder so that you don't crash your O/S when
//	you call ctxsw() before it is complete.  But, this line of code
//	is not actually a part of a completed solution.
	ret

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
