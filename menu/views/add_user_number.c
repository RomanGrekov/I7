#include "add_user_number.h"

#define clean_char_symb '<'
#define space_symb ' '
#define exit_symb '^'
#define x_size 2
#define y_size 12
const uint8_t alphabet_[y_size][x_size]={
	   ///0   L
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
		{clean_char_symb, 0 },
		{'+',exit_symb},
};

void add_user_number(void){
	button *btn_obj;
	uint8_t resp[20];

		lcd_clrscr();
		LCDPrintS("Enter caller #1");
		lcd_goto(2, 0);
		turn_on_cursor();

		response_init(resp, 20);
		alphabet_init(alphabet_, y_size, x_size);
		management_btns_init(clean_char_symb, space_symb, exit_symb);
	do{

		btn_obj = get_btn();

	}while (typing(btn_obj));
	USART2SendStr(resp);
}

