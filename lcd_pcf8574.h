#ifndef __LCD_PCF8574__
#define __LCD_PCF8574__

#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include <string.h>

/** Chieu gui I2C toi PCF8574 la tu P7 - P0.
                Ma P7 ------- D7
                         P6 ------- D6
                         P5 ------- D5
                         P4 ------- D4
                         P3 ------- BL
                         P2 ------- E
                         P1 ------- RW
                         P0 ------- RS
*/

#define PCF8574_I2C_ADDRESS 0x27
/** Function table */

/* Fucntion with RS and RW == 0 */
#define LCD_CMD_CLS_DP 0x01
#define LCD_CMD_RETURN_HOME 0x02 /* Bit cuoi Dont care */
#define LCD_CMD_ENTRY_SET 0x04
/** Hai bit cuoi can duoc tuy chinh
 *  Bit 7 I/D: Increment/Decre
 *  Bit 8: S: Shift/No shift
 */
#define LCD_CMD_DPS_CTL 0x08
/** Ba bit cuoi la tuong ung voi
 *  D0 - B (Blinking): Nhay con tro
 *	 D1 - C (Cursor): Bat tat con tro
 *  D2 - D (Display): Bat/tat man hinh
 */
/* Func nay chua dung toi */
#define LCD_CMD_SHIFT 0x10
/** Can set bit 5 va 6. Bit 7 8 dont care
 *  Bit 5 (S/C): Screen/Cursor
 *  Bit 6 R/L: Right/Left
 */
#define LCD_CMD_FUNC_SET 0x20
/** Bit 4 5 6 can set.
 * Bit 4: DL (8bit/4bit) Data length
 * Bit 5: N Number of display line (2-line/1-line)
 * Bit 6: F Font type (5x10 dots/5x8 dots)
 */
#define LCD_FUNC_SET_2_LINES 1
#define LCD_FUNC_SET_1_LINES 0
#define LCD_FUNC_SET_5x10_DOTS 1
#define LCD_FUNC_SET_5x8_DOTS 0

#define LCD_CMD_SET_CGRAM 0x40 /* Bit 3 4 5 6 7 8 se lan luot la AC5 - AC0 */
#define LCD_CMD_SET_DDRAM 0x80 /* Bit 2 3 4 5 6 7 8 se la luot la AC6 - AC0 */

#define LCD_MODE_CONFIG 0x00
#define LCD_MODE_DATA 0x01
#define LCD_MODE_READ 0x02
#define LCD_MODE_WRITE 0x00
#define LCD_ENABLE 0x04
#define LCD_BACKLIGHT_ON 0x08

/* DDRAM address */
#define DDRAM_START_LINE1 0x00
#define DDRAM_START_LINE2 0x40
#define DDRAM_END_LINE1 0x27
#define DDRAM_END_LINE2 0x67

/* Command with RS/RW != 0 */
/* RS = 1, RW =0 */
// #define LCD_CMD_WRITE_RAM 0x0

struct __cursorPosition {
  uint8_t row;
  uint8_t col;
};

typedef struct __LCD_HandleTypeDef {
  uint8_t display;
  uint8_t cursor;
  uint8_t blinking;
  uint8_t data_length;
  uint8_t dp_line;
  uint8_t font;
  uint8_t increment;
  uint8_t char_shift;
  uint8_t address;
  uint8_t backlight;
  I2C_HandleTypeDef *i2c_interface;
  struct __cursorPosition cursorPosition;
} LCDHandleType;

/* Write string to LCD device */
int LCD_Write(LCDHandleType *handle, char *string);

/* Initialize the LCD device */
int LCD_init(LCDHandleType *handle);

/* Clear all character on the screen */
int LCD_ClearDP(LCDHandleType *handle);

/* Bring cursor home */
int LCD_ReturnHome(LCDHandleType *handle);

/* Turn the cursor on/off */
int LCD_CursorOnOff(LCDHandleType *handle, uint8_t status);

/* Turn the display on/off */
int LCD_DisplayOnOff(LCDHandleType *handle, uint8_t status);

/* Add default value to the LCD object */
int new_LCD_object(LCDHandleType *lcd, uint8_t address,
                   I2C_HandleTypeDef *i2c_instant);

/* Change the number of memory line displayed */
int LCD_ChangeDisplayLines(LCDHandleType *handle, uint8_t status);

/* Change font size */
int LCD_ChangeFont(LCDHandleType *handle, uint8_t font);

/* Turn Cursor blinking on/off */
int LCD_SetBlinking(LCDHandleType *handle, uint8_t status);

/* Move cursor to the destinated point */
int LCD_moveCursor(LCDHandleType *lcd, int row, int col);

/* Unused funtions
int write8Bits (LCDHandleType *lcd, uint8_t data, uint8_t mode);
int writeOneByte(LCDHandleType *lcd, uint8_t mode, uint8_t data); */

/* For testing only */
int LCD_Write_Char(LCDHandleType *lcd, char theone);

#endif
