#include "drivers/std/d_in.h"
#include "drivers/d_uart.h"

FILE d_in = FDEV_SETUP_STREAM(NULL, D_IN_GetChar, _FDEV_SETUP_READ);

void D_IN_Init(void) {
    D_UART_Init();
}

char D_IN_GetChar(FILE* stream) {
    return D_UART_GetChar();
}