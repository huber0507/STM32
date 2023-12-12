/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
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
#include "bsp_led.h"
#include "bsp_dma_mtm.h"

#define SOFT_DELAY Delay(0x0FFFFF);
extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];

void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	uint8_t status=0;
	/* LED 端口初始化 */
	LED_GPIO_Config();	
	//传输前亮黄灯
	LED_YELLOW;	
	//延时
	Delay(0x0FFFFFF);
	//初始化MDA
	MtM_DMA_Config();
	
	/* 等待DMA传输完成 */
  while(DMA_GetFlagStatus(MTM_DMA_FLAG_TC)==RESET);

	status= Buffercmp(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE);  //判断数据是否完整，对比一下是否传输完成，传输完成返回1，否则返回0
	if(status==1)
	{
		LED_RED;   //传输完成红灯
	}
	else
	{
		LED_GREEN;   //传输不完成绿灯
	}
	
	MtM_DMA_Config();	   //这里再次初始化是为什么？  是为了下次传输所以就提前初始化了？

	while (1)
	{
		
	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
