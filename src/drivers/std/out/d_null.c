#include "drivers/std/d_out.h"

FILE d_out = FDEV_SETUP_STREAM(D_OUT_PutChar, NULL, _FDEV_SETUP_WRITE);

void D_OUT_Init(void) {}

void D_OUT_PutChar(char c, FILE* stream) {
    switch (c) {
        case '\n':
            D_OUT_PutChar('\r', stream);
            break;

        default:
            break;
    }
}