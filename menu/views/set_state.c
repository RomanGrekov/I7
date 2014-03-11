#include "set_state.h"
//#include "enter_usr_pwd.h"
#include "../../flash/flash.h"
#include "../../security/security.h"

void set_state_menu(uint8_t btn, uint8_t duaration){

	switch (btn){
		case '*':
			SysConf.alarm_in_memory=0;
			SysConf.state=DISARM;
			break;
		case '0':
			SysConf.state=HOME;
			break;
		case '#':
			if(enter_usr_pwd() == 1)SysConf.state=AWAY;
			else return;
			break;
	}

	check_state();
	flash_write_struct(params_addr, &SysConf, sizeof(SysConf));

}
