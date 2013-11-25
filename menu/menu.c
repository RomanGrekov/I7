#include "menu.h"

menuItem* selectedMenuItem;
menuItem	NULL_ENTRY = {(void*)0, (void*)0, (void*)0, (void*)0, NULL_HANDLER, 0, {0x00}};
uint8_t menu_changed;
//		  name    next	prev	parent		child	handler   view
MAKE_MENU(modem,  test, test1,  NULL_ENTRY, modem_on, NULL_HANDLER, 0, "Modem menu");
MAKE_MENU(test,  test1, modem,  NULL_ENTRY, NULL_ENTRY, NULL_HANDLER, 0, "test");
MAKE_MENU(test1,  modem, test,  NULL_ENTRY, NULL_ENTRY, NULL_HANDLER, 0, "test1");

MAKE_MENU(modem_on,  modem_cmd, modem_cmd,  modem, NULL_ENTRY, turn_on_off, 1, "Modem on/off");
MAKE_MENU(modem_cmd,  modem_on, modem_on,  modem, NULL_ENTRY, send_test_cmds, 1, "Modem commands");

MAKE_MENU(at_cmd,  ata_cmd, ata_cmd,  modem_cmd, NULL_ENTRY, NULL_HANDLER, 1, "at command");
MAKE_MENU(ata_cmd,  at_cmd, at_cmd,  modem_cmd, NULL_ENTRY, NULL_HANDLER, 1, "ata command");

void NULL_HANDLER(void){

}
void InitMenu(void){

	menu_changed = 1;
	selectedMenuItem = (menuItem*)&modem;
}

uint8_t* GetCurMenuName(void){
	menu_changed = 0;
	return selectedMenuItem->Text;
}
uint8_t GetCurMenuView(void){
	return selectedMenuItem->View;
}

uint8_t MenuChanged(void){
	return menu_changed;
}

void changeMenu(uint8_t el_num)
{
	menuItem *new_menu;

	menu_changed = 1;

	switch (el_num){
	case 1:
		new_menu = selectedMenuItem->Next;
	break;
	case 2:
		new_menu = selectedMenuItem->Previous;
	break;
	case 3:
		new_menu = selectedMenuItem->Child;
	break;
	case 4:
		new_menu = selectedMenuItem->Parent;
	break;
	case 5:
		new_menu = selectedMenuItem->This;
	break;
	}
	if ((void*)new_menu == (void*)&NULL_ENTRY)
	  return;

	selectedMenuItem = new_menu;
}

void ProcessMenu(uint8_t btn, uint8_t duaration){

	uint8_t view;

	view = GetCurMenuView();//Get view FLAG
	if (btn != 0){
		switch (btn){
			case '6':
				changeMenu(MENU_NEXT);
				break;
			case '4':
				changeMenu(MENU_PREVIOUS);
				break;
			case '2':
				changeMenu(MENU_PARENT);
				break;
			case '8':
				if(!view){// If menu doesn't have view go to the child
					changeMenu(MENU_CHILD);
				}
				else{// If view exists, call it
					selectedMenuItem->handler();
				}
				break;
		}
	}
	if(MenuChanged()){
		lcd_clrscr();
		LCDPrintS(GetCurMenuName());
	}
}

