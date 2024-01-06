/**
  ******************************************************************************
  * @file    main.c
  * @author  WEI
  * @version V1.0
  * @date    2024年1月6日
  * @brief   SG90经典舵机测试实验
  ******************************************************************************
  * @attention
  * 舵机红色线 --> 5V
  *	舵机棕色线 --> GND	
  *	舵机橙色线 --> GPIO_Pin_6
  ******************************************************************************
  */
  

#include "bsp_led.h"
#include "server.h"							//舵机驱动文件


/*以下函数为360度舵机的正向转动、停止、反向转动的测试函数
	*	PWM 信号与360舵机转速的关系：
	*	0.5ms ---------------- 正向最大转速
	*	1.5ms ---------------- 速度为0
	*	2.5ms ---------------- 反向最大转速
 
	*	舵机频率与占空比的计算：
	*	设舵机的频率为50HZ，则PWM周期为20ms，0度对应的占空比为2.5%，即0.05ms的高电平输出。
 */
 
//360度舵机测试函数
void Server_360_Forward(void)
{
	TIM_SetCompare1(ADVANCE_TIM,5);//0.5ms --> 正向最大转速
	delay_ms(10000);
	
	TIM_SetCompare1(ADVANCE_TIM,15);//1.5ms --> 速度为0
	delay_ms(10000);

	TIM_SetCompare1(ADVANCE_TIM,25);//2.5ms --> 反向最大转速
	delay_ms(10000);

}
/*****************************************END****************************************/

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	ADVANCE_TIM_Init();   //舵机初始化
          	 
	while(1)
	{  
		Server_360_Forward();//360度测试   2024年1月6日23:17:13 终于搞好了
	}
}
/*********************************************END OF FILE**********************/
