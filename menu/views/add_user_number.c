#include "add_user_number.h"

	const uint8_t alphabet[12][char_per_btn]={
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
	};

void add_user_number(void){
	button *btn_obj;
	uint8_t btn, btn_old=0, duration=0, press_counter=0;

	uint8_t c_position=0, final_char;
	uint32_t time_after_press=0;

		lcd_clrscr();
		LCDPrintS("Enter caller #1");
		lcd_goto(2, 0);
		turn_on_cursor();
	do{

		btn_obj = get_btn();
		btn = btn_obj->button;

		if(btn){
			time_after_press = 0; //Clean time counter after every button press
			if(btn_old == btn){
				if(press_counter == get_vars_amount(btn)-1)press_counter=0;
				else press_counter++; //If button pressed not first time, increment counter
			}
			if(press_counter == char_per_btn-1)press_counter=0; //get chars by circle
			if(btn_old != btn){
				if(btn_old != 0)time_after_press = TIME_AFTER_PRESS; //if not first time
				else{
					press_counter = 0;
					btn_old = btn;
					duration = btn_obj->duration;
				}
			}
			temp_lcd_show(btn_old, duration, press_counter);
		}

		if(btn_old){ //If some button was pressed
			//If time after button pressed has passed or if button doesn't have variants or if exit
			if(time_after_press == TIME_AFTER_PRESS || !has_variants(btn_old) || is_exit(btn_old, duration, press_counter)){
				time_after_press=0;
				final_char = get_symbol(btn_old, duration, press_counter);
				btn_old = 0;

				switch (final_char){
				case exit_symb:
					turn_off_cursor();
					changeMenu(MENU_THIS);
					return;
					break;
				case space_symb:
					if(c_position < 15){
						cursor_shift(RIGHT);
						c_position++;
					}
					break;
				case clean_char_symb:
					if(c_position > 0){
						cursor_shift(LEFT);
						lcd_putc(' ');
						cursor_shift(LEFT);
						c_position--;
					}
					break;
				default:
					if(c_position < 15){
						lcd_putc(final_char);
						c_position++;
					}
				}
			}
			time_after_press++;
		}

	}while (1);
}

uint8_t get_symbol(uint8_t btn, uint8_t duration, uint8_t pressed_cnt){
	uint8_t line_num;
	uint8_t el_num;

	line_num = get_line(btn);

	el_num = pressed_cnt;
	if(duration == '1'){
		el_num = char_per_btn-1;
	}

	if(alphabet[line_num][el_num] == 0)el_num=0; //If there is no char for such pressed btn
	return alphabet[line_num][el_num];
}

uint8_t temp_lcd_show(uint8_t btn, uint8_t duration, uint8_t pressed_cnt){
	uint8_t symbol;

	symbol = get_symbol(btn, duration, pressed_cnt);
	switch (symbol){
	case clean_char_symb:
		break;
	case exit_symb:
		break;
	default:
		lcd_putc(get_symbol(btn, duration, pressed_cnt));
		cursor_shift(LEFT);
		break;
	}
}

uint8_t has_variants(uint8_t btn){
	uint8_t line_num;
	uint8_t chars_amount=0;

	line_num = get_line(btn);
	for(uint8_t i=1;i<char_per_btn - 1;i++){
		if(alphabet[line_num][i] != 0){
			return 1;
		}
	}
	return 0;
}

uint8_t get_line(uint8_t btn){
	uint8_t line_num;

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
	return line_num;
}

uint8_t get_vars_amount(uint8_t btn){
	uint8_t line;
	uint8_t cnt=1;

	line = get_line(btn);
	for(uint8_t i=1;i<char_per_btn;i++){
		if(alphabet[line][i] != 0)cnt++;
		else return cnt;
	}
}

uint8_t is_exit(uint8_t btn, uint8_t duration, uint8_t pressed_cnt){
	if(get_symbol(btn, duration, pressed_cnt) == exit_symb) return 1;
	return 0;
}

