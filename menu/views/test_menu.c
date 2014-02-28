#include "test_menu.h"

void test_menu(void){
EditorConf config = {.clean_char_symb = '<',
.space_symb = ' ',
.exit_symb_ok = '^',
.exit_symb_discard  ='~',
.x_size = 6,
.y_size = 12,
.alphabet={
        							   ///0   1   2	  3   4   L
                                        {'0', 0 , 0 , 0 , 0 ,' '},
                                        {'1','_', 0 , 0 , 0 , 0 },
                                        {'2','a','b','c', 0 , 0 },
                                        {'3','d','e','f', 0 , 0 },
                                        {'4','g','h','i', 0 , 0 },
                                        {'5','j','k','l', 0 , 0 },
                                        {'6','m','n','o', 0 , 0 },
                                        {'7','p','q','r','s', 0 },
                                        {'8','t','u','v', 0 , 0 },
                                        {'9','w','x','y','z', 0 },
                                        {'<', 0 , 0 , 0 , 0 ,'#'},
                                        {'*','+', 0 , 0 , 0 ,'^'},
        },
.resp_size = 15,
.response={"hello"}};

	button *btn_obj;
	uint8_t status=0;

		lcd_clrscr();
		LCDPrintS("Test editor");
		lcd_goto(2, 0);
		turn_on_cursor();
		init_editor(config);

	do{

		btn_obj = get_btn();
		status = typing(btn_obj);

	}while (status == 0);
	if(status == 1){
		//USART2SendStr(resp);

	}
}
