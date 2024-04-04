#include <xinu.h>

static ulong *pgTraverseAndCreate(pgtbl pagetable, ulong virtualaddr, int attr);

/**
 * Maps a page to a specific virtual address
 * @param pagetable     the base pagetable
 * @param page          the page that will be stored at the virtual address
 * @param virtualaddr   the virtual address at which the page will reside
 * @param attr          any attributes to set on the page table entry
 * @return              OK if successful, otherwise a SYSERR
 */
syscall mapPage(pgtbl pagetable, page pg, ulong virtualaddr, int attr)
{
    ulong addr;

    addr = (ulong)truncpage(virtualaddr);

    if (pgTraverseAndCreate(pagetable, addr, attr) == (ulong *)SYSERR)
    {
        return SYSERR;
    }

    return OK;
}

/**
 * Maps a given virtual address range to a corresponding physical address range.
 * @param pagetable    the base pagetable
 * @param virtualaddr  the start of the virtual address range. This will be truncated to the nearest page boundry.
 * @param physicaladdr the start of the physical address range
 * @param length       the length of the range to map
 * @param attr         any attributes to set on the page table entry
 * @return             OK if successful, otherwise a SYSERR
 */
syscall mapAddress(pgtbl pagetable, ulong virtualaddr, ulong physicaladdr,
               ulong length, int attr)
{
    ulong addr, end;
    ulong nlength;


    if (length == 0)
    {
        return SYSERR;
    }

    // Round the length to the nearest page size
    nlength = roundpage(length);
    addr = (ulong)truncpage(virtualaddr);
    end = addr + nlength;

    // Loop over the entire range
    for (; addr < end; addr += PAGE_SIZE, physicaladdr += PAGE_SIZE)
    {
        // Create a page table entry if one doesn't exist. Otherwise, get the existing page table entry.
        if (pgTraverseAndCreate(pagetable, addr, attr) == (ulong *)SYSERR)
        {
            return SYSERR;
        }
    }

    return OK;
}

/**
 * Starting at the base pagetable, tranverse the hierarchical page table structure for the virtual address.  Create pages along the way if they don't exist.
 * @param pagetable    the base pagetable
 * @param virtualaddr  the virtual address to find the it's corresponding page table entry.
 * @param attr	       the attributes to set on the leaf page
 * @return             OK
 */
static ulong *pgTraverseAndCreate(pgtbl pagetable, ulong virtualaddr, int attr)
{
    /**
    * TODO:
    * For each level in the page table, get the page table entry by masking and shifting the bits in the virtualaddr depending on the level
    * If the valid bit is set, use that pagetable for the next level
    * Otherwise create the page by calling pgalloc().  Make sure to setup the page table entry accordingly. Call sfence_vma once finished to flush TLB
    * Once you've tranversed all three levels, set the attributes (attr) for the leaf page (don't forget to set the valid bit!)
    */
	
	//ulong vpn0  = virtualaddr >> 30 & 0x1ff;
	//ulong vpn1 = (virtualaddr << 9) >> 30 & 0x1ff;
	//ulong vpn2 = (virtualaddr << 18) >> 30 & 0x1ff;
	/*
	if(!(vpn0 & PTE_V)){
		vpn0 = pgalloc();
	}
	if (!(vpn1 & PTE_V)){
		vpn1 = pgalloc();
	}
	if(!(vpn2 & PTE_V)){
		vpn2 = pgalloc();
	}
	
	sfence_vma();
	*/
	ulong vpn[3] = { // Store the virtual page numbers for each level
        virtualaddr >> 30 & 0x1ff,
        virtualaddr >> 21 & 0x1ff,
        virtualaddr >> 12 & 0x1ff 
    	};
	//kprintf("PTE adresses allocated succesefully\n\a");

    	pgtbl current_pagetable = pagetable; // Start at the root page table 

	//kprintf("root page assignmnet complete\n\a");

	pgtbl new_pagetable;

    	for (int i = 0; i < 2; i++) {
		//kprintf("for loop started\n\a");
        	ulong ptentry = current_pagetable[vpn[i]];
		//kprintf("uh-oh\n\a");
        	if (!(ptentry & PTE_V)) { // Page table entry is not valid
            		new_pagetable = (pgtbl)pgalloc(); // Allocate a new page table
			//kprintf("random stuff\n\a");
            		ptentry = PA2PTE((ulong)new_pagetable) | PTE_V;  // Create PTE
			//kprintf("maybe?\n\a");
            		current_pagetable[vpn[i]] = ptentry; // Update parent page table
        }
		
		//kprintf("doubt this happens\n\a");
        	current_pagetable = (pgtbl)PTE2PA(ptentry); // Move to the next level 

    }

    // At this point, we've reached the leaf level page 
    //ulong leaf_page = (ulong)PTE2PA(current_pagetable[vpn[2]]); 
    current_pagetable[vpn[2]] = attr | PTE_V | PA2PTE(virtualaddr) ; // Set attributes and valid bit
    	

    sfence_vma(); // Flush the TLB

    return (ulong *)OK;
}

