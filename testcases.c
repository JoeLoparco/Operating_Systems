/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	
 * @author Maxwell Steffen, Joseph Loparco, ChatGPT4
 * TA-BOT:MAILTO maxwell.steffen@marquette.edu joseph.loparco@marquette.edu 
 *
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

/* Here is a visual representation of the page tables createFakeTable makes.
 * This will allow you to test your printPageTable function without having paging
 * completely working.
 * NOTE: You must have lines 57-58 in initialize.c uncommented for this to work!
 * ┌──────────┐                  ┌──────────┐                ┌───────────┐
 * │          │                  │          │                │           │
 * │          │                  │          │                ├───────────┤
 * |		  |					 |			|				 |  0x8000	 |
 * |──────────|                  |──────────|                ├───────────┤
 * │          ├─────────────────►│          ├───────────────►│           │
 * │──────────│                  |──────────|                ├───────────┤
 * |		  |					 |			|				 |  0x4000	 |
 * │          │                  │          │                ├───────────┤
 * │          │                  │          │                │           │
 * │          │                  └──────────┘                ├───────────┤
 * |		  |					            				 |  0x1000	 |
 * │          │                                              ├───────────┤
 * │          │                                              │           │
 * │          │                                              └───────────┘
 * └──────────┘
 */
pgtbl createFakeTable(void){
	pgtbl root = pgalloc();
	pgtbl lvl1 = pgalloc();
	pgtbl lvl0 = pgalloc();

	volatile ulong *pte = &(root[5]);
	*pte = PA2PTE(lvl1) | PTE_V;

	ulong *lvl1pte = &(lvl1[145]);
	*lvl1pte = PA2PTE(lvl0) | PTE_V;

	ulong *lvl0pte = &(lvl0[343]);
	*lvl0pte = PA2PTE(0x1000) | PTE_W | PTE_R | PTE_V;

	ulong *lvl0pte1 = &(lvl0[120]);
	*lvl0pte1 = PA2PTE(0x4000) | PTE_X | PTE_R | PTE_V;

	ulong *lvl0pte2 = &(lvl0[45]);
	*lvl0pte2 = PA2PTE(0x8000) | PTE_X | PTE_R | PTE_V;

	return root;kprintf("ptentry is link");
}

void userProcess(void) {
    int counter = 0;

    while (1) {
        kprintf("Hello from user process! (Iteration: %d) \r\n", counter++);
        for(int i=0; i < 1000000; i++)
	{
	}	// Sleep for small period of time
    }
}

void printPageTable(pgtbl pagetable)
{
	ulong ptentry;
	ulong frameaddr; 
	for(int i=0; i < 512 ;i ++){ // i needs to be less than number of page table entries in the given page table(via safemem.h)
		 ptentry = pagetable[i]; // get page table entry
	
		if (ptentry & PTE_V) {
			//kprintf("ptenrty validattion succesful.\r\n");
			if(!(ptentry & (PTE_W | PTE_R | PTE_X))){
				//kprintf("ptentry is link\r\n");
				pgtbl nxtpgtbl = (pgtbl)PTE2PA(ptentry);
				//kprintf("next page table adress created succesfully\r\n");
				frameaddr = PTE2PA(ptentry);
                                kprintf("LINK: entry #%d: maps to 0x%08x\r\n", i, frameaddr);
				printPageTable(nxtpgtbl);
				//kprintf("printPagaTable occured\r\n");
			}else {
				frameaddr = PTE2PA(ptentry);
				kprintf("LEAF: entry #%d: maps to 0x%08x\r\n", i, frameaddr);
			}
		}

	}  
	/*
	* TODO: Write a function that prints out the page table.
	* Your function should print out all *valid* page table entries in the page table
	* If any of the entires are a link (if the Read/Write/Execute bits aren't set), recursively print that page table
	* Otherwise if it's a leaf, print the page table entry and the physical address is maps to. 
	*/

}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	uchar c;

	kprintf("===TEST BEGIN===\r\n");

	// TODO: Test your operating system!

	c = kgetc();

	switch (c)
	{
		case '0':
			// TODO: Write a testcase that creates a user process and prints out it's page table
			pgtbl pagetable = createFakeTable();
			printPageTable(pagetable);
			//pid_typ newpid = create((void *)userProcess, INITSTK, 2, "PROC1", 0, NULL);
			//pcb *userproc = &proctab[newpid];
			//printPageTable(userproc->pagetable);
			kprintf("Test Case 0 Done.");
			break;
		case '1':
			// TODO: Write a testcase that demonstrates a user process cannot access certain areas of memory
			break;
		case '2':
			// TODO: Write a testcase that demonstrates a user process can read kernel variables but cannot write to them
			break;
		case '3':
			// TODO: Extra credit! Add handling in xtrap to detect and print out a Null Pointer Exception.  Write a testcase that demonstrates your OS can detect a Null Pointer Exception.
			break;
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
