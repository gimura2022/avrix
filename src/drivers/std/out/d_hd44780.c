#include "drivers/std/d_out.h"
#include "drivers/d_hd44780.h"

#include "u_common.h"

FILE d_out = FDEV_SETUP_STREAM(D_OUT_PutChar, NULL, _FDEV_SETUP_WRITE);

static u8 line NO_INIT;

void D_OUT_Init(void) {
    D_hd44780_Init();
    line = 0;
}

void D_OUT_PutChar(char c, FILE* stream) {
    switch (c) {
        case '\n':
            line = line + 1 > 1 ? 0 : line;
            D_hd44780_Command(LCD_CURSORPOS_BEGIN + 16 * line);
            return;

        case '\r':
            D_hd44780_Command(LCD_CURSORPOS_BEGIN + 16 * line);
            return;

        default:
            break;
    }

    D_hd44780_SendChar(c);
}