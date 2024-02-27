/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 Assignment 4
 * @author Maxwell Steffen, Joseph Loparco, ChatGPT4
 * Instructor Brylow
 * TA-BOT:MAILTO maxwell.steffen@marquette.edu joseph.loparco@marquette.edu
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */

    if (ssize < MINSTK)
        ssize = MINSTK;

    ssize = (ulong)((((ulong)(ssize + 3)) >> 2) << 2);
    saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
    pid = newpid();
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];

    // Setup PCB entry for new process.
    ppcb->state = PRSUSP;
    ppcb->stkbase = (void *)(saddr);
    ppcb->stklen = ssize;
    strncpy(ppcb->name, name, PNMLEN - 1);
    ppcb->name[PNMLEN - 1] = '\0';

    *saddr = STACKMAGIC; // Initialize stack with accounting block
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    if (nargs)
    {
        pads = ((nargs - 1) / 8) * 8;
    }
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0; // Pad for extra args if more than 4
    }

    // Initialize process context.
    for (i = 0; i < 32; i++) *--saddr = 0; // Emulate saved registers
        saddr[CTX_PC] = funcaddr;
        saddr[CTX_RA] = userret;
        saddr[CTX_SP] = saddr;
    // Place arguments into activation record.

    va_start(ap, nargs);
    ulong ival;
    int w = 0;
    ulong *p;
    for(p = ap; *p; p++){
        ival = va_arg(ap, ulong);
        *(w + saddr) = ival;
        w++;
        if(pads!=0 && w==8)
        {
                w+=24;
        }
    }
   /* for (i = 0; i < nargs; i++) {
         // Place arguments on stack
        
        if (i < 8){
        saddr[i] = va_arg(ap, ulong);
        }
        else{
        *--saddr = va_arg(ap, ulong);
        }
    }*/

    ppcb->stkptr = saddr; // Update process stack pointer in PCB
    va_end(ap);
    return pid;
}

static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++) { /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state) {
            return nextpid;
        }
    }
    return SYSERR;
}

void userret(void)
{
   user_kill(); // Terminate the current process
}
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
~                 
