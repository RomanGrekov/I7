#ifndef ADD_USER_NUMBER_H
#define ADD_USER_NUMBER_H
#include "../../libs/stm32f10x.h"
#include <string.h>
#include "../../hd44780/hd44780.h"
#include "../../kb_driver/keyboard_driver.h"
#include "../menu.h"
#include "../../delay/delay.h"

void add_user_number(void);
uint8_t get_symbol(uint8_t btn, uint8_t duration, uint8_t amount);
#endif
