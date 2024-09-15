#include "drivers/fs/storage_d/d_ram.h"

u8 D_ram_Read(usize addr) {
    return *((u8*) addr);
}

void D_ram_Write(usize addr, u8 byte) {
    *((u8*) addr) = byte;
}