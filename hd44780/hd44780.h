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

#ifndef HD44780_H
#define HD44780_H

#include "mac.h"

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------
/* SET LCD<->MCU CONNECTIONS */
//-------------------------------
#define LCD_WIRE          		      GPIOB->ODR // select your microcontroller gpio port
#define SET_R						  ODR //
#define E  		 	   	  		      11 // pin number in selected port
#define RS 		  	   	  		      10 // pin number in selected port
#define D7 		 	   	  		      15 // pin number in selected port
#define D6 		 	   	  		      14 // pin number in selected port
#define D5 		 	   	  		      13 // pin number in selected port
#define D4 		  	   	  		      12 // pin number in selected port
//-------------------------------
// SELECT MCU FREQUENCE
//-------------------------------
#define MCU_CLK_VALUE				  12000000 /* in MHz. Minimal value - 1 MHz */
#define MCU_CLK						  ((MCU_CLK_VALUE - 1)/4)
#define MCU_WAIT_CYCLES				  180
//-------------------------------
// DISPLAY CONFIG MACROS
//-------------------------------
#define DISPLAY_CONFIG_4bit_2L_5x8	  0x2C /* Use 4-bit interface, 2 Lines, 5x8 pixel resolution */
#define DISPLAY_CONFIG_4bit_1L_5x10	  0x24 /* Use 4-bit interface, 1 Line, 5x10 pixel resolution */
#define DEFAULT_DISPLAY_CONFIG		  DISPLAY_CONFIG_4bit_2L_5x8
//-------------------------------
// ENTRY MODE MACROS
//-------------------------------
#define ENTRY_MODE_DEC_NO_SHIFT	      0x04 // ������ �� ����, ��� ������ ������� ����� DDRAM ������
#define ENTRY_MODE_DEC_WITH_SHIFT     0x05 // ������ �� ����, �� ������� ������� ����� DDRAM ������
#define ENTRY_MODE_INC_NO_SHIFT	      0x06 // ����� �� �����, ��� ������ ������� ����� DDRAM ������
#define ENTRY_MODE_INC_WITH_SHIFT     0x07 // ����� �� �����, �� ������� ������� ����� DDRAM ������
#define DEFAULT_ENTRY_MODE			  ENTRY_MODE_INC_NO_SHIFT
//-------------------------------
// DISPLAY CONTROL MACROS
//-------------------------------
#define DISPLAY_CTRL_DispOn_BlkOn_CrsOn	  	  0x0F /* Display On, Blink On, Cursor On */
#define DISPLAY_CTRL_DispOn_BlkOff_CrsOn	  0x0E /* Display On, Blink Off, Cursor On */
#define DISPLAY_CTRL_DispOn_BlkOn_CrsOff	  0x0D /* Display On, Blink On, Cursor Off */
#define DISPLAY_CTRL_DispOn_BlkOff_CrsOff	  0x0C /* Display On, Blink Off, Cursor Off */
#define DISPLAY_CTRL_DispOff_BlkOff_CrsOff	  0x08 /* Display Off, Blink Off, Cursor Off */
#define DEFAULT_DISPLAY_CONTROL		  DISPLAY_CTRL_DispOn_BlkOff_CrsOff
//-------------------------------
// SET FORMATTED OUTPUT OPTIONS
//-------------------------------
#define USE_FORMATTED_OUTPUT	 	  0 /* 1 (true) or 0 (false) */
#define TAB_SPACE					  4 /* 1, 2, 4 or 8 */
//-------------------------------
// PROGRESS BAR DRAWING OPTIONS
//-------------------------------
#define USE_PROGRESS_BAR			  0  /* 1 or 0 */
#define DRAW_PROGRESS_BAR_ON_LINE	  2  /* Select lcd line: 1, 2, 4, ... */
#define PROGRESS_BAR_HEIGHT			  5  /* in pixel: 1(min),2,3,4,5,6,7,8(max) */
#define PROGRESS_BAR_WIDTH			  16 /* Number of chars in lcd line: ... , 8, 16, 20 */
//-------------------------------
// OTHER MACROS
//-------------------------------
#define CGRAM	 	   	  		      5 /* used in "lcd_goto(CGRAM,address)" function */
#define RIGHT   	  			  	  '>'
#define LEFT       			  		  '<'
#define DRAW_CHAR_SIZE    			  8 /* 8 or 11 */
#define NUMBER_OF_CELL_ELEMENTS		  6
#define FULL_ELEMENT				  (NUMBER_OF_CELL_ELEMENTS - 1)
#define NUMBER_OF_BAR_ELEMENTS		  (PROGRESS_BAR_WIDTH * FULL_ELEMENT)
#define CELL_RATIO(data)	  		  ((data)%FULL_ELEMENT)
#define EMPTY_ELEMENT				  0				

//-------------------------------
// LCDlib API
//-------------------------------
void lcd_cmd(int8u_t data);
void lcd_clrscr(void);
void lcd_return(void);
void lcd_goto(int8u_t line, int8u_t address);
void lcd_prints(const int8u_t *p);
void lcd_putc(int8u_t data);
void lcd_load(int8u_t *vector, int8u_t position);
void lcd_drawchar(int8u_t *vector, int8u_t position, int8u_t line, int8u_t address);
void lcd_backspace(void);
void lcd_scroll(int8u_t direction);
void cursor_shift(int8u_t direction);
void lcd_itostr(int32s_t value);
void lcd_numTOstr(int16u_t value, int8u_t nDigit);
void lcd_config(int8u_t param);
void lcd_init(void);
void gpio_init(void);

void lcd_putcc(uint8_t sym);
void LCDPrintS(uint8_t *p);
//-------------------------------
// PROGRESS BAR API
//-------------------------------
#if ( USE_PROGRESS_BAR )
void lcd_readybar(void);
void lcd_drawbar(int8u_t data);
void lcd_clearbar(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* HD44780_H */

//-------------------------------
/* END OF FILE */
//-------------------------------
