#ifndef MENU_H
#define MENU_H
#include "../libs/stm32f10x.h"
#include <string.h>
#include "../hd44780/hd44780.h"
#include "../kb_driver/keyboard_driver.h"
#include "views/sim_on_off.h"
#include "views/sim_send_cmd.h"

typedef struct PROGMEM{
	void       *Next;
	void       *Previous;
	void       *Parent;
	void       *Child;
	void	   (*handler)();
	uint8_t     Select;
	const char  Text[16];
	void       *This;
} menuItem;

#define MAKE_MENU(Name, Next, Previous, Parent, Child, handler, Select, Text) \
    extern menuItem Next;     \
	extern menuItem Previous; \
	extern menuItem Parent;   \
	extern menuItem Child;  \
	menuItem Name = {(void*)&Next, (void*)&Previous, (void*)&Parent, (void*)&Child, (void*)&handler, (uint8_t)Select, { Text }, (void*)&Name}

enum {
    MENU_NEXT=1,
    MENU_PREVIOUS,
    MENU_CHILD,
    MENU_PARENT,
    MENU_THIS,
};

void InitMenu(void);
void menuChange(menuItem* NewMenu);
void changeMenu(uint8_t el_num);
uint8_t* GetCurMenuName(void);
void ProcessMenu(uint8_t btn, uint8_t duaration);
uint8_t GetCurMenuView(void);
void NULL_HANDLER(void);

#endif
