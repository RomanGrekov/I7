#include "menu.h"

menuItem* selectedMenuItem;
menuItem	NULL_ENTRY = {(void*)0, (void*)0, (void*)0, (void*)0, NULL_HANDLER, 0, {0x00}};
uint8_t menu_changed=0;
/*
//		  name    next	prev	parent		child	handler   view
MAKE_MENU(main_menu,  modem, modem,  NULL_ENTRY, modem, NULL_HANDLER, 0, "   Main screen");
MAKE_MENU(modem,  user_settings, test1,  main_menu, modem_on, NULL_HANDLER, 0, "Modem menu");
MAKE_MENU(user_settings,  test1, modem,  main_menu, users_phones, NULL_HANDLER, 0, "User settings");
MAKE_MENU(test1,  modem, user_settings,  main_menu, NULL_ENTRY, test_menu, 1, "test1");

MAKE_MENU(modem_on,  modem_cmd, modem_cmd,  modem, NULL_ENTRY, turn_on_off, 1, "Modem on/off");
MAKE_MENU(modem_cmd,  modem_on, modem_on,  modem, NULL_ENTRY, send_test_cmds, 1, "Modem commands");
MAKE_MENU(users_phones,  users_phones, users_phones,  user_settings, user_phone1, NULL_HANDLER, 0, "Users phones");

MAKE_MENU(at_cmd,  ata_cmd, ata_cmd,  modem_cmd, NULL_ENTRY, NULL_HANDLER, 1, "at command");
MAKE_MENU(ata_cmd,  at_cmd, at_cmd,  modem_cmd, NULL_ENTRY, NULL_HANDLER, 1, "ata command");
MAKE_MENU(user_phone1,  user_phone2, user_phone2,  users_phones, NULL_ENTRY, add_user1_number, 1, "Number 1");
MAKE_MENU(user_phone2,  user_phone1, user_phone1,  users_phones, NULL_ENTRY, add_user2_number, 1, "Number 2");
*/
MAKE_MENU(main_menu,  test1, test1,  NULL_ENTRY, test1, NULL_HANDLER, 0, "   Main screen");
MAKE_MENU(test1,  test1, test1,  main_menu, NULL_ENTRY, test_menu, 1, "test1");

void NULL_HANDLER(void){
}

uint8_t is_in_menu(void){
	if(selectedMenuItem->Parent == &NULL_ENTRY) return 0;
	return 1;
}
void InitMenu(void){
	selectedMenuItem = (menuItem*)&main_menu;
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

	if (btn != 0){
		if (SysConf.state == DISARM){
			view = GetCurMenuView();//Get view FLAG
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
						callView();
					}
					break;
				default:
					set_state_menu(btn, duaration);
					break;
				}
			}
			if(MenuChanged()){
				if(is_in_menu()){
					lcd_clrscr();
					LCDPrintS(GetCurMenuName());
				}
			}
		else{
			set_state_menu(btn, duaration);
		}
	}
}

void callView(){
	selectedMenuItem->handler();
}
