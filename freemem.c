/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with getmem().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to getmem().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    register struct memblock *block, *next, *prev;
    struct memhead *head = NULL;
    ulong top;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < (ulong)PROCHEAPADDR))
    {
	kprintf("SYS ERR: Block not found in heap\r\n");    
        return SYSERR;
    }

    head = (struct memhead *)PROCHEAPADDR;
    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     */
    prev =(struct memblock*) head;
    next = head->head; 

    // Find where to insert the block on the freelist
    while (next != NULL && next < block) {
        prev = next;
        next = next->next;
    }

    // Check for overlaps with previous and next blocks
    if (prev != NULL && ((ulong)prev + prev->length) > (ulong)block) {
        kprintf("SYS ERR: oervlap w/ previous block.\r\n");
	return SYSERR;  // Overlap with previous block
    }
    if (next != NULL && (ulong)block + nbytes > (ulong)next) {
        kprintf("SYS ERR; overlap /w next blcok\r\n");
	return SYSERR;  // Overlap with next block
    }

    // Coalesce with previous block if adjacent
    if (prev != NULL && ((ulong)prev + prev->length == (ulong)block)) {
        prev->length += nbytes;
        block = prev;
    } else {
        block->length = nbytes;
        block->next = next;
        prev->next = block;
    }

    // Coalesce with next block if adjacent 
    if (next != NULL && ((ulong)block + block->length == (ulong)next)) {
        block->length += next->length;
        block->next = next->next;
    }

    head->length += nbytes; // Update the total free memory length

    kprintf("got through function");
    return OK;
}
