#include "add_user_number.h"

#define x_size 2
#define y_size 12
const uint8_t alphabet_[y_size][x_size]={
	   ///0   1    L
		{'0', 0 },
		{'1', 0 },
		{'2', 0 },
		{'3', 0 },
		{'4', 0 },
		{'5', 0 },
		{'6', 0 },
		{'7', 0 },
		{'8', 0 },
		{'9', 0 },
		{'<', 0 },
		{'+','^'},
};

void add_user_number(void){
	button *btn_obj;

		lcd_clrscr();
		LCDPrintS("Enter caller #1");
		lcd_goto(2, 0);
		turn_on_cursor();
		editor_init(alphabet_, y_size, x_size);
	do{

		btn_obj = get_btn();

	}while (typing(btn_obj));
}

