#include <xinu.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */
static unsigned char ungetArray[UNGETMAX];
unsigned int  bufp;

syscall kgetc()
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // Check the unget buffer first.
    if (bufp > 0) {
        return ungetArray[--bufp]; // Decrement bufp and return the character.
    }

    // Check if data is available in the UART.
    while (!(regptr->lsr & UART_LSR_DR)); // Wait for Data Ready condition.
    return (unsigned char)(regptr->rbr); // Read the receiver buffer register.

}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
            volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // Check if there's a character in the unget buffer.
    if (bufp > 0) {
        return TRUE;
    }

    // Check UART for an available character.
    if (regptr->lsr & UART_LSR_DR) {
        return TRUE;
    }

    return FALSE;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    // Ensure there's room in the buffer.
    if (bufp >= UNGETMAX) {
        return SYSERR; // Buffer is full.
    }

    // Store the character and increment bufp.
    ungetArray[bufp++] = c;
    return c;
}

syscall kputc(uchar c)
{
        volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    // Wait for the Transmitter Holding Register to be empty.
    while (!(regptr->lsr & UART_LSR_THRE));

    // Write the character to the transmitter holding register.
    regptr->thr = (unsigned char)c;

    return c;
}

syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(long, long))kputc, 0);
    va_end(ap);
    return retval;
}
