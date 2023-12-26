/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   AT24C02测试（软件模拟I2C）
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
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h" 
#include "./i2c/bsp_i2c_ee.h"
#include "./key/bsp_key.h"
#include <stdio.h>



 /**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  
    LED_GPIO_Config();
		USART_Config();

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

		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			LED_GREEN;
			delay_ms(1000);
			Usart_SendString( DEBUG_USARTx, "欢迎王茗轩光临我的STM32小屋~\n");
			LED_RGBOFF;

				

		} 


		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			LED_RED;
			delay_ms(1000);
			printf("欢迎光临\n");
			LED_RGBOFF;
		}

	}

}

}
/*********************************************END OF FILE**********************/

