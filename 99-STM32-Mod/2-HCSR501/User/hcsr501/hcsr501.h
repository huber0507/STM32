#ifndef __HCSR501_H__
#define	__HCSR501_H__
#include "stm32f10x.h"
#include "bsp_led.h"



#define				HC_SR501						PAin(0)
#define				HC_SR501_Pin				GPIO_Pin_2
#define    	 	HC_SR50PORT         GPIOA
#define     	HC_SR50CLKLINE      RCC_APB2Periph_GPIOA
#define 			Read_STATE				GPIO_ReadInputDataBit(HC_SR50PORT ,HC_SR501_Pin)    //这个可以读取外部输入状态，取代了自己写的函数




extern void HC_SR501Configuration(void);
extern FunctionalState HC_SR501_Status(void);
#endif /*__HCSR501_H__*/

