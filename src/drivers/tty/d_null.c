#include "drivers/d_tty.h"

FILE d_tty_out = FDEV_SETUP_STREAM(D_TTY_PutChar, NULL, _FDEV_SETUP_WRITE);
FILE d_tty_in  = FDEV_SETUP_STREAM(NULL, D_TTY_GetChar, _FDEV_SETUP_READ);

void D_TTY_Init(void) {}
void D_TTY_PutChar(char c, FILE* stream) {}
char D_TTY_GetChar(FILE* stream) {}