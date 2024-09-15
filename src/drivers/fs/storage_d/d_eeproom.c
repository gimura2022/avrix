#include "drivers/fs/storage_d/d_eeproom.h"

#include <avr/eeprom.h>

u8 D_eeproom_Read(usize addr) {
    return eeproom_read_byte(addr);
}

void D_eeproom_Write(usize addr, u8 byte) {
    eeproom_write_byte(addr, byte);
}