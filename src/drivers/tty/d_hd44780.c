#include "drivers/d_tty.h"
#include "drivers/video/d_hd44780.h"

#include "u_common.h"

FILE d_tty_out = FDEV_SETUP_STREAM(D_TTY_PutChar, NULL, _FDEV_SETUP_WRITE);
FILE d_tty_in  = FDEV_SETUP_STREAM(NULL, D_TTY_GetChar, _FDEV_SETUP_READ); 

static u8 line NO_INIT;

void D_TTY_Init(void) {
    D_VID_TEXT_hd44780_Init();
    line = 0;
}

void D_TTY_PutChar(char c, FILE* stream) {
    switch (c) {
        case '\n':
            line = line + 1 > 1 ? 0 : line;
            D_VID_TEXT_hd44780_Command(LCD_CURSORPOS_BEGIN + 16 * line);
            return;

        case '\r':
            D_VID_TEXT_hd44780_Command(LCD_CURSORPOS_BEGIN + 16 * line);
            return;

        default:
            break;
    }

    D_VID_TEXT_hd44780_SendChar(c);
}

char D_TTY_GetChar(FILE* stream) {}