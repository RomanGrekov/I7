#ifndef SIM_SEND_CMD_H
#define SIM_SEND_CMD_H
#include "../../libs/stm32f10x.h"
#include <string.h>
#include "../../hd44780/hd44780.h"
#include "../../kb_driver/keyboard_driver.h"
#include "../../sim900/sim900.h"
#include "../menu.h"
#include "../../globs.h"

void send_test_cmds(void);
#endif
