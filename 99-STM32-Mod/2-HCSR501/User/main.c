/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   dht11温湿度传感器测试实验
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "./systick/bsp_SysTick.h"
#include "./dht11/bsp_dht11.h"
#include "./usart/bsp_usart.h"
#include "./lcd/bsp_ili9341_lcd.h"
#include "bsp_led.h"
#include "hcsr501.h"


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	char dispBuff[100];
 	/* 初始化系统定时器 */
	SysTick_Init();
  LED_GPIO_Config();
  //LCD 初始化
	ILI9341_Init (); 
 //其中0、3、5、6 模式适合从左至右显示文字，
 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 //其中 6 模式为大部分液晶例程的默认显示方向  
	ILI9341_GramScan ( 6 );

  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
  
	HC_SR501Configuration();
		while(1)
		{
			if(Read_STATE==1)
			{
				ILI9341_DispStringLine_EN(LINE(0),"Break&enter!");
				LED_GREEN;				//有人绿灯
				delay_ms(1000);		//延时   通过软件调整延时时间
			}
			else
			{
				ILI9341_DispStringLine_EN(LINE(0),"unmanned!");
				LED_RGBOFF;				//无人关灯
			}
		};
}
/*********************************************END OF FILE**********************/
