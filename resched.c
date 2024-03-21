/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 6
 * @author Maxwell Steffen, Joseph Loparco, ChatGPT4
 * Instructor Brylow
 * TA-BOT:MAILTO maxwell.steffen@marquette.edu joseph.loparco@marquette.edu
*/
/* Embedded XINU, Copyright (C) 2008,2024.  All rights reserved. */

#include <xinu.h>

ulong pickRandTicket();
ulong getTicketNum();
ulong findRandTicket(ulong);

extern void ctxsw(void *, void *);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */

    oldproc = &proctab[currpid];

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in
     * current and ready states.
     * Use random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the
     * random ticket value.  Remove process from queue.
     * Set currpid to the new process.
     */
    ulong randTicket =  pickRandTicket();
    currpid = findRandTicket(randTicket);

    remove(currpid);
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt = QUANTUM;
#endif

    ctxsw(&oldproc->stkptr, &newproc->stkptr);

    /* The OLD process returns here when resumed. */
    return OK;
}

ulong getTicketNum(){ // counts total number of tickets that are held by the processes in the ready and current state
	int i = 0; 
	ulong total_tickets = 0; 
	for(int i= 0; i < NPROC; i++){
		if(proctab[i].state == PRREADY || proctab[i].state == PRCURR)
			total_tickets += proctab[i].tickets;

	}
	return total_tickets;
}

ulong pickRandTicket(){ // generates random possible ticket
	ulong ticket = random(getTicketNum());
	return ticket; 
}

ulong findRandTicket(ulong randTicketNum){ //this function looks for the winning ticket and trurtns the pid of the corresponsging proc
	ulong cumulativeTick = 0; 
	for(int i= 0; i < NPROC; i++){
		if(proctab[i].state == PRREADY || proctab[i].state == PRCURR){
			cumulativeTick += proctab[i].tickets;
			if(cumulativeTick > randTicketNum){
				//proctab[i].state = PRCURR;
				return i;
			}
		}
	}
	return 0;
}

