#include "drivers/std/d_in.h"

FILE d_in = FDEV_SETUP_STREAM(NULL, D_IN_GetChar, _FDEV_SETUP_READ);

void D_IN_Init(void) {}
char D_IN_GetChar(FILE* stream) {}