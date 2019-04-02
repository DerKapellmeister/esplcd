#include "esplcd.h"

#define LCD_CMD 0x00
#define LCD_DATA 0x01

#define LCD_HOME 0x02
#define LCD_CLEAR 0x01

#define LCD_PULSE 500
#define LCD_CMD_DELAY 4100

#define I2C_WRITE 0x00
#define I2C_READ 0x01

uint8_t _transm_started = 0;

// Start a new i2c transaction
uint8_t ICACHE_FLASH_ATTR _i2c_start(uint8_t addr, uint8_t rw_mode) {

	// Shift Address and set RW Mode
	addr = ((addr << 1) | rw_mode);

	i2c_master_start();
	i2c_master_writeByte(addr);

	_transm_started = (uint8_t) i2c_master_checkAck();
	return _transm_started;

}

// Stop i2c transaction
void ICACHE_FLASH_ATTR _i2c_stop() {
	if (_transm_started) {
		i2c_master_stop();
		_transm_started = 0;
	}
}

// write single byte to the i2c bus (call _i2c_start beforhand!)
void ICACHE_FLASH_ATTR _i2c_writeData(uint8_t i2c_data) {

	if (_transm_started) {
		i2c_master_writeByte(i2c_data);
		i2c_master_checkAck();
	}

}

// write byte to i2c bus and delay afterward for us_delay microseconds
void ICACHE_FLASH_ATTR _i2c_writeDataDelay(uint8_t i2c_data, uint16_t us_delay) {

	_i2c_writeData(i2c_data);
	ets_delay_us(us_delay);

}

// write a string of char's to the lcd
void ICACHE_FLASH_ATTR lcdWriteString(char *str, lcd_settings *settings) {

	uint8_t c, data;

	_i2c_start(settings->address, I2C_WRITE);

	while (*str) {

		c = (uint8_t) *str;
		data = (uint8_t) ((c & 0xf0) | settings->backlight << 3 | LCD_DATA);
		_i2c_writeDataDelay((uint8_t) (data | 0x04), LCD_PULSE);
		_i2c_writeDataDelay(data, LCD_CMD_DELAY);

		data = (uint8_t) ((c << 0x04) | settings->backlight << 3 | LCD_DATA);
		_i2c_writeDataDelay((uint8_t) (data | 0x04), LCD_PULSE);
		_i2c_writeDataDelay(data, LCD_CMD_DELAY);

		str++;
	}

	_i2c_stop();

}

// Clear all contents from LCD and move cursor to 0,0
void ICACHE_FLASH_ATTR lcdClearScreen(lcd_settings *settings) {

	lcdWriteCMD(LCD_CLEAR, settings);

}

// Move cursor to position x,y
void ICACHE_FLASH_ATTR lcdSetCursor(uint8_t x, uint8_t y, lcd_settings *settings) {

	x %= 0x14;

	switch (y) {
		case 2:
			x += 0x14;
			break;

		case 1:
			x += 0x40;
			break;

		case 3:
			x += 0x54;
			break;

		default:
			break;
	}

	lcdWriteCMD((uint8_t) (x | (1 << 7)), settings);

}

// Update the backlight state of the LCD corresponding to settings->backlight (0 or 1)
void ICACHE_FLASH_ATTR lcdUpdateBacklight(lcd_settings *settings) {
	lcdWriteCMD(0x00, settings);
}

// Send Command to the LCD
void ICACHE_FLASH_ATTR lcdWriteCMD(uint8_t cmd, lcd_settings *settings) {

	uint8_t cmd_nib;

	_i2c_start(settings->address, I2C_WRITE);

	// Most siginificant Nibble
	cmd_nib = (uint8_t) ((cmd & 0xf0) | settings->backlight << 3 | LCD_CMD);
	_i2c_writeDataDelay((uint8_t) (cmd_nib | 0x04), LCD_PULSE);
	_i2c_writeDataDelay(cmd_nib, LCD_CMD_DELAY);

	// Least siginificant Nibble
	cmd_nib = (uint8_t) (LCD_CMD | settings->backlight << 3 | (cmd << 0x04));
	_i2c_writeDataDelay((uint8_t) (cmd_nib | 0x04), LCD_PULSE);
	_i2c_writeDataDelay(cmd_nib, LCD_CMD_DELAY);

	_i2c_stop();

}

// Initialize the LCD
void ICACHE_FLASH_ATTR lcdInit(lcd_settings *settings) {

	i2c_master_gpio_init();
	i2c_master_init();
	ets_delay_us(50000);

	lcdWriteCMD(LCD_HOME, settings); // Set 4-bit mode of the LCD controller
	lcdWriteCMD(0x28, settings); // 2 lines, 5x8 dot matrix, 4 bit
	lcdWriteCMD(0x0c, settings); // cursor off, display on
	lcdWriteCMD(0x06, settings); // no scrolling, increment cursor right
	lcdSetCursor(0, 0, settings);

}