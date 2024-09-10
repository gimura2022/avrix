#ifndef _d_uart_h
#define _d_uart_h

#ifndef BAUD
#   define BAUD 9600
#endif

#include <stdio.h>

extern FILE d_tty_out;
extern FILE d_tty_in;

void D_TTY_Init(void);
void D_TTY_PutChar(char c, FILE* stream);
char D_TTY_GetChar(FILE* stream);

#endif