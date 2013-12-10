//------------------------------------------------------
/* File:       Library for HD44780 compatible displays  */
/* Version:	   v1.1  						 			*/
/* Language	   ANSI C			   		  	 			*/
/* Author:     GrAnd/www.MakeSystem.net		 			*/
/* Tested on:  AVR		  			 	 	 		 	*/
/* License:	   GNU LGPLv2.1		 		 	 			*/
//------------------------------------------------------
/* Copyright (C)2012 GrAnd. All right reserved 			*/
//------------------------------------------------------

/*
  [hd44780.h - Library for HD44780 compatible displays]
  [Copyright (C)2012 GrAnd. All right reserved]

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Contact information :
						mail@makesystem.net
						http://makesystem.net/?page_id=2
*/

#include "hd44780.h"

volatile uint8_t lcd_cnt=0;

#if ( USE_PROGRESS_BAR )
static int8u_t progress_bar[NUMBER_OF_CELL_ELEMENTS] = {0x00,0x10,0x18,0x1C,0x1E,0x1F};
static int8u_t current_bar_load;
#endif

const uint8_t symbols[] = {'0','1','2','3','4','5','6','7','8','9'};
gpio_init(void){

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; //Clock port
    GPIOB->CRH      &= ~(GPIO_CRH_CNF10 |
    					 GPIO_CRH_CNF11 |
    					 GPIO_CRH_CNF12 |
    					 GPIO_CRH_CNF13 |
    					 GPIO_CRH_CNF14 |
    					 GPIO_CRH_CNF15);
    GPIOB->CRH		|= (GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1 |
    			        GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1 |
    			        GPIO_CRH_MODE12_0 | GPIO_CRH_MODE12_1 |
    			        GPIO_CRH_MODE13_0 | GPIO_CRH_MODE13_1 |
    			        GPIO_CRH_MODE14_0 | GPIO_CRH_MODE14_1 |
    			        GPIO_CRH_MODE15_0 |  GPIO_CRH_MODE15_1);
}

//-------------------------------
// LOW LEVEL FUNCTIONS
//-------------------------------
static void DELAY(int32u_t us);
static void LCD_STROBE(int32u_t u);
static void HIGHBITS(int8u_t data);
static void LOWBITS(int8u_t data);

//-------------------------------
/* DELAY FUNCTION */
//-------------------------------
static void DELAY(volatile int32u_t us)
{
 volatile int32u_t n, alfa, i;
 n = us * (MCU_CLK_VALUE / 1000000);
 i=0;
 for(alfa=0;alfa<n;alfa++)
 {
	 i++;
 }
}

//-------------------------------
/* INITIATE TRANSFER OF DATA/COMMAND TO LCD */
//-------------------------------
static void LCD_STROBE(uint32_t us)
{
 ENABLE(LCD_WIRE,E);
 DELAY(us);
 DISABLE(LCD_WIRE,E); // Enter
 DELAY(us);
}

//-------------------------------
/* PUT HIGH BITS */
//-------------------------------
static void HIGHBITS(int8u_t data)
{
 if(data & 0x80) ENABLE(LCD_WIRE,D7); else DISABLE(LCD_WIRE,D7);
 if(data & 0x40) ENABLE(LCD_WIRE,D6); else DISABLE(LCD_WIRE,D6);
 if(data & 0x20) ENABLE(LCD_WIRE,D5); else DISABLE(LCD_WIRE,D5);
 if(data & 0x10) ENABLE(LCD_WIRE,D4); else DISABLE(LCD_WIRE,D4);
}

//-------------------------------
/* PUT LOW BITS */
//-------------------------------
static void LOWBITS(int8u_t data)
{
 if(data & 0x08) ENABLE(LCD_WIRE,D7); else DISABLE(LCD_WIRE,D7);
 if(data & 0x04) ENABLE(LCD_WIRE,D6); else DISABLE(LCD_WIRE,D6);
 if(data & 0x02) ENABLE(LCD_WIRE,D5); else DISABLE(LCD_WIRE,D5);
 if(data & 0x01) ENABLE(LCD_WIRE,D4); else DISABLE(LCD_WIRE,D4);
}

