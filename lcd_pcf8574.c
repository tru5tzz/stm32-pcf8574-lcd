#include "lcd_pcf8574.h"
#include "stm32f1xx_hal.h"

#ifdef DEBUG 
	/* Test DEBUG define */
#endif

/** Tao mot struct LCDHandleType voi cac gia tri mac dinh */
int new_LCD_object(LCDHandleType *lcd, uint8_t address, I2C_HandleTypeDef *i2c_instant) {
	lcd->display = 0x04;
	lcd->cursor = 0x00;
	lcd->blinking = 0x00;
	lcd->data_length = 0x00;
	lcd->dp_line = 0x04;
	lcd->font = 0x00;
	lcd->increment = 0x02;
	lcd->char_shift = 0x00;
	lcd->address = address;
	lcd->i2c_interface = i2c_instant;
	lcd->backlight = 0x08;
	
	return 0;
}

/* Low level I2C write to device
data pointer nhan vao array gom 2 phan tu co kieu du lieu uint8_t

*/
int sendToDevice (LCDHandleType *lcd, uint8_t *data) {
	uint8_t va[4], i = 0;
	va[0] = data[0] | LCD_ENABLE;
	va[1] = data[0] | ~LCD_ENABLE;
	va[2] = data[1] | LCD_ENABLE;
	va[3] = data[1] | ~LCD_ENABLE;
	
	SEND:;
	uint8_t ret = HAL_I2C_Master_Transmit(lcd->i2c_interface, lcd->address, va, 4, 100);
	
	if (ret != HAL_OK) {
		i++;
		if (i == 3) {
			return HAL_ERROR;
		}
		goto SEND;
	}
	
	return 0;
}

/* Function to add RW, RW, BL bit */
/* int addControlBit (LCDHandleType *lcd, uint8_t *data, uint8_t mode) {
	for (int i = 0; i < sizeof(data); i++) {
		data[i] = data[i] | lcd->backlight | mode;
	}
	
	sendToDevice(lcd, data);
	
	return 0;
} */

/** Make 2 8-bit package from D7-D0
		Add RW, RS, BL bit
		Seperate into 2 8-bit package
		Send to device
*/
int write8Bits (LCDHandleType *lcd, uint8_t data, uint8_t mode) {
	uint8_t va[2], ret;
	va[0] = data & 0xF0;
	va[1] = (data & 0x0F) << 4;
	
	for (int i = 0; i < 2; i++) {
		va[i] |= (lcd->backlight | mode);
	}
	
	ret = sendToDevice(lcd, va);
	
	return ret;
}

/** Turn on or off display
Arg: status --- 1 for on, 0 for off
*/
int LCD_DisplayOnOff (LCDHandleType *handle, uint8_t status) {
	uint8_t mode = LCD_MODE_CONFIG | LCD_MODE_WRITE;
	if (status) {
		handle->display = 0x04;
	}
	else {
		handle->display = 0x00;
	}
	
	uint8_t value = LCD_CMD_DPS_CTL | handle->display | handle->cursor | handle->blinking;
	write8Bits(handle, value, mode);
	
	return 0;
}

/** Turn cursor blinking on or off
Arg: status --- 1 for on, 0 for off
*/
int LCD_SetBlinking (LCDHandleType *handle, uint8_t status) {
	if (status) {
		handle->blinking = 0x01;
	} else {
		handle->blinking = 0x00;
	}
	
	uint8_t mode = LCD_MODE_CONFIG | LCD_MODE_WRITE;
	
	uint8_t value = LCD_CMD_DPS_CTL | handle->display | handle->cursor | handle->blinking;
	write8Bits(handle, value, mode);
	
	return 0;
}

/** Turn cursor on or off
Arg: status --- 1 for on, 0 for off
*/
int LCD_CursorOnOff (LCDHandleType *handle, uint8_t status) {
	if (status) {
		handle->cursor = 0x02;
	}
	else {
		handle->cursor = 0x00;
	}
	
	uint8_t mode = LCD_MODE_CONFIG | LCD_MODE_WRITE;
	
	uint8_t value = LCD_CMD_DPS_CTL | handle->display | handle->cursor | handle->blinking;
	write8Bits(handle, value, mode);
	
	return 0;
}

/** Clear screen
Arg: none
*/
int LCD_ClearDP(LCDHandleType *handle){
	uint8_t value, mode;
	
	mode = LCD_MODE_CONFIG | LCD_MODE_WRITE;
	
	value = LCD_CMD_CLS_DP;
	
	write8Bits(handle, value, mode);
	
	HAL_Delay(5);
	
	return 0;
}

/** Bring cursor back to home (first DDRAM)
Arg: None
*/
int LCD_ReturnHome (LCDHandleType *handle) {
	uint8_t value, mode;
	
	mode = LCD_MODE_CONFIG | LCD_MODE_WRITE;
	
	value = LCD_CMD_RETURN_HOME;
	
	write8Bits(handle, value, mode);
	handle->cursorPosition.row = 1;
	handle->cursorPosition.col = 1;
	HAL_Delay(5);
	
	return 0;
}

