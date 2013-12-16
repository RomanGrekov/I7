#include "add_user_number.h"

#define clean_char_symb '<'
#define space_symb ' '
#define exit_symb_ok '^'
#define exit_symb_discard '~'
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
		{clean_char_symb, exit_symb_discard},
		{'+',exit_symb_ok},
};

void add_user_number(void){
	button *btn_obj;
	uint8_t status=0;
	struct SavedDomain Conf;
	flash_read_struct(&Conf);

		lcd_clrscr();
		LCDPrintS("Enter caller #1");
		lcd_goto(2, 0);
		turn_on_cursor();

		response_init(Conf.priv_num1, 20);
		alphabet_init(alphabet_, y_size, x_size);
		management_btns_init(clean_char_symb, space_symb, exit_symb_ok, exit_symb_discard);
	do{

		btn_obj = get_btn();
		status = typing(btn_obj);

	}while (status == 0);
	if(status == 1){
		//USART2SendStr(resp);
		flash_write_struct(params_addr, &Conf);

	}
}
