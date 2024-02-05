/**
 * @file testcases.c
 * @provides testcases
 *
 * Modified by:
 *
 * and
 *
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

/**
 * testcases - called after initialization completes to test things.
 */



void delay(int seconds) {
    volatile unsigned long wait;
    for (int sec = 0; sec < seconds; sec++) {
        for (wait = 0; wait < 100000000; wait++);    }
}


void test_kputc(void) {
    kprintf("Testing kputc()\r\n");
    kputc('H');
    kputc('e');
    kputc('l');
    kputc('l');
    kputc('o');
    kputc('\r'); // Carriage return
    kputc('\n'); // New line
}

void test_kgetc(void) {
    unsigned char c;

    kprintf("Please type a character: ");
    c = kgetc();
    kprintf("\r\nYou typed: %c\r\n", c);
}

void test_kungetc(void) {
    unsigned char c = 'X';

    kungetc(c); // Pretend we've just read 'X' from the user
    kprintf("Testing kungetc(): ");
    c = kgetc(); // This should retrieve the 'X' we just "un-got"
    kprintf("%c\r\n", c);
}


void test_kcheckc(void) {
    kprintf("Testing kcheckc() - Type something within 10 seconds.\r\n");

    // Wait for up to 10 seconds for a character to be available
    for (int i = 0; i < 10; i++) {
        if (kcheckc()) {
            kprintf("Character available.\r\n");
            return; // Exit the test once a character is detected
        }
        delay(1);
    }

    kprintf("No character available after 10 seconds.\r\n");
}

void testcases(void)
{
    unsigned char c;
    unsigned char q = '0';
    kprintf("===TEST BEGIN===\r\n");
    while(q=='0'){
    kprintf("Please type a character 1-4:\r\n");
    c = kgetc();
    switch (c)
    {
    case '1':
         test_kputc();
         break;
    case '2':
         test_kgetc();
         break;
    case '3':
         test_kungetc();
         break;
    case '4':
         test_kcheckc();
         break;
        // TODO: Test your operating system!

    default:
        kprintf("No test case is assigned to that value!\r\n");
    }
    if(c==4)
    {
        q = kgetc();
    }
    kprintf("Enter 0 to test again or anything else to quit:\r\n ");
    q = kgetc();
}
    kprintf("\r\n===TEST END===\r\n");
    return;
}
~    
