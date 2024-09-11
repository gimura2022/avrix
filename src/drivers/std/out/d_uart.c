#include "drivers/std/d_out.h"
#include "drivers/d_uart.h"

FILE d_out = FDEV_SETUP_STREAM(D_OUT_PutChar, NULL, _FDEV_SETUP_WRITE);

void D_OUT_Init(void) {
    D_UART_Init();
}

void D_OUT_PutChar(char c, FILE* stream) {
    if (c == '\n') D_OUT_PutChar('\r', stream);
    D_UART_PutChar(c);
}