//-------------------------------
/* PUT DATA/COMMAND TO LCD */
//-------------------------------
void lcd_cmd(int8u_t data)
{/* LCD ELEMENTARY COMMAND */
 HIGHBITS(data);
 LCD_STROBE(20);
 LOWBITS(data);
 LCD_STROBE(20); // busy delay
}


//-------------------------------
/* LCD CLEAR SCREEN */
//-------------------------------
void lcd_clrscr(void)
{
 lcd_cmd(0x01); // clear screen
 DELAY(200);
 lcd_cnt=0;
}

//-------------------------------
/* LCD RETURN CURSOR */
//-------------------------------
void lcd_return(void)
{
 lcd_cmd(0x02); // return cursor
}

//-------------------------------
/* GO TO SPECIFIED MEMORY ADDRESS */
//-------------------------------
void lcd_goto(int8u_t line, int8u_t address)
{/* GO TO SPECIFIED ADDRESS */
 switch(line)
 {
  case     1: lcd_cmd(0x80|address); break;
  case     2: lcd_cmd(0xC0|address); break;
  case CGRAM: lcd_cmd(0x40|address); break; // CGRAM address
 }
}

//-------------------------------
/* WRITE ENTIRE STRING 
   TO SPECIFIED MEMORY */
//-------------------------------
void lcd_prints(const int8u_t *p)
{/* WRITE A STRING TO LCD */
 ENABLE(LCD_WIRE,RS);
  while(*p)
  {
#if ( USE_FORMATTED_OUTPUT )
//-------------------------------
// new line
//-------------------------------
   if((*p == '\n'))
   {
	DISABLE(LCD_WIRE,RS);
	lcd_goto(2,0);
	ENABLE(LCD_WIRE,RS);
	p++;
   }
//-------------------------------
// return
//-------------------------------
   else if((*p == '\r'))
   {
	DISABLE(LCD_WIRE,RS);
	lcd_return();
	ENABLE(LCD_WIRE,RS);
	p++;
   }
//-------------------------------
// tab
//-------------------------------
   else if((*p == '\t'))
   {
	DISABLE(LCD_WIRE,RS);
	switch(TAB_SPACE)
    {
	 case 8: lcd_cmd(0x14,0); // cursor right shift
     	  	 lcd_cmd(0x14,0); // cursor right shift
     		 lcd_cmd(0x14,0); // cursor right shift
     		 lcd_cmd(0x14,0); // cursor right shift
     case 4: lcd_cmd(0x14,0); // cursor right shift
     	  	 lcd_cmd(0x14,0); // cursor right shift
     case 2: lcd_cmd(0x14,0); // cursor right shift
     case 1: lcd_cmd(0x14,0); // cursor right shift
    }
	ENABLE(LCD_WIRE,RS);
	p++;
   }
//-------------------------------
// display
//-------------------------------
   else
#endif
    lcd_cmd(*p++);
 }
 DISABLE(LCD_WIRE,RS);
}

//-------------------------------
/* WRITE A SINGLE CHARACTER 
   TO SPECIFIED MEMORY */
//-------------------------------
void lcd_putc(int8u_t data)
{/* WRITE A CHARACTER TO LCD */
 ENABLE(LCD_WIRE,RS);
 lcd_cmd(data);
 DISABLE(LCD_WIRE,RS);
}

//-------------------------------
/* LOAD USER-DEFINED CHARACTER IN CGRAM */
//-------------------------------
void lcd_load(int8u_t* vector, int8u_t position)
{/* USE CGRAM CHAR SPACE: 0 to 7 */
 int8u_t i;
 lcd_goto(CGRAM,position*DRAW_CHAR_SIZE);
 for(i=0;i<DRAW_CHAR_SIZE;i++)
  lcd_putc(vector[i]);
}

//-------------------------------
/* DISPLAY USER-DEFINED CHARACTER ON DDRAM */
//-------------------------------
void lcd_drawchar( int8u_t* vector, 
	 			   int8u_t position, 
	 			   int8u_t line, 
				   int8u_t address )
{/* USE CGRAM CHAR SPACE */
 lcd_load(vector,position);
 lcd_goto(line,address);
 lcd_putc(position);
}

