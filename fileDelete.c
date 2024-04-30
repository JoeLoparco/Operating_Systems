/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by Joseph Loparco                                                    */
/*                                                                 */
/* and Max Steffens & Google gemini                                                            */
/*                                                                 */
/*                                                                 */

#include <kernel.h>
#include <memory.h>
#include <file.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd)
{
    // TODO: Unlink this file from the master directory index,
    //  and return its space to the free disk block list.
    //  Use the superblock's locks to guarantee mutually exclusive
    //  access to the directory indexes
    int result;
    
    wait(supertab->sb_dirlock);



    
    if ((NULL == supertab) || (NULL == filetab))
    {
        return SYSERR;
    }

    struct dentry *phw = supertab->sb_disk;
    int diskfd = phw - devtab;


    if(fd >= DIRENTRIES)
    {
	signal(supertab->sb_dirlock);
	return SYSERR;
    }	
    
    filetab[fd].fn_length = 0;
    filetab[fd].fn_cursor = 0;
    filetab[fd].fn_name[0] = "\0";
    free(filetab[fd].fn_data);
    filetab[fd].fn_state = FILE_FREE;
    sbFreeBlock(supertab, filetab->fn_blocknum);
	
    seek(diskfd, supertab->sb_dirlst->db_blocknum);
    if(SYSERR == write(diskfd,supertab->sb_dirlst, sizeof(struct dirblock)))
	   {
		   return SYSERR;
	   }
    signal(supertab->sb_dirlock);
    return OK;

}
