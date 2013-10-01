#ifndef MENU_H
#define MENU_H
#include "../libs/stm32f10x.h"
#include <string.h>

typedef struct{
	uint32_t *child;
	struct menu_item *parent;
	struct menu_item *next;
	struct menu_item *prev;
	uint8_t name[17];
} menu_item;

typedef struct{
	struct menu_item *cur_item;
	uint8_t changed;
} menu;

void InitMenu(void);
uint8_t * GetCurMenuName(void);
void NextItem(void);
void PrevItem(void);
void DownItem(void);
void UpItem(void);
void set_parent(menu_item *item, menu_item *parent);
void set_next(menu_item *item, menu_item *next);
void set_name(menu_item *item, uint8_t *name);
void set_prev(menu_item *item, menu_item *prev);
void set_child(menu_item *item, menu_item *child);
uint8_t MenuChanged(void);

#endif
