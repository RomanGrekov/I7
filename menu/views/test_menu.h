#ifndef TEST_MENU_H
#define TEST_MENU_H
#include "../../libs/stm32f10x.h"
#include <string.h>
#include "../../hd44780/hd44780.h"
#include "../../kb_driver/keyboard_driver.h"
#include "../menu.h"
#include "../../delay/delay.h"
#include "../../globs.h"
#include "../../text_editor/text_editor.h"
#include "../../USART/usart2.h"
#include "../../flash/flash.h"

void test_menu(void);
#endif
