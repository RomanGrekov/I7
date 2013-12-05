#include "add_user_number.h"

	#define char_per_btn 6
	const uint8_t alphabet[12][char_per_btn]={
		   ///0   1   2	  3   4   L
			{'0', 0 , 0 , 0 , 0 ,'_'},
			{'1', 0 , 0 , 0 , 0 ,' '},
			{'2','a','b','c', 0 , 0 },
			{'3','d','e','f', 0 , 0 },
			{'4','g','h','i', 0 , 0 },
			{'5','j','k','l', 0 , 0 },
			{'6','m','n','o', 0 , 0 },
			{'7','p','q','r','s', 0 },
			{'8','t','u','v', 0 , 0 },
			{'9','w','x','y','z', 0 },
			{'#', 0 , 0 , 0 , 0 ,'<'},
			{'*', 0 , 0 , 0 , 0 ,'+'},
	};

void add_user_number(void){
	button *btn_obj;
	uint8_t btn, btn_old=0, duration_old=0, press_counter=0;

	uint8_t c_position=0, final_char;
	uint32_t time_after_press=0;

		lcd_goto(2, 0);
		turn_on_cursor();
	do{

		btn_obj = get_btn();
		btn = btn_obj->button;

		if(btn){
			if(btn_old == btn)press_counter++;
			if(btn_old != btn){
				press_counter = 0;
				btn_old = btn;
				duration_old = btn_obj->duration;
			}
			time_after_press = 0;
		}

		if(btn_old){
			time_after_press++;
			if(time_after_press == 80000){
				time_after_press=0;
				final_char = get_symbol(btn_old, duration_old, press_counter);
				btn_old = 0;

				switch (final_char){
				case ' ':
					if(c_position < 15){
						cursor_shift(RIGHT);
						c_position++;
					}
					else{
						lcd_goto(2, 0);
						c_position=0;
					}
					break;
				case '<':
					if(c_position > 0){
						cursor_shift(LEFT);
						c_position--;
					}
					else{
						lcd_goto(2, 15);
						c_position=15;
					}
					break;
				default:
					lcd_putc(final_char);
					c_position++;
				}
			}
		}

	}while (btn != '2');
}

uint8_t get_symbol(uint8_t btn, uint8_t duration, uint8_t amount){
	uint8_t line_num;
	uint8_t el_num;
	uint8_t res_char;

	switch(btn){
	case '#':
		line_num = btn - 25;
		break;
	case '*':
		line_num = btn - 31;
		break;
	default:
		line_num = btn - 48;
		break;
	}

	el_num = amount;
	if(duration == '1'){
		el_num = char_per_btn;
	}

	if(el_num >= char_per_btn)res_char = alphabet[line_num][0];
	else res_char = alphabet[line_num][el_num];
	return res_char;
}