//-------------------------------
/* ERASE A SINGLE CHARACTER 
   FROM DISPLAY */
//-------------------------------
void lcd_backspace(void)
{/* ERASE LEFT CHAR */
 lcd_cmd(0x10); // �������� ������ �� ���� ������� �����
 lcd_putc(' '); // �������, ����� ���� ���������� ������������� ������
 lcd_cmd(0x10); // �������� ������ �� ���� ������� �����
}

//-------------------------------
/* SCROLL DISPLAY 
   TO SPECIFIED DIRECTION */
//-------------------------------
void lcd_scroll(int8u_t direction)
{
 switch(direction)
 {
  case RIGHT : lcd_cmd(0x1C); break; // scroll display to right
  case LEFT  : lcd_cmd(0x18); break; // scroll display to left
 }
}

//-------------------------------
/* SHIFT CURSOR 
   TO SPECIFIED DIRECTION */
//-------------------------------
void cursor_shift(int8u_t direction)
{
 switch(direction)
 {
  case RIGHT : lcd_cmd(0x14); break; // shift cursor to right
  case LEFT  : lcd_cmd(0x10); break; // shift cursor to left
 }
}

//-------------------------------
/* DISPLAY A INTEGER NUMER */
//-------------------------------
void lcd_itostr(int32s_t value)
{/* DISPLAY A INTEGER NUMER: +/- 2147483647 */
 int32s_t i;
 if(value<0)
 {
  lcd_putc('-');
  value=-value;
 }
 for(i=1;(value/i)>9;i*=10);
 lcd_putc(value/i+'0');
 i/=10;
 while(i)
 {
  lcd_putc((value%(i*10))/i+'0');
  i/=10;
 }
}

//-------------------------------
/* DISPLAY A 4-DIGIT INTEGER NUMER */
//-------------------------------
void lcd_numTOstr(int16u_t value, int8u_t nDigit)
{/* DISPLAY n-DIGIT INTEGER NUMER */
 switch(nDigit)
 {
  case 4: lcd_putc((value/1000)+'0');
  case 3: lcd_putc(((value/100)%10)+'0');
  case 2: lcd_putc(((value/10)%10)+'0');
  case 1: lcd_putc((value%10)+'0');
 }
}

#if ( USE_PROGRESS_BAR )
//-------------------------------
/* PRELOAD PROGRESS BAR ELEMENTS IN CGRAM */
//-------------------------------
void lcd_readybar(void)
{
 int8u_t i,j;
 for(i=0;i<NUMBER_OF_CELL_ELEMENTS;i++)
 {
  lcd_goto(CGRAM,(i*DRAW_CHAR_SIZE));
  for(j=0;j<PROGRESS_BAR_HEIGHT;j++)
   lcd_putc(progress_bar[i]);
 }
 lcd_goto(1,0);
}

//-------------------------------
/* DRAW PROGRESS BAR ON DDRAM */
//-------------------------------
void lcd_drawbar(int8u_t next_bar_load)
{
 int8u_t i = current_bar_load;
 int8u_t cell = (current_bar_load/FULL_ELEMENT); // find current cell position in progress bar
 if(next_bar_load > NUMBER_OF_BAR_ELEMENTS ) next_bar_load = NUMBER_OF_BAR_ELEMENTS;
 if( next_bar_load > current_bar_load )
 {
//-------------------------------
// increment progress bar code //
//-------------------------------
  lcd_goto(DRAW_PROGRESS_BAR_ON_LINE, cell); // goto current cell position
  while( i != next_bar_load )
  {
   i++;
   if( CELL_RATIO(i) == 0 ) lcd_putc( FULL_ELEMENT );
    else lcd_putc( CELL_RATIO(i) );
   if( CELL_RATIO(i) ) cursor_shift(LEFT);
  }
 }
 else
 {
//-------------------------------
// decrement progress bar code //
//-------------------------------
  if(CELL_RATIO(i) == 0) cell--;
  lcd_goto(DRAW_PROGRESS_BAR_ON_LINE, cell); // goto current cell position
  lcd_cmd(ENTRY_MODE_DEC_NO_SHIFT,0); // decrement lcd cursor
  while( i != next_bar_load )
  {
   i--;
   lcd_putc( CELL_RATIO(i) );
   if( CELL_RATIO(i) ) cursor_shift(RIGHT);
  }
  lcd_cmd(ENTRY_MODE_INC_NO_SHIFT,0); // increment lcd cursor
 }
//-------------------------------
//       store new value       //
//-------------------------------
 current_bar_load = next_bar_load;
}

