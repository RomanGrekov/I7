#ifndef ENTER_USR_PWD_H
#define ENTER_USR_PWD_H
#include "../../libs/stm32f10x.h"
#include <string.h>
#include "../../hd44780/hd44780.h"
#include "../../kb_driver/keyboard_driver.h"
#include "../menu.h"
#include "../../delay/delay.h"
#include "../../globs.h"
#include "../../text_editor/text_editor.h"
#include "../../flash/flash.h"

uint8_t enter_usr_pwd(void);
#endif
