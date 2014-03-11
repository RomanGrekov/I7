#include "test_menu.h"

void test_menu(void){

uint8_t my_alphabet[]={
              ///0   1   2   3   4   L
                '0', 0 , 0 , 0 , 0 ,' ',
                '1','_', 0 , 0 , 0 , 0,
                '2','a','b','c', 0 , 0,
                '3','d','e','f', 0 , 0,
                '4','g','h','i', 0 , 0,
                '5','j','k','l', 0 , 0,
                '6','m','n','o', 0 , 0,
                '7','p','q','r','s', 0,
                '8','t','u','v', 0 , 0,
                '9','w','x','y','z', 0,
                '<', 0 , 0 , 0 , 0 ,'~',
                '*','+', 0 , 0 , 0 ,'^'};

EditorConf config = {
	.top_line="Test me!!!",
    .clean_char_symb = '<',
    .space_symb = ' ',
    .exit_symb_ok = '^',
    .exit_symb_discard  ='~',
    .x_size = 6,
    .y_size = 12,
    .alphabet=my_alphabet,
    .resp_size=20,
    .do_exit_on_max_resp=0,
    .old_response="",
	.mask=0};

	button *btn_obj;
	uint8_t status=0;

	init_editor(config);

	do{

		btn_obj = get_btn();
		status = typing(btn_obj);

	}while (status == 0);

	if(status != 0){
		USART2SendStr(get_text());
	}
}
