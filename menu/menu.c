#include "menu.h"

menuItem* selectedMenuItem;
menuItem	NULL_ENTRY = {(void*)0, (void*)0, (void*)0, (void*)0, 0, {0x00}};
uint8_t menu_changed;

	MAKE_MENU(m_11,  m_12, m_13,  NULL_ENTRY, NULL_ENTRY, 0, "Start");
	MAKE_MENU(m_12,  m_13, m_11,  NULL_ENTRY, NULL_ENTRY, 0, "Setup");
	MAKE_MENU(m_13,  m_11, m_12,  NULL_ENTRY, NULL_ENTRY, 1, "Reset");

void InitMenu(void){

	menu_changed = 1;
	selectedMenuItem = (menuItem*)&m_11;
}

uint8_t* GetCurMenuName(void){
	menu_changed = 0;
	return selectedMenuItem->Text;
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
	}
	if ((void*)new_menu == (void*)&NULL_ENTRY)
	  return;

	selectedMenuItem = new_menu;
}
