/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   AT24C02���ԣ����ģ��I2C��
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
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h" 
#include "./i2c/bsp_i2c_ee.h"
#include "./key/bsp_key.h"
#include <stdio.h>
<<<<<<< HEAD
#include "systick.h"
=======
>>>>>>> b9b49c6a3809620a8135636d9c0ccfdd4cf08834


 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  
    LED_GPIO_Config();
		USART_Config();
<<<<<<< HEAD
  while(1)
	{
    LED_BLUE;
		delay_ms(1000);
		LED_RED;
		delay_ms(1000);
		LED_GREEN;
		delay_ms(1000);
		LED_YELLOW;
		delay_ms(1000);
		LED_PURPLE;
		delay_ms(1000);
		LED_CYAN;
		delay_ms(1000);
		LED_WHITE;
		delay_ms(1000);
		LED_RGBOFF;
		delay_ms(1000);
=======
		Key_GPIO_Config();
  while(1)
	{
//    LED_BLUE;
//		delay_ms(1000);
//		LED_RED;
//		delay_ms(1000);
//		LED_GREEN;
//		delay_ms(1000);
//		LED_YELLOW;
//		delay_ms(1000);
//		LED_PURPLE;
//		delay_ms(1000);
//		LED_CYAN;
//		delay_ms(1000);
//		LED_WHITE;
//		delay_ms(1000);
//		LED_RGBOFF;
//		delay_ms(1000);
>>>>>>> b9b49c6a3809620a8135636d9c0ccfdd4cf08834
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			LED_GREEN;
			delay_ms(1000);
			Usart_SendString( DEBUG_USARTx, "��ӭ�����������ҵ�STM32С��~\n");
			LED_RGBOFF;
<<<<<<< HEAD
=======
				
>>>>>>> b9b49c6a3809620a8135636d9c0ccfdd4cf08834
		} 


		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			LED_RED;
			delay_ms(1000);
			printf("��ӭ����\n");
			LED_RGBOFF;
		}
<<<<<<< HEAD
	}
=======
}
>>>>>>> b9b49c6a3809620a8135636d9c0ccfdd4cf08834
}
/*********************************************END OF FILE**********************/
