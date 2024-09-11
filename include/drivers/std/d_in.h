#ifndef _d_in_h
#define _d_in_h

#include <stdio.h>

extern FILE d_in;

void D_IN_Init(void);
char D_IN_GetChar(FILE* stream);

#endif