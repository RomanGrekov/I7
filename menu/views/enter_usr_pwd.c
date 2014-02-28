#include "enter_usr_pwd.h"

uint8_t enter_usr_pwd(void){
	#define clean_char_symb ' '
	#define space_symb ' '
	#define exit_symb_discard ' '
	#define exit_symb_ok ' '
	#define x_size 2
	#define y_size 11
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
	};

	button *btn_obj;
	uint8_t status=0;
	char pwd[5]={0,0,0,0};
	uint8_t pwd_correct = 1;
	uint8_t btn_cnt=0;
	uint8_t btn;

		lcd_clrscr();
		LCDPrintS("Enter user passw");
		lcd_goto(2, 0);
		turn_on_cursor();

		response_init(pwd, 5);
		alphabet_init(alphabet_, y_size, x_size);
		management_btns_init(clean_char_symb, space_symb, exit_symb_ok, exit_symb_discard);
	do{

		btn_obj = get_btn();
		status = typing(btn_obj);
		btn = btn_obj->button;
		if(btn)btn_cnt++;
		if(btn_cnt == 4)status = 1;

	}while (status == 0);
	if(status == 1){
		for(uint8_t i=0; i<4; i++){
			if(pwd[i] != SysConf.usr_pwd[i]){
				pwd_correct = 0;
				return pwd_correct;
			}
		}
		return pwd_correct;
	}
}
