/**
 * @file testcases.c
 * @provides testcases
 *
 * TA-BOT:MAILTO
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

extern void main(int, char *);

int testmain(int argc, char **argv)
{
    int i = 0;
    kprintf("Hello XINU World!\r\n");

    for (i = 0; i < 10; i++)
    {
        kprintf("This is process %d\r\n", currpid);

        /* Uncomment the resched() line for cooperative scheduling. */
        // resched();
    }
    return 0;
}

void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p)
{
    kprintf("Testing bigargs...\r\n");
    kprintf("a = 0x%08X\r\n", a);
    kprintf("b = 0x%08X\r\n", b);
    kprintf("c = 0x%08X\r\n", c);
    kprintf("d = 0x%08X\r\n", d);
    kprintf("e = 0x%08X\r\n", e);
    kprintf("f = 0x%08X\r\n", f);
    kprintf("g = 0x%08X\r\n", g);
    kprintf("h = 0x%08X\r\n", h);
    kprintf("i = 0x%08X\r\n", i);
    kprintf("j = 0x%08X\r\n", j);
    kprintf("k = 0x%08X\r\n", k);
    kprintf("l = 0x%08X\r\n", l);
    kprintf("m = 0x%08X\r\n", m);
    kprintf("n = 0x%08X\r\n", n);
    kprintf("o = 0x%08X\r\n", o);
    kprintf("p = 0x%08X\r\n", p);
}

void printpcb(int pid)
{
    pcb *ppcb = NULL;

    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];

    /* Printing PCB */
    kprintf("Process name		  : %s \r\n", ppcb->name);

    switch (ppcb->state)
    {
    case PRFREE:
        kprintf("State of the process	  : FREE \r\n");
        break;
    case PRCURR:
        kprintf("State of the process 	  : CURRENT \r\n");
        break;
    case PRSUSP:
        kprintf("State of the process	  : SUSPENDED \r\n");
        break;
    case PRREADY:
        kprintf("State of the process	  : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", ppcb->stkbase);
    kprintf("Stack length of process   : %8u \r\n", ppcb->stklen);
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c, pid;

    kprintf("0) Test creation of one process\r\n");
    kprintf("1) Test passing of many args\r\n");
    kprintf("2) Create three processes and run them\r\n");

    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
    case '0':
        // Process creation testcase
        pid = create((void *)testmain, INITSTK, "MAIN1", 2, 0, NULL);
        printpcb(pid);
        break;

    case '1':
        // Many arguments testcase
        pid = create((void *)testbigargs, INITSTK, "MAIN1", 16,
                     0x11111111, 0x22222222, 0x33333333, 0x44444444,
                     0x55555555, 0x66666666, 0x77777777, 0x88888888,
                     0x99999999, 0x10101010, 0x11111111, 0x12121212,
                     0x13131313, 0x14141414, 0x15151515, 0x16161616);
        printpcb(pid);
        // TODO: print out stack with extra args
	
	pcb *ppcb = &proctab[pid];

    	kprintf("Stack contents:\r\n");

    	ulong *argStart = (ulong *)(ppcb->stkbase - (16 * sizeof(ulong))); 
	// (16 total args)

	// Print the extra arguments 
	for (int i = 0; i < 16 ; i++) {
    		kprintf("Extra arg %d: 0x%08X\r\n", i + 1, *argStart);
    		argStart--; // Move to the next argument
	}

        // TODO: ready(pid, RESCHED_YES);
        break;

    case '2':
        // Create three copies of a process, and let them play.
        ready(create((void *)testmain, INITSTK, "MAIN1", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, "MAIN2", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, "MAIN3", 2, 0, NULL),
              RESCHED_YES);
        break;

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
