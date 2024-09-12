#include "drivers/std/d_null.h"

FILE d_null = FDEV_SETUP_STREAM(D_NULL_PutChar, NULL, _FDEV_SETUP_WRITE);

void D_NULL_PutChar(char c, FILE* stream) {
    switch (c) {
        case '\n':
            D_NULL_PutChar('\r', stream);
            break;

        default:
            break;
    }
}