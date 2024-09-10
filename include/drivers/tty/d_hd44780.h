#ifndef _d_hd44780_h
#define _d_hd44780_h

#include <avr/io.h>

#ifndef LCD_DIR
#   define LCD_DIR DDRB
#endif

#ifndef LCD_PORT
#   define LCD_PORT PORTB
#endif

#ifndef RS
#   define RS PB0
#endif

#ifndef EN
#   define EN PB1
#endif

typedef enum {
    LCD_CLEAR           = 0x01,
    LCD_CURSOR_TO_HOME  = 0x02,
    LCD_MOVE_TO_LEFT    = 0x04,
    LCD_MOVE_TO_RIGHT   = 0x06,
    LCD_DISABLE_DISPLAY = 0x08,

    LCD_DISABLE_CURSOR     = 0x0c,
    LCD_SQUARE_CURSOR      = 0x0d,
    LCD_LINE_CURSOR        = 0x0e,
    LCD_LINE_SQUARE_CURSOR = 0x0f,

    LCD_1L4B_MODE = 0x20,
    LCD_2L4B_MODE = 0x28,
    LCD_1L8B_MODE = 0x30,
    LCD_2L8B_MODE = 0x38,

    LCD_CHARGEN_RAM_BEGIN = 0x40,
    LCD_CHARGEN_RAM_END   = 0x7f,

    LCD_CURSORPOS_BEGIN = 0x80,
    LCD_CURSORPOS_END   = 0xcf
} lcd_command_t;

#endif