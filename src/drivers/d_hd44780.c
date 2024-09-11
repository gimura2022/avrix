#include "drivers/d_hd44780.h"

#include <util/delay.h>

void D_hd44780_Init(void) {
    LCD_DIR = 0xff;
    _delay_ms(20);

    D_hd44780_CursorToHome();
    D_hd44780_Set2Line4BitsMode();
    D_hd44780_SetDisabledCursor();
    D_hd44780_MoveCursorToRight();
    D_hd44780_ClearDisplay();

    _delay_ms(2);
}

void D_hd44780_Command(u8 cmd) {
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

void D_hd44780_SendChar(u8 c) {
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