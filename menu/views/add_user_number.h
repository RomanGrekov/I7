#ifndef ADD_USER_NUMBER_H
#define ADD_USER_NUMBER_H
#include "../../libs/stm32f10x.h"
#include <string.h>
#include "../../hd44780/hd44780.h"
#include "../../kb_driver/keyboard_driver.h"
#include "../menu.h"
#include "../../delay/delay.h"
#include "../../globs.h"

#define TIME_AFTER_PRESS F_CPU/400
#define clean_char_symb '<'
#define space_symb ' '
#define exit_symb '^'
#define char_per_btn 6

void add_user_number(void);
uint8_t get_symbol(uint8_t btn, uint8_t duration, uint8_t pressed_cnt);
uint8_t temp_lcd_show(uint8_t btn, uint8_t duration, uint8_t pressed_cnt);
uint8_t has_variants(uint8_t btn);
uint8_t get_line(uint8_t btn);
uint8_t get_vars_amount(uint8_t btn);
uint8_t is_exit(uint8_t btn, uint8_t duration, uint8_t pressed_cnt);
#endif
