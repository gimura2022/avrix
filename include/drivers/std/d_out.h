#ifndef _d_out_h
#define _d_out_h

#include <stdio.h>

extern FILE d_out;

void D_OUT_Init(void);
void D_OUT_PutChar(char c, FILE* stream);

#endif