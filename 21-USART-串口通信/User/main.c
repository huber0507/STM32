/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "stm32f10x_it.h"
#include <stdio.h>

int main(void)
{	
	//uint8_t a[10]={1,2,3,4,5,6,7,8,9,10};

	uint8_t a=0;
	USART_Config();
	LED_GPIO_Config();
//	Usart_SendByte(DEBUG_USARTx,'x'); //��ʾ�����ַ� 
//	Usart_SendHalfword(DEBUG_USARTx,0xff56);
//	Usart_SendArray(DEBUG_USARTx,a,10);
	Usart_SendString( DEBUG_USARTx, "��ӭ�����������ҵ�STM32С��~������\n");
//	printf("��ӭ����\n");
	//scanf();
	
//	putchar('w');
	while(1)
	{
		a=getchar();
		printf("a=%c\n",a);
		switch(a)
		{
			case '1':LED_YELLOW;
				break;
			case '2':LED_PURPLE;
				break;
			case '3':LED_CYAN;
				break;
			default:LED_RGBOFF;
				break;
		}
	}
}

/*********************************************END OF FILE**********************/
