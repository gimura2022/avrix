#ifndef _d_hd44780_h
#define _d_hd44780_h

#include <avr/io.h>

#include "u_common.h"

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

#define D_VID_TEXT_hd44780_ClearDisplay()        D_VID_TEXT_hd44780_Command(LCD_CLEAR)
#define D_VID_TEXT_hd44780_CursorToHome()        D_VID_TEXT_hd44780_Command(LCD_CURSOR_TO_HOME)
#define D_VID_TEXT_hd44780_MoveCursorToLeft()    D_VID_TEXT_hd44780_Command(LCD_MOVE_TO_LEFT)
#define D_VID_TEXT_hd44780_MoveCursorToRight()   D_VID_TEXT_hd44780_Command(LCD_MOVE_TO_RIGHT)
#define D_VID_TEXT_hd44780_DisableDisplay()      D_VID_TEXT_hd44780_Command(LCD_DISABLE_DISPLAY)
#define D_VID_TEXT_hd44780_SetDisabledCursor()   D_VID_TEXT_hd44780_Command(LCD_DISABLE_CURSOR)
#define D_VID_TEXT_hd44780_SetSquareCursor()     D_VID_TEXT_hd44780_Command(LCD_SQUARE_CURSOR)
#define D_VID_TEXT_hd44780_SetLineCursor()       D_VID_TEXT_hd44780_Command(LCD_LINE_CURSOR)
#define D_VID_TEXT_hd44780_SetLineSquareCursor() D_VID_TEXT_hd44780_Command(LCD_LINE_SQUARE_CURSOR)
#define D_VID_TEXT_hd44780_Set1Line4BitsMode()   D_VID_TEXT_hd44780_Command(LCD_1L4B_MODE)
#define D_VID_TEXT_hd44780_Set2Line4BitsMode()   D_VID_TEXT_hd44780_Command(LCD_2L4B_MODE)
#define D_VID_TEXT_hd44780_Set1Line8BitsMode()   D_VID_TEXT_hd44780_Command(LCD_1L8B_MODE)
#define D_VID_TEXT_hd44780_Set2Line8BitsMode()   D_VID_TEXT_hd44780_Command(LCD_2L8B_MODE)

void D_VID_TEXT_hd44780_Init(void);
void D_VID_TEXT_hd44780_Command(u8 cmd);
void D_VID_TEXT_hd44780_SendChar(u8 c);

#endif