#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>

#ifndef BAUD
#   define BAUD 9600
#endif
#include <util/setbaud.h>

#include "drivers/d_uart.h"

void D_UART_Init(void) {
    static bool is_inited = false;
    if (is_inited) return;

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#   if USE_2X
    UCSR0A |= _BV(U2X0);
#   else
    UCSR0A &= ~(_BV(U2X0));
#   endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);

    is_inited = true;
}

void D_UART_PutChar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

char D_UART_GetChar(void) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}