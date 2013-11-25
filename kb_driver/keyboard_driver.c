#include "keyboard_driver.h"

void init_keyboard(void)
{
	btn_cnt = buttons_em;

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; //Clock port
	GPIOA->CRH 	&= ~(GPIO_CRH_MODE8_0 | GPIO_CRH_MODE11_0 | GPIO_CRH_MODE12_0);//Ножки 8,11,12 - входы,
	GPIOA->CRH 	&= ~(GPIO_CRH_MODE8_1 | GPIO_CRH_MODE11_1 | GPIO_CRH_MODE12_1);//
	GPIOA->CRH	&= ~(GPIO_CRH_CNF8_0  | GPIO_CRH_CNF11_0  | GPIO_CRH_CNF12_0); //подтяжка к +
	GPIOA->CRH |=   (GPIO_CRH_CNF8_1  | GPIO_CRH_CNF11_1  | GPIO_CRH_CNF12_1); //
	GPIOA->ODR |=   (GPIO_ODR_ODR8    | GPIO_ODR_ODR11    | GPIO_ODR_ODR12);   //подтяжка к +
	
	GPIOA->CRL 	|=  (GPIO_CRL_MODE4_0 | GPIO_CRL_MODE5_0 | GPIO_CRL_MODE6_0 | GPIO_CRL_MODE7_0);// 50MHz
	GPIOA->CRL 	|=  (GPIO_CRL_MODE4_1 | GPIO_CRL_MODE5_1 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE7_1);//
	GPIOA->CRL	&= ~(GPIO_CRL_CNF4_0  | GPIO_CRL_CNF5_0  | GPIO_CRL_CNF6_0  | GPIO_CRL_CNF7_0);// Ножки 3,4,5,6 - выходы
	GPIOA->CRL	&= ~(GPIO_CRL_CNF4_1  | GPIO_CRL_CNF5_1  | GPIO_CRL_CNF6_1  | GPIO_CRL_CNF7_1);//
}

void kb_strobe(void)
{
	uint16_t cols_[] = cols;
	uint16_t rows_[] = rows;

	for (uint8_t col=0; col<cols_num; col++)//Перебираем все строки
	{
		for (uint8_t i=0; i<cols_num; i++)//Формируем маску
		{
			if (i == col){KB_PORT->ODR &= ~cols_[i];}
			else{ KB_PORT->ODR |= cols_[i];}
		}

		for (uint8_t row=0; row<rows_num; row++)//Опрашиваем построчно
		{
			if ((KB_PORT->IDR & rows_[row]) == 0)
			{
				check_buttons[col][row]++;
			}
			else{
				if (check_buttons[col][row] >= trashhold1 &&
						check_buttons[col][row] <= trashhold2){
					add_button(col, row, '0');//0 means short press
				}
				if(check_buttons[col][row] > trashhold2){
					add_button(col, row, '1');//1 means long press
				}

				check_buttons[col][row]=0;
			}
		}
	}
}

void add_button(uint8_t col, uint8_t row, uint8_t duration){
	if (btn_cnt == 0) btn_cnt = buttons_em;
	buttons[btn_cnt-1] = templates[col*rows_num+row];
	durations[btn_cnt-1] = duration;
	btn_cnt--;
}

button* get_btn(void){
	cur_btn.button = 0;
	cur_btn.duration = 0;
	if(btn_cnt < buttons_em){
		cur_btn.button = buttons[btn_cnt];
		cur_btn.duration = durations[btn_cnt];
		btn_cnt++;
	}
	return &cur_btn;
}

uint8_t get_btn_simple(void){
	if(btn_cnt < buttons_em){
		btn_cnt++;
		return buttons[btn_cnt--];
	}
	else return 0;
}
