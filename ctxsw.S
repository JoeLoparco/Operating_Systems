/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 * 
 * @author Maxwell Steffen, Joseph Loparco, ChatGPT4
 * Instructor Brylow
 * TA-BOT:MAILTO maxwell.steffen@marquette.edu joseph.loparco@marquette.edu
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <riscv.h>

.text
        .align 4
        .globl  ctxsw


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
//      callee-save ("non-volatile") registers.

        addi sp,sp,-256

        sd a0, CTX_A0*8(sp) #saves register x3 to sp offset by 24(3*8)
        sd a1, CTX_A1*8(sp) #saves register x4 to sp offset by 32(4*8)
        sd a2, CTX_A2*8(sp) #saves register x5 to sp offset by 40(5*8)
        sd a3, CTX_A3*8(sp) #saves register x6 to sp offset by 48(6*8)
        sd a4, CTX_A4*8(sp) #saves register x7 to sp offset by 56(7*8)
        sd a5, CTX_A5*8(sp) #saves register x8 to sp offset by 64(8*8)
        sd a6, CTX_A6*8(sp) #saves register x9 to sp offset by 72(9*8)
        sd a7, CTX_A7*8(sp) #saves register x10 to sp offset by 80(10*8)
        sd s0, CTX_S0*8(sp) #saves register x3 to sp offset by 24(3*8)
        sd s1, CTX_S1*8(sp) #saves register x4 to sp offset by 32(4*8)
        sd s2, CTX_S2*8(sp) #saves register x5 to sp offset by 40(5*8)
        sd s3, CTX_S3*8(sp) #saves register x6 to sp offset by 48(6*8)
        sd s4, CTX_S4*8(sp) #saves register x7 to sp offset by 56(7*8)
        sd s5, CTX_S5*8(sp) #saves register x8 to sp offset by 64(8*8)
        sd s6, CTX_S6*8(sp) #saves register x9 to sp offset by 72(9*8)
        sd s7, CTX_S7*8(sp) #saves register x10 to sp offset by 80(10*8)
        sd s8, CTX_S8*8(sp) #saves register x11 to sp offset by 88(11*8)
        sd s9, CTX_S9*8(sp) #saves register x12 to sp offset by 96(12*8)
        sd s10, CTX_S10*8(sp) #saves register x13 to sp offset by 104(13*4)
        sd s11, CTX_S11*8(sp) #saves register x14 to sp offset by 112(14*8)
        sd ra, CTX_RA*8(sp)
        sd sp, CTX_SP*8(sp)
        sd ra, CTX_PC*8(sp)


// TODO: Restore process state, including at least the
//      

        sd sp, (a0)
        ld sp, (a1)
        mv t1, a1

        ld a0, CTX_A0*8(sp) #saves register x3 to sp offset by 24(3*8)
        ld a1, CTX_A1*8(sp) #saves register x4 to sp offset by 32(4*8)
        ld a2, CTX_A2*8(sp) #saves register x5 to sp offset by 40(5*8)
        ld a3, CTX_A3*8(sp) #saves register x6 to sp offset by 48(6*8)
        ld a4, CTX_A4*8(sp) #saves register x7 to sp offset by 56(7*8)
        ld a5, CTX_A5*8(sp) #saves register x8 to sp offset by 64(8*8)
        ld a6, CTX_A6*8(sp) #saves register x9 to sp offset by 72(9*8)
        ld a7, CTX_A7*8(sp) #saves register x10 to sp offset by 80(10*8)
        ld s0, CTX_S0*8(sp) #saves register x3 to sp offset by 24(3*8)
        ld s1, CTX_S1*8(sp) #saves register x4 to sp offset by 32(4*8)
        ld s2, CTX_S2*8(sp) #saves register x5 to sp offset by 40(5*8)
        ld s3, CTX_S3*8(sp) #saves register x6 to sp offset by 48(6*8)
        ld s4, CTX_S4*8(sp) #saves register x7 to sp offset by 56(7*8)
        ld s5, CTX_S5*8(sp) #saves register x8 to sp offset by 64(8*8)
        ld s6, CTX_S6*8(sp) #saves register x9 to sp offset by 72(9*8)
        ld s7, CTX_S7*8(sp) #saves register x10 to sp offset by 80(10*8)
        ld s8, CTX_S8*8(sp) #saves register x11 to sp offset by 88(11*8)
        ld s9, CTX_S9*8(sp) #saves register x12 to sp offset by 96(12*8)
        ld s10, CTX_S10*8(sp) #saves register x13 to sp offset by 104(13*4)
        ld s11, CTX_S11*8(sp) #saves register x14 to sp offset by 112(14*8)
        ld ra, CTX_RA*8(sp)
        ld t0, CTX_PC*8(sp)

        addi    sp,sp,256

//      These call and restore segments must match the register data
//      layout you choose in create().

// TODO: Jump to next function
            sd sp, (t1)
//          jalr zero, (t0)           // Jump to the new program counter
//      Note: there are two important values at play here --
//      the address of the process's function that we want to end up in
//      once we leave this ctxsw(), and the value we want to have in the
//      return address register.  Those are almost always the same value, except
//      for the very first time we ctxsw() to a new process, when they
//      are different.  See create() for more details.

// Note: This ret line immediately returns from whence we came.  This
//      is here as a placeholder so that you don't crash your O/S when
//      you call ctxsw() before it is complete.  But, this line of code
//      is not actually a part of a completed solution.
//      ret

// ASSIGNMENT 5 TODO:
// Note: the following code is for the next assignment.  DO NOT uncomment it until you believe your trap handler is working in assignment
// Determine which mode we privilege mode the process should be in
//      ld t0, CTX_PC*8(t0)
        beq t0, ra, switch
        li t4, SSTATUS_S_MODE
        csrc sstatus, t4
        csrw sepc, t0

        sret
switch:
        ret

        .end ctxsw                                                                                                                                                                  100,10-17     B                                                                                                                                                                  1,3           Top
~                                        
