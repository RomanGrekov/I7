#include "sim_on_off.h"

void turn_on_off(void){
	button *btn_obj;
	uint8_t btn;
	uint8_t status[10];
	uint8_t st_flag=2, retcode=1, old_retcode=0;

	do{
		if (st_flag != SimGetStatus() || retcode != old_retcode){
			st_flag = SimGetStatus();

			if (st_flag){
				retcode = SimInit();
				if(retcode == 0)strncpy(status, "On  Ok\0", 7);
				else strncpy(status, "On Bad\0", 7);
			}
			if (!st_flag)strncpy(status, "Off   \0", 7);

			old_retcode = retcode;

			lcd_clrscr();
			LCDPrintS("Status is ");
			LCDPrintS(status);
			LCDPrintS("1-on, 3-off, *-I");
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
		case '*':
			retcode = SimInit();
		}

	}while (btn != '2');
}
