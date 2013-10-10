#ifndef MENU_H
#define MENU_H
#include "../libs/stm32f10x.h"
#include <string.h>
#include "../hd44780/hd44780.h"

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

enum {
    MENU_NEXT=1,
    MENU_PREVIOUS,
    MENU_CHILD,
    MENU_PARENT,
};

void InitMenu(void);
void menuChange(menuItem* NewMenu);
void changeMenu(uint8_t el_num);
uint8_t* GetCurMenuName(void);

#endif
