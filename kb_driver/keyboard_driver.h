#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H
#include "../libs/stm32f10x.h"

typedef struct {
	uint8_t button;
	uint8_t duration;
} button;

button cur_btn;

#define KB_PORT GPIOA

#define cols_num 4 //Строки
#define rows_num 3 //Колонки
#define buttons_em cols_num*rows_num //Общее кол-во кнопок
#define trashhold1 10 //Нижний уровень стробирования
#define trashhold2 70 //Верхний уровень стробирования
#define templates "123456789*0#" //Знаки на клавиатуре
#define cols {GPIO_ODR_ODR3,GPIO_ODR_ODR4,GPIO_ODR_ODR5,GPIO_ODR_ODR6}; //Пины строк
#define rows {GPIO_IDR_IDR0,GPIO_IDR_IDR1,GPIO_IDR_IDR2}; //Пины колонок

uint8_t check_buttons[cols_num][rows_num];
uint8_t buttons[buttons_em];
uint8_t durations[buttons_em];
uint8_t btn_cnt;
void init_keyboard(void);
void kb_strobe(void);
void add_button(uint8_t col, uint8_t row, uint8_t duration);
button* get_btn(void);
uint8_t get_btn_simple(void);

#endif
