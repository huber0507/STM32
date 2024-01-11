/**
  ******************************************************************************
  * @file    main.c
  * @author  WEI
  * @version V1.0
  * @date    2024年1月7日
  * @brief   HC-SR04超声波测试程序
  ******************************************************************************
  * @attention
  * 0.96寸OLED SCL --> GPIO_PinB_6
  *	0.96寸OLED SDA --> GPIO_PinB_7
  *	HC-SR04 Trig --> GPIO_PinA_6
  * HC-SR04 Echo --> GPIO_PinA_7
  ******************************************************************************
  */
  
#include "stm32f10x.h"       // Device header
#include "Delay.h"           //使用的是b站江科大的延时函数,可以自己用别的替代延时功能
#include "OLED.h"            //使用的是b站江科大的OLED驱动代码,用于展示测距结果,可以在相应的地方更换为串口通信展示到电脑的串口助手上
#include "hc_sr04.h"
#include "bsp_led.h"

extern uint8_t flag;				//用于记录中断信号是上升沿还是下降沿
extern uint32_t number;			//记录定时器中断的次数
extern uint32_t times;			//记录回响信号的持续时间

/*****************************************END****************************************/


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
	
int main(void)
{
		DelayInit();  //延时初始化
		LED_GPIO_Config();  //LED初始化
		OLED_Init();    //OLED初始化
		HC_SR04_Init();		//HC_SR04初始化
    while(1){
				OLED_ShowNum(2,1,distance_mearsure_FUN(),8,8);  //在OLED显示实时反馈的距离值
				OLED_Update();
						}
}
void TIM2_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

/*********************************************END OF FILE**********************/
