#include "text_editor.h"

/*
const uint8_t alphabet_full[12][char_per_btn]={
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
*/
const uint8_t *alphabet;
uint8_t x_size, y_size;
uint8_t editor_resp_size;
uint8_t *editor_resp;
uint8_t resp_ptr;
uint8_t c_position;
const uint8_t *clean_char_symb;
const uint8_t *space_symb;
const uint8_t *exit_symb_ok;
const uint8_t *exit_symb_discard;

uint8_t alphabet_pull(uint8_t line, uint8_t element){
	if(line >= y_size || element >= x_size) return 0;
	return *(alphabet + x_size * line + element);
}

void alphabet_init(const uint8_t *alphabet_, uint8_t y, uint8_t x){
	alphabet = alphabet_;
	x_size = x;
	y_size = y;
}

void response_init(uint8_t *response, uint8_t size){
	editor_resp = response;
	editor_resp_size = size;
	resp_ptr=0;
	c_position=0;
	//Show already exist response
	for(uint8_t i=0;i<size;i++){
		if(response[i] != '\0'){
			c_position++;
			response_push(response[i]);
			lcd_putc(response[i]);
		}
		else break;
	}
}

void management_btns_init(const uint8_t *clean_char_,
						  const uint8_t *space_symb_,
						  const uint8_t *exit_symb_ok_,
						  const uint8_t *exit_symb_discard_){
	clean_char_symb = clean_char_;
	space_symb = space_symb_;
	exit_symb_ok = exit_symb_ok_;
	exit_symb_discard = exit_symb_discard_;
}

void response_push(uint8_t symbol){
	if(resp_ptr < editor_resp_size){
		editor_resp[resp_ptr] = symbol;
		resp_ptr++;
	}
}

void response_rm_char(void){
	if(resp_ptr > 0){
		editor_resp[resp_ptr] = '\0';
		resp_ptr--;
		editor_resp[resp_ptr] = '\0';
	}
}

uint8_t typing(button *button_obj){
	uint8_t btn, duration=0, final_char, ACTION_FLAG=0;
	static uint8_t btn_old=0, press_counter=0;
	static uint32_t time_after_press=0;

	btn = button_obj->button;
	if(btn){
		time_after_press = 0; //Clean time counter after every button press
		if(btn_old == btn){
			if(press_counter == get_vars_amount(btn)-1)press_counter=0;
			else press_counter++; //If button pressed not first time, increment counter
		}
		//if(press_counter == x_size-1)press_counter=0; //get chars by circle
		if(btn_old != btn){
			if(btn_old != 0)time_after_press = TIME_AFTER_PRESS; //if not first time
			else{
				press_counter = 0;
				btn_old = btn;
				duration = button_obj->duration;
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
			if(final_char == exit_symb_ok){
				turn_off_cursor();
				response_push('\0');
				changeMenu(MENU_THIS);
				btn_old=0;
				press_counter=0;
				c_position=0;
				time_after_press=0;
				ACTION_FLAG=1;
				return 1;
			}
			if(final_char == exit_symb_discard){
				turn_off_cursor();
				response_push('\0');
				changeMenu(MENU_THIS);
				btn_old=0;
				press_counter=0;
				c_position=0;
				time_after_press=0;
				ACTION_FLAG=1;
				return 2;
			}
			if(final_char == space_symb){
				if(c_position >= disp_line_length){
					shift_display(LEFT);
				}
				cursor_shift(RIGHT);
				c_position++;
				response_push(' ');
				ACTION_FLAG=1;
			}
			if(final_char == clean_char_symb){
				if(c_position > disp_line_length)shift_display(RIGHT);
				if(c_position > 0){
					cursor_shift(LEFT);
					lcd_putc(' ');
					cursor_shift(LEFT);
					c_position--;
					response_rm_char();
				}
				ACTION_FLAG=1;
			}
			if(!ACTION_FLAG){
				if(c_position >= disp_line_length){
					shift_display(LEFT);
				}
					response_push(final_char);
					lcd_putc(final_char);
					c_position++;
			}
		}
		time_after_press++;
	}
	return 0;
}

uint8_t get_symbol(uint8_t btn, uint8_t duration, uint8_t pressed_cnt){
	uint8_t line_num;
	uint8_t el_num;

	line_num = get_line(btn);

	el_num = pressed_cnt;
	if(duration == '1'){
		el_num = x_size-1;
	}

	if(alphabet_pull(line_num,el_num) == 0)el_num=0; //If there is no char for such pressed btn
	return alphabet_pull(line_num,el_num);
}

void temp_lcd_show(uint8_t btn, uint8_t duration, uint8_t pressed_cnt){
	uint8_t symbol;

	symbol = get_symbol(btn, duration, pressed_cnt);
	if(symbol != exit_symb_ok && symbol != clean_char_symb){
		lcd_putc(get_symbol(btn, duration, pressed_cnt));
		cursor_shift(LEFT);
	}
}

uint8_t has_variants(uint8_t btn){
	uint8_t line_num;
	uint8_t chars_amount=0;

	line_num = get_line(btn);
	for(uint8_t i=1;i<x_size - 1;i++){
		if(alphabet_pull(line_num,i) != 0){
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
	for(uint8_t i=1;i<x_size;i++){
		if(alphabet_pull(line,i) != 0)cnt++;
		else return cnt;
	}
	return cnt;
}

uint8_t is_exit(uint8_t btn, uint8_t duration, uint8_t pressed_cnt){
	if(get_symbol(btn, duration, pressed_cnt) == exit_symb_ok) return 1;
	return 0;
}

