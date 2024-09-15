#ifndef _d_ram_h
#define _d_ram_h

#include "u_common.h"

u8   D_ram_Read(usize addr);
void D_ram_Write(usize addr, u8 byte);

#endif