#ifndef SIM_SEND_CMD_H
#define SIM_SEND_CMD_H
#include "../../libs/stm32f10x.h"
#include <string.h>
#include "../../hd44780/hd44780.h"
#include "../../kb_driver/keyboard_driver.h"
#include "../menu.h"
#include "../../delay/delay.h"
#include "../../slow_timer/slow_timer.h"
#include "../../USART/usart2.h"

struct command{
	uint8_t cmd[20];
	uint16_t timeout;
};

#define MAKE_COMMAND(name, cmd, amount) \
	command name = {(uint8_t)amount, { cmd }}\

void send_test_cmds(void);
#endif
