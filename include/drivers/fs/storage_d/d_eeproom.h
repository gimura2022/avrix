#ifndef _d_eeproom_h
#define _d_eeproom_h

#include "u_common.h"

u8   D_eeproom_Read(usize addr);
void D_eeproom_Write(usize addr, u8 byte);

#endif