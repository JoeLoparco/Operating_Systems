/**
 * @file     xsh_chpass.c
 * @provides xsh_chpass
 *
 */
/* Embedded XINU, Copyright (C) 2024.  All rights reserved. */
/*Modified by Joseph Loparco                                      */
/*                                                                 */
/* and Max Steffens & Google Gemini                                */
/* Instructor Brylow                                               */
/* TA-BOT:MAILTO maxwell.steffen@marquette.edu joseph.loparco@marquette.edu */





#include <xinu.h>

/**
 * Shell command (chpass) changes an existing user password.
 * @param args array of arguments
 * @return OK for success, SYSERR for errors.
 */
command xsh_chpass(int nargs, char *args[])
{
/**
 * TODO:
 * This function creates a new password for an existing user.
 * You may break this task down into any number of new helper
 * functions within this file, and also may rely on helper functions
 * that already exist, such as getusername(), hasspassword(), and
 * passwdFileWrite().
 *
 * Steps:
 * 1) If no user name was provided to the shell command, superusr should
 *    be prompted for one.  Normal users are not prompted, because we
 *    default to changing their own password.
 *   
 * 2) Search for the user name in the usertab.
 * 3) If the current user is not superusr, prompt for the previous password.
 *    Prompt text = "Enter previous password for user %s: ".
 * 4) If the hash of the previous password matched what is on record,
 *    prompt for new password.
 *    Prompt test = "Enter new password for user %s: ".
 * 5) Place the new password hash into the user entry, and commit to disk.
 * 6) Printf "Successfully changed password for user ID %d\n" with user ID.
 *
 * Errors to watch for:
 * 1) There is not already a user logged in.
 *    Error text = "Must login first\n".
 * 2) The logged in userid is not SUPERUID, but is trying to change someone
 *    else's password.
 *    Error text = "ERROR: Only superusr can change other passwords!\n".
 * 3) The given user name cannot be found in the existing user table.
 *    Error text = "User name %s not found.\n".   
 * 4) The password change failed.  (i.e., passwords didn't match.)
 *    Error text = "Password for user %s does not match!\n".
 */

   //int c;
   //int i = 0;   
   //int pp = 0;
   int wantedUserId;
   char username[MAXUSERLEN + 1];
   //bzero(username, MAXUSERLEN + 1);
   if (userid < SUPERUID) //Checks if a valid user is logged in
    {
        printf("Must login first\n");
    }
    
   if(nargs < 2){ // checks to see if username was provided in command
	 if(userid == SUPERUID){ // checks if admin is current user
		printf("Please enter desired username:\n");
		getusername(username, MAXUSERLEN);
	 }else{
		strncpy(username, usertab[userid].username, MAXUSERLEN);
	        username[MAXUSERLEN + 1] = '\0';
		//username = usertab[userid].username;
		// if user is not admin we defualt to changing thier own pass
	}
   }
   else{ // username was provided as second arg 
	if(userid == SUPERUID){
		strncpy(username, args[1], MAXUSERLEN);
		username[MAXUSERLEN + 1] = '\0';
		//username = args[1] username is equal to second comand line arg whcih is where we expect username to be 
  	}else{
		printf("ERROR: Only superusr can change other passwords!\n");
		return SYSERR;
	}
   }
   // look for desired username in usertable 
   if(searchname(username) == SYSERR){
	printf("User name %s mot found", username);
	return SYSERR;
   }else{
   	wantedUserId = searchname(username); 
   }
   char currusrname;
   char newpass[MAXPASSLEN + 1];

   if(userid != SUPERUID){ // checks if current user is admin and requests password if !admin
	ulong passhash; 
	printf("Enter previous password for user %s: ", username);
        passhash = hashpassword(usertab[wantedUserId].salt);
        if(usertab[wantedUserId].passhash == passhash){
		printf("Enter new password for user %s: ", username);
		usertab[wantedUserId].passhash = hashpassword(usertab[wantedUserId].salt);	
		passwdFileWrite();
	}
	else{
		printf("Password for user %s does not match", username);
		return SYSERR;
	}
   }else{
	printf("Enter new password for user %s: ", username);
        usertab[wantedUserId].passhash = hashpassword(usertab[wantedUserId].salt);
	passwdFileWrite();
	  }
   printf("Successfully changed password for user ID %d\n ", wantedUserId);
    	
    return OK;
}
