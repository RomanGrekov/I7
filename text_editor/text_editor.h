#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include "../libs/stm32f10x.h"
#include "../hd44780/hd44780.h"
#include "../kb_driver/keyboard_driver.h"
#include "../delay/delay.h"
#include "../globs.h"
#include "../menu/menu.h"

#define TIME_AFTER_PRESS F_CPU/400
#define disp_line_length 15 //symbols (count from 0)

typedef	struct
	{
        uint8_t clean_char_symb;
        uint8_t space_symb;
        uint8_t exit_symb_ok;
        uint8_t exit_symb_discard;
        uint8_t x_size;
        uint8_t y_size;
        uint8_t resp_size;
        uint8_t * response;
        uint8_t * alphabet;
	}EditorConf;

void init_editor(EditorConf config);
//void editor_init(uint8_t const *alphabet);
uint8_t get_symbol(uint8_t btn, uint8_t duration, uint8_t pressed_cnt);
void temp_lcd_show(uint8_t btn, uint8_t duration, uint8_t pressed_cnt);
uint8_t has_variants(uint8_t btn);
uint8_t get_line(uint8_t btn);
uint8_t get_vars_amount(uint8_t btn);
uint8_t is_exit(uint8_t btn, uint8_t duration, uint8_t pressed_cnt);
uint8_t typing(button *button_obj);
uint8_t alphabet_pull(uint8_t line, uint8_t element);
void editor_init(const uint8_t *alphabet_, uint8_t y, uint8_t x);
void response_init(uint8_t *response, uint8_t size);
void management_btns_init(const uint8_t *clean_char_,
						  const uint8_t *space_symb_,
						  const uint8_t *exit_symb_ok_,
						  const uint8_t *exit_symb_discard_);
void response_push(uint8_t symbol);
void response_rm_char(void);

#endif
