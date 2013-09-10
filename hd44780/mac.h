//-------------------------------
/* MACRO DEFINITIONS */
//-------------------------------

#ifndef MAC_H
#define MAC_H

/* Include here the header file of your microcontroller */
#include "../libs/stm32f10x.h"
//#include <avr/io.h>

//-------------------------------
#define ENABLE(x,n)           	((x) |=  (1<<(n)))
#define DISABLE(x,n)  		  	((x) &=~ (1<<(n)))
#define TOGGLE(x,n)   		  	((x) ^=  (1<<(n)))
#define CHECKBIT(x,n) 		  	((x) &   (1<<(n)))
//-------------------------------
typedef unsigned char    		int8u_t;
typedef signed char      		int8s_t;
typedef unsigned short   		int16u_t;
typedef signed short     		int16s_t;
typedef unsigned long    		int32u_t;
typedef signed long      		int32s_t;
//-------------------------------

#endif /* MAC_H */

//-------------------------------
/* THE END */
//-------------------------------
