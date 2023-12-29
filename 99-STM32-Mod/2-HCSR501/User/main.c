/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   dht11��ʪ�ȴ���������ʵ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� �Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
	char dispBuff[100];
 	/* ��ʼ��ϵͳ��ʱ�� */
	SysTick_Init();
  LED_GPIO_Config();
  //LCD ��ʼ��
	ILI9341_Init (); 
 //����0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 //���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
 //���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����  
	ILI9341_GramScan ( 6 );

  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ��������ʾȫ�� */
  
	HC_SR501Configuration();
		while(1)
		{
			if(Read_STATE==1)
			{
				ILI9341_DispStringLine_EN(LINE(0),"Break&enter!");
				LED_GREEN;				//�����̵�
				delay_ms(1000);		//��ʱ   ͨ�����������ʱʱ��
			}
			else
			{
				ILI9341_DispStringLine_EN(LINE(0),"unmanned!");
				LED_RGBOFF;				//���˹ص�
			}
		};
}
/*********************************************END OF FILE**********************/
