#include "drivers/d_tty.h"
#include "drivers/tty/d_hd44780.h"

#include "u_common.h"

#include <util/delay.h>

FILE d_tty_out = FDEV_SETUP_STREAM(D_TTY_PutChar, NULL, _FDEV_SETUP_WRITE);
FILE d_tty_in  = FDEV_SETUP_STREAM(NULL, D_TTY_GetChar, _FDEV_SETUP_READ); 

static void D_TTY_LCD_Command(u8 cmd);
static void D_TTY_LCD_SendChar(u8 c);

static u8 charpos NO_INIT;

void D_TTY_Init(void) {
    LCD_DIR = 0xff;
    _delay_ms(20);

    D_TTY_LCD_Command(LCD_CURSOR_TO_HOME);
    charpos = 0;
    D_TTY_LCD_Command(LCD_2L4B_MODE);
    D_TTY_LCD_Command(LCD_DISABLE_CURSOR);
    D_TTY_LCD_Command(LCD_MOVE_TO_RIGHT);
    D_TTY_LCD_Command(LCD_CLEAR);
    _delay_ms(2);
}

void D_TTY_PutChar(char c, FILE* stream) {
    charpos++;
    switch (c) {
        case '\n':
            D_TTY_PutChar('\r', stream);
            D_TTY_LCD_Command(LCD_CURSORPOS_BEGIN + 16);
            charpos = 16;
            return;

        case '\r':
            D_TTY_LCD_Command(LCD_CURSORPOS_BEGIN + (charpos / 16) * 16);
            charpos = (charpos / 16) * 16;
            return;

        default:
            break;
    }

    D_TTY_LCD_SendChar(c);
}

char D_TTY_GetChar(FILE* stream) {}

static void D_TTY_LCD_Command(u8 cmd) {
    LCD_PORT  = (LCD_PORT & 0x0f) | (cmd & 0xf0);
    LCD_PORT &= ~(1 << RS);
    LCD_PORT |=  (1 << EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN);

    _delay_us(200);

    LCD_PORT  = (LCD_PORT & 0x0f) | (cmd << 4);
    LCD_PORT |= (1 << EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN);
    _delay_ms(2);
}

static void D_TTY_LCD_SendChar(u8 c) {
    LCD_PORT  = (LCD_PORT & 0x0f) | (c & 0xf0);
    LCD_PORT |= (1 << RS);
    LCD_PORT |= (1 << EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN);

    _delay_us(200);

    LCD_PORT  = (LCD_PORT & 0x0f) | (c << 4);
    LCD_PORT |= (1 << EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN);
    _delay_ms(2);
}