#include "security.h"
#include "../globs.h"

const char text_state[3][17] = {"=====DISARM=====",
								"======HOME======",
								"======AWAY======"};

void check_state(void){
	static uint8_t old_state=4;

	if(old_state != SysConf.state){
		switch(SysConf.state){
		case DISARM:
			lcd_clrscr();
			LCDPrintS(text_state[DISARM]);
			break;
		case HOME:
			lcd_clrscr();
			LCDPrintS(text_state[HOME]);
			break;
		case AWAY:
			lcd_clrscr();
			LCDPrintS(text_state[AWAY]);
			break;
		}
		old_state=SysConf.state;
	}
}
