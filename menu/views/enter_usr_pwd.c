#include "enter_usr_pwd.h"

uint8_t enter_usr_pwd(void){
uint8_t my_alphabet[]={
              ///0    L
                '0',
                '1',
                '2',
                '3',
                '4',
                '5',
                '6',
                '7',
                '8',
                '9'};

EditorConf config = {
	.top_line="Enter password",
    .clean_char_symb = 0,
    .space_symb = 0,
    .exit_symb_ok = 0,
    .exit_symb_discard = 0,
    .x_size = 1,
    .y_size = 10,
    .alphabet=my_alphabet,
    .resp_size=4,
    .do_exit_on_max_resp=1,
    .old_response="",
	.mask=0};

	button *btn_obj;
	uint8_t status=0;
	uint8_t *got_pwd;

	init_editor(config);

	do{
		btn_obj = get_btn();
		status = typing(btn_obj);
	}while (status == 0);

	if(status == 1){
		got_pwd = get_text();
		for(uint8_t i=0; i<4; i++){
			if(got_pwd[i] != SysConf.usr_pwd[i]){
				return 0;
			}
		}
		return 1;
	}
}
