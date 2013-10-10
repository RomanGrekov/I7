#ifndef MENU_H
#define MENU_H
#include "../libs/stm32f10x.h"
#include <string.h>

typedef struct PROGMEM{
	void       *Next;
	void       *Previous;
	void       *Parent;
	void       *Child;
	uint8_t     Select;
	const char  Text[];
} menuItem;

#define MAKE_MENU(Name, Next, Previous, Parent, Child, Select, Text) \
    extern menuItem Next;     \
	extern menuItem Previous; \
	extern menuItem Parent;   \
	extern menuItem Child;  \
	menuItem Name = {(void*)&Next, (void*)&Previous, (void*)&Parent, (void*)&Child, (uint8_t)Select, { Text }}

#define PREVIOUS   ((menuItem*)pgm_read_word(&selectedMenuItem->Previous))
#define NEXT       ((menuItem*)(&selectedMenuItem->Next))
#define PARENT     ((menuItem*)pgm_read_word(&selectedMenuItem->Parent))
#define CHILD      ((menuItem*)pgm_read_word(&selectedMenuItem->Child))
#define SELECT		(pgm_read_byte(&selectedMenuItem->Select))

menuItem* selectedMenuItem;

menuItem* menuStack[10];
volatile uint8_t menuStackTop;

enum {
    MENU_CANCEL=1,
    MENU_RESET,
    MENU_MODE1,
    MENU_MODE2,
    MENU_MODE3,
    MENU_SENS1,
    MENU_SENS2,
};

void InitMenu(void);
void menuChange(menuItem* NewMenu);

/*
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

const char hi_mes[]=>>_HELLO_>>

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
*/
#endif
