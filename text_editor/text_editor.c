#include "text_editor.h"

void display_symbol(uint8_t btn, uint8_t duration, uint8_t press_counter);
uint8_t is_service_symbol(uint8_t btn);
void acept_btn(uint8_t btn, uint8_t duration, uint8_t press_cnt);
void timer_ready(void);
void make_service(uint8_t symbol);
void delete_timer(void);
void reset_timer(void);
void lcd_show_btn(uint8_t btn, uint8_t duration, uint8_t pressed_cnt);
void clean_flags(void);
uint8_t is_max_response(void);
void response_push(uint8_t symbol);
void response_rm_char(void);
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
uint8_t c_position, press_counter=0;

uint8_t resp_ptr;
uint8_t btn_old=0;
uint8_t duration_old=0;
uint8_t timer_id=0;

EditorConf EdConf;

uint8_t typing(button *button_obj){
	uint8_t btn, duration;

	btn = button_obj->button;
	duration = button_obj->duration;

	if(btn){ //if real button pressed
		reset_timer();
		if(btn_old == btn){
			if(press_counter == get_vars_amount(btn)-1)press_counter=0;
			else press_counter++; //If button pressed not first time, increment counter
		}
		else{
			if(btn_old != 0){// If already was pressed button
				acept_btn(btn_old, duration_old, press_counter);
				delete_timer();
			}
			else{// Button was pressed first time. all before were committed
				//USART2_PutChar('A');
				btn_old = btn;
				duration_old = duration;
				timer_id = Slow_Timer_Add(tm_Once, TIME_AFTER_PRESS, timer_ready);
			}
		}
		display_symbol(btn, duration, press_counter);
	}
	if(is_max_response())return 1;
/*
	if(btn_old){ //If some button was pressed
		//If time after button pressed has passed or if button doesn't have variants or if exit
		if(time_after_press == TIME_AFTER_PRESS || !has_variants(btn_old) || is_exit(btn_old, duration, press_counter)){
			time_after_press=0;
			final_char = get_symbol(btn_old, duration, press_counter);
			btn_old = 0;
			if(final_char == EdConf.exit_symb_ok){
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
			if(final_char == EdConf.exit_symb_discard){
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
			if(final_char == EdConf.space_symb){
				if(c_position >= disp_line_length){
					shift_display(LEFT);
				}
				cursor_shift(RIGHT);
				c_position++;
				response_push(' ');
				ACTION_FLAG=1;
			}
			if(final_char == EdConf.clean_char_symb){
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
	*/
	return 0;
}

void delete_timer(void){
	if(timer_id != 0){
		Slow_Timer_Delete(timer_id);
		timer_id=0;
	}
}

void reset_timer(void){
	if(timer_id != 0){
		Slow_Timer_Modify(timer_id, tm_Not_Change, 1, 0);
	}
}

void timer_ready(void){
	acept_btn(btn_old, duration_old, press_counter);
	timer_id = 0;
	return;
}

void display_symbol(uint8_t btn, uint8_t duration, uint8_t press_counter){
	uint8_t symbol;
	symbol = get_symbol(btn_old, duration_old, press_counter);
	if(! is_service_symbol(symbol))lcd_show_btn(btn, duration, press_counter); //if not service btn
}

void acept_btn(uint8_t btn, uint8_t duration, uint8_t press_cnt){
	uint8_t symbol;
	symbol = get_symbol(btn_old, duration_old, press_counter);

	if(is_service_symbol(symbol)){
		make_service(symbol);
        clean_flags();
		return;
	}

	response_push(symbol);
	cursor_shift(RIGHT);
	clean_flags();
}

void clean_flags(void){
	btn_old = 0;
	duration_old=0;
	press_counter = 0;
	timer_id = 0;
}

void make_service(uint8_t symbol){
	if(symbol == EdConf.clean_char_symb){
		if(c_position > disp_line_length)shift_display(RIGHT);
        if(c_position > 0){
        	cursor_shift(LEFT);
        	lcd_putc(' ');
        	cursor_shift(LEFT);
        	c_position--;
        	response_rm_char();
        }
	}
}

uint8_t get_symbol(uint8_t btn, uint8_t duration, uint8_t pressed_cnt){
	uint8_t line_num;
	uint8_t el_num;
	uint8_t symbol;

	line_num = get_line(btn); //Get line from alphabet

	el_num = pressed_cnt;
	if(duration == '1'){ //If it was long button press than take last element
		el_num = get_vars_amount(btn)-1;
	}

	symbol = alphabet_pull(line_num,el_num);
	if(symbol == 0)el_num=0; //If there is no char for such pressed btn
	return symbol;
}

void lcd_show_btn(uint8_t btn, uint8_t duration, uint8_t pressed_cnt){
	lcd_putc(get_symbol(btn, duration, pressed_cnt));
	cursor_shift(LEFT);
	c_position++;
}

uint8_t is_service_symbol(uint8_t btn){
	if(btn == EdConf.exit_symb_ok)return 1;
	if(btn == EdConf.exit_symb_discard)return 1;
	if(btn == EdConf.clean_char_symb)return 1;
	return 0;
}

uint8_t has_variants(uint8_t btn){
	uint8_t line_num;
	uint8_t chars_amount=0;

	line_num = get_line(btn);
	for(uint8_t i=1; i<max_alph_x-1; i++){
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
	uint8_t cnt=0;

	line = get_line(btn);
	for(uint8_t i=0; i< max_alph_x; i++){
		if(alphabet_pull(line,i) != 0)cnt++;
		else return cnt;
	}
	return cnt;
}

uint8_t is_exit(uint8_t btn, uint8_t duration, uint8_t pressed_cnt){
	if(get_symbol(btn, duration, pressed_cnt) == EdConf.exit_symb_ok) return 1;
	return 0;
}

void init_editor(EditorConf config){
	EdConf = config;
	response_init(EdConf.response, EdConf.resp_size);
}

uint8_t alphabet_pull(uint8_t line, uint8_t element){
	return (EdConf.alphabet[line][element]);
}

void response_init(uint8_t *response, uint8_t size){
	uint8_t *editor_resp;
	uint8_t symb;
	editor_resp = response;
	resp_ptr=0;
	c_position=0;
	//Show already exist response
	for(uint8_t i=0;i<size;i++){
		symb = EdConf.response[i];
		if(symb != '\0'){
			c_position++;
			resp_ptr++;
			lcd_putc(symb);
		}
		else break;
	}
}

void response_push(uint8_t symbol){
	if(resp_ptr < EdConf.resp_size){
		EdConf.response[resp_ptr] = symbol;
		resp_ptr++;
	}
}

void response_rm_char(void){
	if(resp_ptr > 0){
		EdConf.response[resp_ptr] = '\0';
		resp_ptr--;
		EdConf.response[resp_ptr] = '\0';
	}
}

uint8_t is_max_response(void){
	if(resp_ptr == EdConf.resp_size)return 1;
	return 0;
}
