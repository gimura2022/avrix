#include "drivers/d_tty.h"

#include <avr/io.h>
#include <util/setbaud.h>

FILE d_tty_out = FDEV_SETUP_STREAM(D_TTY_PutChar, NULL, _FDEV_SETUP_WRITE);
FILE d_tty_in  = FDEV_SETUP_STREAM(NULL, D_TTY_GetChar, _FDEV_SETUP_READ);

void D_TTY_Init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#   if USE_2X
    UCSR0A |= _BV(U2X0);
#   else
    UCSR0A &= ~(_BV(U2X0));
#   endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
}

void D_TTY_PutChar(char c, FILE* stream) {
    if (c == '\n') D_TTY_PutChar('\r', stream);

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

char D_TTY_GetChar(FILE* stream) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}