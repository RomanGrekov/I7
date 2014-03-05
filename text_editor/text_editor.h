#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include "../libs/stm32f10x.h"
#include "../hd44780/hd44780.h"
#include "../kb_driver/keyboard_driver.h"
#include "../delay/delay.h"
#include "../globs.h"
#include "../menu/menu.h"
//#include "../slow_timer/slow_timer.h"

#define TIME_AFTER_PRESS 700 //in ms
#define disp_line_length 16 //symbols (count from 0)
#define max_resp_size 50

typedef	struct
	{
		uint8_t *top_line;
        uint8_t clean_char_symb;
        uint8_t space_symb;
        uint8_t exit_symb_ok;
        uint8_t exit_symb_discard;
        uint8_t resp_size;
        uint8_t do_exit_on_max_resp;
        uint8_t *old_response;
        uint8_t x_size;
        uint8_t y_size;
        uint8_t *alphabet;
	}EditorConf;

void init_editor(EditorConf config);

#endif
