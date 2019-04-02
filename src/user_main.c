#include "osapi.h"
#include "user_main.h"

static os_timer_t taskTmr;
static lcd_settings lcd1;


uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void) {
	enum flash_size_map size_map = system_get_flash_size_map();
	uint32 rf_cal_sec = 0;

	switch (size_map) {
		case FLASH_SIZE_4M_MAP_256_256:
			rf_cal_sec = 128 - 5;
			break;

		case FLASH_SIZE_8M_MAP_512_512:
			rf_cal_sec = 256 - 5;
			break;

		case FLASH_SIZE_16M_MAP_512_512:
		case FLASH_SIZE_16M_MAP_1024_1024:
			rf_cal_sec = 512 - 5;
			break;

		case FLASH_SIZE_32M_MAP_512_512:
		case FLASH_SIZE_32M_MAP_1024_1024:
			rf_cal_sec = 1024 - 5;
			break;

		case FLASH_SIZE_64M_MAP_1024_1024:
			rf_cal_sec = 2048 - 5;
			break;
		case FLASH_SIZE_128M_MAP_1024_1024:
			rf_cal_sec = 4096 - 5;
			break;
		default:
			rf_cal_sec = 0;
			break;
	}
	return rf_cal_sec;
}

void ICACHE_FLASH_ATTR my_init(void) {

	// Testing

	os_timer_disarm(&taskTmr);
	os_timer_setfn(&taskTmr, (os_timer_func_t *) myTask, (void *) 0);
	os_timer_arm(&taskTmr, SEC_TASK_PERIOD, 0);
	//os_timer_arm(&taskTmr, SEC_TASK_PERIOD, 1);

	lcd1.backlight = 0;
	lcd1.address = 0x27;

	lcdInit(&lcd1);

	lcdSetCursor(0, 0, &lcd1);
	lcdWriteString("Row #1", &lcd1);
	lcdSetCursor(0, 1, &lcd1);
	lcdWriteString("Row #2", &lcd1);
	lcdSetCursor(0, 2, &lcd1);
	lcdWriteString("Row #3", &lcd1);
	lcdSetCursor(0, 3, &lcd1);
	lcdWriteString("Row #4", &lcd1);

}


void user_init(void) {

	system_init_done_cb(my_init);

}


static void ICACHE_FLASH_ATTR myTask(void *arg) {

	lcd1.backlight ^= 0x01;
	lcdUpdateBacklight(&lcd1);

	lcdClearScreen(&lcd1);
	lcdWriteString("Test", &lcd1);

}