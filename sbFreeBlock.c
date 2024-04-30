/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by Joseph Loparco                                      */
/*                                                                 */
/* and Max Steffens & Google Gemini                                */
/* Instructor Brylow                                               */
/* TA-BOT:MAILTO maxwell.steffen@marquette.edu joseph.loparco@marquette.edu */
/*                                                                 */
/*                                                                 */

#include <kernel.h>
#include <device.h>
#include <memory.h>
#include <disk.h>
#include <file.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall swizzle(int diskfd, struct freeblock *freeblk)
{
	struct freeblock *free2 = freeblk->fr_next;

	if(NULL == freeblk->fr_next)
	{
		freeblk->fr_next = 0;
	}else
	{
		freeblk->fr_next = (struct freeblock *)freeblk->fr_next->fr_blocknum;
	}
	seek(diskfd, freeblk->fr_blocknum);
	if(SYSERR == write(diskfd, freeblk, sizeof(struct freeblock)))
	{
		return SYSERR;	
	}
	freeblk->fr_next = free2;
	return OK;
}

devcall swizzleSuperBlock(int diskfd, struct superblock *psuper)
{
	struct freeblock *swizzle = psuper->sb_freelst;
	struct dirblock *swizzle2 = psuper->sb_dirlst;

	psuper->sb_freelst = (struct freeblock *)swizzle->fr_blocknum;
	psuper->sb_dirlst = (struct dirblock *)swizzle2->db_blocknum;

	seek(diskfd, psuper->sb_blocknum);
	if(SYSERR == write(diskfd, psuper, sizeof(struct superblock))){
		return SYSERR;
	}

	psuper->sb_freelst = swizzle;
	psuper->sb_dirlst = swizzle2;
	return OK;
}

devcall sbFreeBlock(struct superblock *psuper, int block)
{
    // TODO: Add the block back into the filesystem's list of
    //  free blocks.  Use the superblock's locks to guarantee
    //  mutually exclusive access to the free list, and write
    //  the changed free list segment(s) back to disk

	struct dentry *phw = psuper->sb_disk;

   if (psuper == NULL || block < 0 || block >= psuper->sb_blocktotal) {
        return SYSERR; // Invalid superblock, bad block number
    }

    // Acquire Lock
    wait(psuper->sb_freelock);
    int diskfd = phw - devtab;

    //Case 1
    struct freeblock *freeblk = psuper->sb_freelst;

    if(freeblk == NULL){
	freeblk = (struct freeblock *)malloc(sizeof(struct freeblock));
	freeblk->fr_blocknum  = block;
	freeblk->fr_count = 0;
	freeblk->fr_next = NULL;
	psuper->sb_freelst = freeblk;
	swizzle(diskfd, psuper);
	signal(psuper->sb_freelock);
	return OK;
    }    

    //Case 2
    struct freeblock *curr = psuper->sb_freelst;
   // kprintf("count: %d",curr->fr_count);
   // kprintf("block num %d",curr->fr_blocknum);
    struct freeblock *prev = NULL; // Keep track of the previous node
 
    while (curr->fr_next != NULL) {
       // kprintf("iterating through blocks\n\t");
        curr = curr->fr_next;
    }

    if(curr->fr_count >= FREEBLOCKMAX){
    	struct freeblock *newfreeblk = (struct freeblock *)malloc(sizeof(struct freeblock));
	newfreeblk->fr_blocknum;
	newfreeblk->fr_count;
	newfreeblk->fr_next;
	curr->fr_next = newfreeblk;
	swizzle(diskfd, curr);
	signal(psuper->sb_freelock);
	return OK;
    }else // Case 3
    {
	curr->fr_free[curr->fr_count] = block;
	curr->fr_count ++;
	swizzle(diskfd, curr);
	signal(psuper->sb_freelock);
    	return OK;
    }

    signal(psuper->sb_freelock);
    return OK;
}
