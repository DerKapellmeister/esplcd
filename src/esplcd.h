#ifndef ESPLCD_ESPLCD_H
#define ESPLCD_ESPLCD_H

#include <os_type.h>
#include <osapi.h>
#include <i2c_master.h>

#define I2C_MASTER_SDA_MUX PERIPHS_IO_MUX_GPIO4_U
#define I2C_MASTER_SCL_MUX PERIPHS_IO_MUX_GPIO5_U
#define I2C_MASTER_SDA_GPIO 4
#define I2C_MASTER_SCL_GPIO 5
#define I2C_MASTER_SDA_FUNC FUNC_GPIO4
#define I2C_MASTER_SCL_FUNC FUNC_GPIO5

typedef struct {
		// 1 = enabled; 0 = disabled
		uint8_t backlight;

		// i2c address of PCF8574
		uint8_t address;

} lcd_settings;



void ICACHE_FLASH_ATTR lcdWriteCMD(uint8_t cmd, lcd_settings *settings);
void ICACHE_FLASH_ATTR lcdClearScreen(lcd_settings *settings);
void ICACHE_FLASH_ATTR lcdWriteString(char *str, lcd_settings *settings);
void ICACHE_FLASH_ATTR lcdInit(lcd_settings *settings);
void ICACHE_FLASH_ATTR lcdSetCursor(uint8_t x, uint8_t y, lcd_settings *settings);
void ICACHE_FLASH_ATTR lcdUpdateBacklight(lcd_settings *settings);



#endif //ESPLCD_ESPLCD_H
