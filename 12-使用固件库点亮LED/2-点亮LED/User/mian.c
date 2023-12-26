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
<<<<<<< HEAD:12-使用固件库点亮LED/2-点亮LED/User/mian.c

=======
<<<<<<< HEAD
#include "systick.h"
=======
>>>>>>> b9b49c6a3809620a8135636d9c0ccfdd4cf08834
>>>>>>> 46ee1b890d018862eefe9aa33d58987326c7f4b0:12-使用固件库点亮LED/2-点亮LED/User/main.c


 /**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  
    LED_GPIO_Config();
		USART_Config();
<<<<<<< HEAD:12-使用固件库点亮LED/2-点亮LED/User/mian.c

=======
<<<<<<< HEAD
>>>>>>> 46ee1b890d018862eefe9aa33d58987326c7f4b0:12-使用固件库点亮LED/2-点亮LED/User/main.c
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
<<<<<<< HEAD:12-使用固件库点亮LED/2-点亮LED/User/mian.c

=======
=======
>>>>>>> 46ee1b890d018862eefe9aa33d58987326c7f4b0:12-使用固件库点亮LED/2-点亮LED/User/main.c
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
<<<<<<< HEAD:12-使用固件库点亮LED/2-点亮LED/User/mian.c

=======
>>>>>>> b9b49c6a3809620a8135636d9c0ccfdd4cf08834
>>>>>>> 46ee1b890d018862eefe9aa33d58987326c7f4b0:12-使用固件库点亮LED/2-点亮LED/User/main.c
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			LED_GREEN;
			delay_ms(1000);
			Usart_SendString( DEBUG_USARTx, "欢迎王茗轩光临我的STM32小屋~\n");
			LED_RGBOFF;
<<<<<<< HEAD:12-使用固件库点亮LED/2-点亮LED/User/mian.c

				

=======
<<<<<<< HEAD
=======
				
>>>>>>> b9b49c6a3809620a8135636d9c0ccfdd4cf08834
>>>>>>> 46ee1b890d018862eefe9aa33d58987326c7f4b0:12-使用固件库点亮LED/2-点亮LED/User/main.c
		} 


		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			LED_RED;
			delay_ms(1000);
			printf("欢迎光临\n");
			LED_RGBOFF;
		}
<<<<<<< HEAD:12-使用固件库点亮LED/2-点亮LED/User/mian.c

	}

}

=======
<<<<<<< HEAD
	}
=======
}
>>>>>>> b9b49c6a3809620a8135636d9c0ccfdd4cf08834
>>>>>>> 46ee1b890d018862eefe9aa33d58987326c7f4b0:12-使用固件库点亮LED/2-点亮LED/User/main.c
}
/*********************************************END OF FILE**********************/

