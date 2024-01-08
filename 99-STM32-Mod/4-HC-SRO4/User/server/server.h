#ifndef __SERVER_H
#define __SERVER_H
#include "bsp_led.h"
#include "stm32f10x.h"

/*
 *	舵机驱动头文件
 */


/************高级定时器TIM参数定义，只限TIM1和TIM8************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里我们使用高级控制定时器TIM8

#define            ADVANCE_TIM                   TIM8
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM8
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (200-1)
#define            ADVANCE_TIM_PSC               (7200-1)
#define            ADVANCE_TIM_PULSE             0


/************************** SG90 连接引脚定义********************************/
#define      	SG90_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define       SG90_GPIO_CLK                       RCC_APB2Periph_GPIOC

#define       SG90_GPIO_PORT                      GPIOC
#define       SG90_GPIO_PIN                       GPIO_Pin_6


/**************************函数声明********************************/
void ADVANCE_TIM_Init(void);
	
#endif /*__SERVER_H*/
