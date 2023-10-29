/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include <stdio.h>

int main(void)
{	
	uint8_t a[10]={1,2,3,4,5,6,7,8,9,10};
	USART_Config();
	Usart_SendByte(DEBUG_USARTx,'x'); //显示单个字符 
	Usart_SendHalfword(DEBUG_USARTx,0xff56);
	Usart_SendArray(DEBUG_USARTx,a,10);
	Usart_SendString( DEBUG_USARTx, "欢迎王茗轩光临我的STM32小屋~爱老婆\n");
	printf("欢迎光临\n");
	//scanf();
	putchar('w');
}

/*********************************************END OF FILE**********************/
