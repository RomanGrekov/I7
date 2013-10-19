#include "sim_on_off.h"

void turn_on_off(void){
	button *btn_obj;
	uint8_t btn;
	uint8_t status[10];
	uint8_t st_flag=2;

	do{
		if (st_flag != SimGetStatus()){
			st_flag = SimGetStatus();

			if (st_flag)strncpy(status, "On    \0", 7);
			if (!st_flag)strncpy(status, "Off   \0", 7);


			lcd_clrscr();
			LCDPrintS("Status is ");
			LCDPrintS(status);
			LCDPrintS("1 - on, 3 - off");
		}

		btn_obj = get_btn();
		btn = btn_obj->button;
		switch (btn){
		case '2':
			changeMenu(MENU_THIS);
			break;
		case '1':
			SwitchSim900(1, 5);
			break;
		case '3':
			SwitchSim900(0, 5);
			break;
		}

	}while (btn != '2');
}
