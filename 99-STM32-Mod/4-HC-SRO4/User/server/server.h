#ifndef __SERVER_H
#define __SERVER_H
#include "bsp_led.h"
#include "stm32f10x.h"

/*
 *	�������ͷ�ļ�
 */


/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM8

#define            ADVANCE_TIM                   TIM8
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM8
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (200-1)
#define            ADVANCE_TIM_PSC               (7200-1)
#define            ADVANCE_TIM_PULSE             0


/************************** SG90 �������Ŷ���********************************/
#define      	SG90_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define       SG90_GPIO_CLK                       RCC_APB2Periph_GPIOC

#define       SG90_GPIO_PORT                      GPIOC
#define       SG90_GPIO_PIN                       GPIO_Pin_6


/**************************��������********************************/
void ADVANCE_TIM_Init(void);
	
#endif /*__SERVER_H*/
