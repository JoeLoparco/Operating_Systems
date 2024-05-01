/* passwdFileRead.c - passwdFileRead */
/* Copyright (C) 2024, Marquette University.  All rights reserved. */
/*Modified by Joseph Loparco                                      */
/*                                                                 */
/* and Max Steffens & Google Gemini                                */
/* Instructor Brylow                                               */
/* TA-BOT:MAILTO maxwell.steffen@marquette.edu joseph.loparco@marquette.edu */

#include <xinu.h>

/*------------------------------------------------------------------------
 * passwdFileRead - Read in a password file from filesystem.
 *------------------------------------------------------------------------
 */
devcall passwdFileRead(void)
{
/**
 * TODO:
 * This function opens a file called "passwd" for reading, reads in its
 * contents, and stores them into the global usertab array.
 * Steps:
 * 1) Open file "passwd", getting the file descriptor.
 * 2) Use the descriptor to seek to offset zero in the file, the beginning.
 * 3) Read in the bytes of the file using fileGetChar(), storing them
 *    into a suitable temporary location.
 * 4) Close the file.
 * 5) After checking the file contents look OK, copy over to usertab using
 *    memcpy(), and return OK.
 *
 * Errors to watch for:
 * 1) Trouble opening the passwd file.  (It may not exist.)
 *    Error text = "No passwd file found.\n"
 * 2) Trouble reading bytes from the file.  (It might be too short.)
 *    Return SYSERR.
 * 3) The contents of the file could be blank or corrupted.  Check that
 *    the first field of the first user entry is state USERUSED, and that
 *    the salt field matched the SALT constant for this version of the O/S
 *    before overwriting the contents of the existing user table.
 *    Error text = "Passwd file contents corrupted!\n".
 */

    int i, fd;
    struct userent fakeusertab[MAXUSERS];
    char *p = (char *)fakeusertab;
    fd = fileOpen("passwd");
    if(fd == SYSERR){
	printf("No passwd file found.\n");
	return SYSERR;
    }

    fileSeek(fd, 0);
    for (i = 0; (i < sizeof(usertab)); i++){
	    p[i] = fileGetChar(fd);
	if(p[i] == SYSERR){
		break;
	}
    }
    fileClose(fd);
    if (fakeusertab[SUPERUID].state == USERUSED && fakeusertab[SUPERUID].salt == SALT){
	memcpy(usertab, fakeusertab, sizeof(usertab));
    }else{
	printf("Passwd file contents corrupted!\n");
	return SYSERR;
    }
    
    return OK;
}
