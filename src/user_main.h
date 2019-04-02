#ifndef USER_USER_MAIN_H_
#define USER_USER_MAIN_H_

#include <osapi.h>
#include <os_type.h>
#include <c_types.h>
#include <user_interface.h>
#include "esplcd.h"


#define SEC_TASK_PERIOD	  10000 //ms

static void ICACHE_FLASH_ATTR myTask(void *arg);
void ICACHE_FLASH_ATTR my_init(void);

#endif /* USER_USER_MAIN_H_ */
