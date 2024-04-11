/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Allocate heap memory.
 *
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    register memblk *prev, *curr, *leftover;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);
    struct memhead *head = (memhead *)PROCHEAPADDR;

    /* TODO:
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with remainder splitting)
     *      - if there is no suitable block, call user_incheap
     *        with the request to add more pages to our stack
     *      - return memory address if successful
     */

    curr = head->head;
    prev = (struct memblock *)head;

   // Iterate through the freelist
   //kprintf("%d",curr->length);
   while(curr != NULL && curr->length < nbytes) {
        kprintf("Looking through Freelist...\n\r");
        curr = curr->next;
   }

   if (curr != NULL) { // Suitable block found
        kprintf("Found Memory Block\n\r");

        // Split the block if necessary
        if (curr->length > nbytes) {
            leftover = (struct memblock *)((ulong)curr + nbytes);
            leftover->length = curr->length - nbytes;
            leftover->next = curr->next;
            curr->next = leftover;
            head->length -= nbytes; // Update the total free memory length 
        }

        // Return the address of the current block
        return (void *)curr; 
   } else { // No suitable block found
        ulong addr = sc_incheap(nbytes);  
        // (Ensure user_incheap updates the free list correctly)
        return (void *)addr;  // Assuming user_incheap returns a valid address
    }

    return (void *)SYSERR;
}
