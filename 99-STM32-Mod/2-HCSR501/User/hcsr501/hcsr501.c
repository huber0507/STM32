#include "hcsr501.h"

void HC_SR501Configuration(void)      //hc-sr501��ʼ������ 
{
		GPIO_InitTypeDef GPIO_InitStructure;     
    
    //Enable APB2 Bus
    RCC_APB2PeriphClockCmd(HC_SR50CLKLINE, ENABLE);   //��ʱ�� 
    
    //Register IO 
		GPIO_InitStructure.GPIO_Pin =HC_SR501_Pin;     //�������� 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //���ŵ�ģʽ 
		GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
    GPIO_Init(HC_SR50PORT, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}



