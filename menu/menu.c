#include "menu.h"

menuItem	NULL_ENTRY = {(void*)0, (void*)0, (void*)0, (void*)0, 0, {0x00}};
void menuChange(menuItem* NewMenu)
{
	if ((void*)NewMenu == (void*)&NULL_ENTRY)
	  return;

	selectedMenuItem = NewMenu;
}

	MAKE_MENU(m_s1i1,  m_s1i2, m_s1i3,  NULL_ENTRY, NULL_ENTRY, 0, "Start");
	MAKE_MENU(m_s1i2,  m_s1i3, m_s1i1,  NULL_ENTRY, NULL_ENTRY, 0, "Setup");
	MAKE_MENU(m_s1i3,  m_s1i1, m_s1i2,  NULL_ENTRY, NULL_ENTRY, MENU_RESET, "Reset");

void InitMenu(void){

	selectedMenuItem = (menuItem*)&m_s1i1;
}

uint8_t* GetCurMenuName(void){
	return selectedMenuItem->Text;
}
/*
menu my_menu;
	menu_item item_main;
	menu_item item_1;
	menu_item item_2;
	menu_item item1_1;

void InitMenu(void)
{
	set_name(&item_main, "Main menu\0");
	set_parent(&item_main, &item_main);
	set_next(&item_main, &item_1);
	set_prev(&item_main, &item_2);
	set_child(&item_main, &item1_1);

	set_name(&item_1,"Second menu\0");
	set_parent(&item_1, &item_1);
	set_next(&item_1, &item_2);
	set_prev(&item_1, &item_main);

	set_name(&item_2,"Third menu\0");
	set_parent(&item_2, &item_2);
	set_next(&item_2, &item_main);
	set_prev(&item_2, &item_1);


	set_name(&item1_1, "2nd floar menu\0");
	set_parent(&item1_1, &item_main);
	set_next(&item1_1, &item1_1);
	set_prev(&item1_1, &item1_1);

	my_menu.cur_item = &item_main;
	my_menu.changed = 1;
}

uint8_t* GetCurMenuName(void)
{
	menu_item *m;
	m = my_menu.cur_item;
	my_menu.changed = 0;
	return m->name;
}

uint8_t MenuChanged(void)
{
	return my_menu.changed;
}

void NextItem(void)
{
	menu_item *m;
	m = my_menu.cur_item;
	my_menu.cur_item = m->next;
	my_menu.changed = 1;
}

void PrevItem(void)
{
	menu_item *m;
	m = my_menu.cur_item;
	my_menu.cur_item = m->prev;
	my_menu.changed = 1;
}

void UpItem(void)
{
	menu_item *m;
	m = my_menu.cur_item;
	my_menu.cur_item = m->parent;
	my_menu.changed = 1;
}

void DownItem(void)
{
	menu_item *m;
	m = my_menu.cur_item;
	my_menu.cur_item = m->child;
	my_menu.changed = 1;
}

void set_name(menu_item *item, uint8_t *name)
{
	uint8_t i=0;
	while(*name && i < sizeof(item->name))
	{
		item->name[i] = name[i];
		i++;
	}
}
void set_parent(menu_item *item, menu_item *parent)
{
	item->parent = parent;
}

void set_next(menu_item *item, menu_item *next)
{
	item->next = next;
}

void set_prev(menu_item *item, menu_item *prev)
{
	item->prev = prev;
}

void set_child(menu_item *item, menu_item *child)
{
	item->child = child;
}
*/
