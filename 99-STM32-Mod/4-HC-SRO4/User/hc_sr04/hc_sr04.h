#ifndef __HC_SR04_H
#define __HC_SR04_H

/*参数宏定义*********************/

/************************** HC_SR04 连接引脚定义********************************/
#define      	HC_SR04_SCK_APBxClock_FUN              RCC_APB2PeriphClockCmd

#define				HC_SR04_TIM_SCK_APBxClock_FUN					 RCC_APB1PeriphClockCmd
#define				HC_SR04_TIM_SCK					 							 RCC_APB1Periph_TIM2
#define       HC_SR04_TIM														 TIM2
#define       HC_SR04_GPIO_CLK                       RCC_APB2Periph_GPIOA
#define       HC_SR04_GPIO_AFIO_CLK                  RCC_APB2Periph_AFIO
#define 			HC_SR04_GPIO_EXTI_FUN 						   	 GPIO_EXTILineConfig
#define 			HC_SR04_GPIO_ITSOURCE 					       GPIO_PortSourceGPIOA
#define 			HC_SR04_GPIO_ITCHANNEL 					     	 GPIO_PinSource1


#define       HC_SR04_GPIO_PORT                      GPIOA
#define       HC_SR04_ECHO_PIN                       GPIO_Pin_7
#define       HC_SR04_EXTI_LINE											 EXTI_Line7
#define       HC_SR04_NVIC_IRQChannel								 EXTI9_5_IRQn
#define       HC_SR04_TRIG_PIN                       GPIO_Pin_6

/*********************参数宏定义*/



/*函数声明*********************/

void HC_SR04_Init(void);
void TIM2_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
unsigned int distance_mearsure_FUN(void);
	
	
#endif /* __HC_SR04_H */