//-------------------------------
/*  CLEAR ENTYRE PROGRESS BAR  */
//-------------------------------
void lcd_clearbar(void)
{
 int8u_t i;
 lcd_goto(DRAW_PROGRESS_BAR_ON_LINE, (PROGRESS_BAR_WIDTH - 1));
 lcd_cmd(ENTRY_MODE_DEC_NO_SHIFT,0); // decrement
 for(i=0;i<PROGRESS_BAR_WIDTH;i++)
  lcd_putc(' ');
 lcd_cmd(ENTRY_MODE_INC_NO_SHIFT,0); // increment
 current_bar_load = 0;
}
#endif

//-------------------------------
/* CONFIGURE 4-BIT DISPLAY INTERFACE */
//-------------------------------
void lcd_config(int8u_t param)
{/* CONFIGURE THE DISPLAY */
 HIGHBITS(param); // 4-bit, two lines, 5x8 pixel
  LCD_STROBE(9); // change 8-bit interface to 4-bit interface
  LCD_STROBE(9); // init 4-bit interface
 LOWBITS(param);
  LCD_STROBE(40);
}

//-------------------------------
/* INITIALIZE ENTIRE DISPLAY */
//-------------------------------
void lcd_init(void)
{
 DELAY(15000);
 gpio_init();
 lcd_cmd(0x30); // 1, return home cursor
 lcd_cmd(0x30); // 1, return home cursor
 lcd_cmd(0x30); // 1, return home cursor
 lcd_cmd(0x30); // 1, return home cursor
 lcd_cmd(0x30); // 1, return home cursor

 lcd_config(DEFAULT_DISPLAY_CONFIG); // 1, Data Lenght, Number of lines, character font
 lcd_cmd(DEFAULT_DISPLAY_CONTROL); // 1, lcd, cursor, blink
 lcd_cmd(DEFAULT_ENTRY_MODE); // 1,increment/decrement,display shift on/off
 lcd_cmd(0x01); // clear display
 lcd_cmd(0x02); // 1, return home cursor
#if (USE_PROGRESS_BAR)
 lcd_readybar();
#endif
}

void turn_on_cursor(void){
 lcd_cmd(DISPLAY_CTRL_DispOn_BlkOff_CrsOn); // 1, lcd, cursor, blink
}
void turn_off_cursor(void){
 lcd_cmd(DISPLAY_CTRL_DispOn_BlkOff_CrsOff); // 1, lcd, cursor, blink
}

void lcd_putcc(uint8_t sym){
	if (lcd_cnt == 16)lcd_goto(2, 0);
	if (lcd_cnt == 32)
	{
		lcd_clrscr();
		lcd_cnt = 0;
		lcd_goto(1, 0);
	}
	lcd_putc(sym);
	lcd_cnt++;
}

void LCDPrintS(uint8_t *p)
{
	while(*p)
	{
		lcd_putcc(*p++);
	}
}
void LCDLine(uint8_t line){
	switch(line){
	case 0:
		lcd_cnt = 32;
		break;
	case 1:
		lcd_cnt = 16;
		break;
	}
}

uint8_t GetSymb(uint8_t digit){
	return symbols[digit];
}

void shift_display(uint8_t direction)
{
	if(direction == '>'){
		lcd_cmd(0x1E);
	};
	if(direction == '<'){
		lcd_cmd(0x18);
	};
}
//-------------------------------
/* END OF FILE */
//-------------------------------
