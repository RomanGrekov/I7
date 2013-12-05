#include "main_screen.h"

void main_screen(void){
	button *btn_obj;
	uint8_t btn;

	LCDPrintS("Alarm system v");

	do{

		btn_obj = get_btn();
		btn = btn_obj->button;
		switch (btn){
		case '8':
			changeMenu(MENU_THIS);
			break;
		case '6':
			changeMenu(MENU_NEXT);
			break;
		case '4':
			changeMenu(MENU_PREVIOUS);
			break;
		case '2':
			changeMenu(MENU_THIS);
			break;
		}

	}while (btn != '2');
}
