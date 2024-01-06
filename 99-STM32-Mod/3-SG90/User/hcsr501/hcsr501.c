#include "hcsr501.h"

void HC_SR501Configuration(void)      //hc-sr501初始化函数 
{
		GPIO_InitTypeDef GPIO_InitStructure;     
    
    //Enable APB2 Bus
    RCC_APB2PeriphClockCmd(HC_SR50CLKLINE, ENABLE);   //打开时钟 
    
    //Register IO 
		GPIO_InitStructure.GPIO_Pin =HC_SR501_Pin;     //定义引脚 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //引脚的模式 
		GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
    GPIO_Init(HC_SR50PORT, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}



