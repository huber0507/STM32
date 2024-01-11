/**
  ******************************************************************************
  * @file    main.c
  * @author  WEI
  * @version V1.0
  * @date    2024��1��7��
  * @brief   HC-SR04���������Գ���
  ******************************************************************************
  * @attention
  * 0.96��OLED SCL --> GPIO_PinB_6
  *	0.96��OLED SDA --> GPIO_PinB_7
  *	HC-SR04 Trig --> GPIO_PinA_6
  * HC-SR04 Echo --> GPIO_PinA_7
  ******************************************************************************
  */
  
#include "stm32f10x.h"       // Device header
#include "Delay.h"           //ʹ�õ���bվ���ƴ����ʱ����,�����Լ��ñ�������ʱ����
#include "OLED.h"            //ʹ�õ���bվ���ƴ��OLED��������,����չʾ�����,��������Ӧ�ĵط�����Ϊ����ͨ��չʾ�����ԵĴ���������
#include "hc_sr04.h"
#include "bsp_led.h"

extern uint8_t flag;				//���ڼ�¼�ж��ź��������ػ����½���
extern uint32_t number;			//��¼��ʱ���жϵĴ���
extern uint32_t times;			//��¼�����źŵĳ���ʱ��

/*****************************************END****************************************/


/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
	
int main(void)
{
		DelayInit();  //��ʱ��ʼ��
		LED_GPIO_Config();  //LED��ʼ��
		OLED_Init();    //OLED��ʼ��
		HC_SR04_Init();		//HC_SR04��ʼ��
    while(1){
				OLED_ShowNum(2,1,distance_mearsure_FUN(),8,8);  //��OLED��ʾʵʱ�����ľ���ֵ
				OLED_Update();
						}
}
void TIM2_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

/*********************************************END OF FILE**********************/