/* Set Entry, default setting */
int LCD_SetEntry (LCDHandleType *lcd) {
	uint8_t value, mode;
	
	mode = LCD_MODE_CONFIG | LCD_MODE_WRITE;
	
	value = LCD_CMD_ENTRY_SET | lcd -> increment | lcd -> char_shift;
	
	write8Bits(lcd, value, mode);
	
	return 0;
}

/** Change number of line to display
Arg: 1 for 2 line, 0 for 1 line
Recommend: 1
*/
int LCD_ChangeDisplayLines (LCDHandleType *handle, uint8_t status) {
	if (status == LCD_FUNC_SET_2_LINES) {
		handle->dp_line = 0x08;
	}
	else if (status == LCD_FUNC_SET_1_LINES) {
		handle->dp_line = 0x00;
	}
	
	uint8_t mode, value;
	mode = LCD_MODE_CONFIG | LCD_MODE_WRITE;
	
	value = LCD_CMD_FUNC_SET | handle -> data_length | handle -> dp_line | handle -> font;
	
	write8Bits(handle, value, mode);
	
	return 0;
}

/** Change the font size of the charactor
Arg: font --------------------- 1 for 5x10 dots, 0 for 5x8 dots
Recommend: 0
*/
int LCD_ChangeFont (LCDHandleType *handle, uint8_t font) {
	if (font == LCD_FUNC_SET_5x10_DOTS) {
		handle -> font = 0x04;
	}
	else if (font == LCD_FUNC_SET_5x8_DOTS) {
		handle -> font = 0x00;
	}
	
	uint8_t mode, value;
	mode = LCD_MODE_CONFIG | LCD_MODE_WRITE;
	value = LCD_CMD_FUNC_SET | handle -> data_length | handle -> dp_line | handle -> font;
	
	write8Bits(handle, value, mode);
	
	return 0;
}

int LCD_init (LCDHandleType *lcd) {
	uint8_t value, mode;
	value = 0x00;
	HAL_I2C_Master_Transmit(lcd->i2c_interface, lcd->address, &value, 1, 100);
	
	HAL_Delay(45);
	
	/* Set 4 bit mode */
	mode = LCD_MODE_CONFIG | LCD_MODE_WRITE;
	lcd->data_length = 0x10;
	value = (LCD_CMD_FUNC_SET | lcd->data_length | lcd->dp_line | lcd->font) & 0xF0;
	value |= mode;
	
	uint8_t va[2];
	va[0] = value | LCD_ENABLE;
	va[1] = value;
	for (int i = 0; i < 3; i++) {
		HAL_I2C_Master_Transmit(lcd->i2c_interface, lcd->address, va, 2, 100);
		HAL_Delay(5);
	}
	
	lcd->data_length = 0x00;
	value =(LCD_CMD_FUNC_SET | lcd->data_length | lcd->dp_line | lcd->font) &0xF0;
	value |= mode;
	va[0] = value | LCD_ENABLE;
	va[1] = value;
	HAL_I2C_Master_Transmit(lcd->i2c_interface, lcd->address, va, 2, 100);

	/* Display clear */
	LCD_DisplayOnOff(lcd, 0);
	LCD_ClearDP(lcd);
	
	/* Display setting */
	LCD_SetBlinking(lcd, 0);
	LCD_CursorOnOff(lcd, 0);
	LCD_SetEntry(lcd);
	LCD_ChangeDisplayLines(lcd, LCD_FUNC_SET_2_LINES);
	LCD_ChangeFont(lcd, LCD_FUNC_SET_5x8_DOTS);
	
	LCD_DisplayOnOff(lcd, 1);
	
	lcd->cursorPosition.row = 1;
	lcd->cursorPosition.col = 1;
	
	return 0;
}

int LCD_Write(LCDHandleType *lcd, char *string) {
	
	uint16_t string_length = strlen(string);
	while(*string) {
		write8Bits(lcd, *string, LCD_MODE_DATA | LCD_MODE_WRITE);
		string++;
	}
	lcd->cursorPosition.col += string_length;
	if (lcd->cursorPosition.col > 20) {
		lcd->cursorPosition.row++;
		lcd->cursorPosition.col--;
	}
	return 0;
}

int LCD_Write_Char(LCDHandleType *lcd, char theone) {
	write8Bits(lcd, theone, LCD_MODE_DATA | LCD_MODE_WRITE);
	
	return 0;
}

int LCD_moveCursor(LCDHandleType *lcd, int row, int col) {
	if (col > 20 | row > 4) {
		return 1;
	}
	int value;
	switch (row)
   {
    case 1:
			value = col - 1;
    	break;
    case 2:
			value = col - 1 + 64;
   		break;
		case 3:
			value = col + 20 - 1;
			break;
		case 4:
			value = col - 1 + 84;
			break;
   	default:
   		break;
   }
	
	write8Bits(lcd, value | LCD_CMD_SET_DDRAM, LCD_MODE_CONFIG | LCD_MODE_WRITE);
	
	lcd->cursorPosition.row = row;
	lcd->cursorPosition.col = col;
	return 0;
}
