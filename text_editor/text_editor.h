#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include "../libs/stm32f10x.h"
#include "../hd44780/hd44780.h"
#include "../kb_driver/keyboard_driver.h"
#include "../delay/delay.h"
#include "../globs.h"
#include "../menu/menu.h"
//#include "../slow_timer/slow_timer.h"

#define TIME_AFTER_PRESS 800 //in ms
#define disp_line_length 16 //symbols (count from 0)
#define max_resp_size 50

/*
///pressed times 0   1   2	  3   4   Long press
				'0', 0 , 0 , 0 , 0 ,' ',
				'1','_', 0 , 0 , 0 , 0 ,
				'2','a','b','c', 0 , 0 ,
				'3','d','e','f', 0 , 0 ,
				'4','g','h','i', 0 , 0 ,
				'5','j','k','l', 0 , 0 ,
				'6','m','n','o', 0 , 0 ,
				'7','p','q','r','s', 0 ,
				'8','t','u','v', 0 , 0 ,
				'9','w','x','y','z', 0 ,
				'<', 0 , 0 , 0 , 0 ,'#',
				'*','+', 0 , 0 , 0 ,'^',
*/

typedef	struct
	{
		uint8_t *top_line;// Header text(top line)
        uint8_t clean_char_symb;// Service symbol - backspace
        uint8_t space_symb;// Service symbol - space
        uint8_t exit_symb_ok;// Service symbol - successful exit
        uint8_t exit_symb_discard;// Service symbol - discard
        uint8_t resp_size;// Maximum text length
        uint8_t do_exit_on_max_resp;// Whether we exit when max text size reached
        uint8_t *old_response;// already present text
        uint8_t x_size;// dictionary - variants for one buttons
        uint8_t y_size;// buttons amount
        uint8_t *alphabet;// buttons dictionary
        uint8_t mask;// Symbol for masking lcd
	}EditorConf;

void init_editor(EditorConf config);
uint8_t *get_text(void);

#endif
