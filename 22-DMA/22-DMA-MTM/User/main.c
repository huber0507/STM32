/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	uint8_t status=0;
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	
	//����ǰ���Ƶ�
	LED_YELLOW;	
	//��ʱ
	Delay(0x0FFFFFF);
	//��ʼ��MDA
	MtM_DMA_Config();
	
	/* �ȴ�DMA������� */
  while(DMA_GetFlagStatus(MTM_DMA_FLAG_TC)==RESET);

	status= Buffercmp(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE);  //�ж������Ƿ��������Ա�һ���Ƿ�����ɣ�������ɷ���1�����򷵻�0
	if(status==1)
	{
		LED_RED;   //������ɺ��
	}
	else
	{
		LED_GREEN;   //���䲻����̵�
	}
	
	MtM_DMA_Config();	   //�����ٴγ�ʼ����Ϊʲô��  ��Ϊ���´δ������Ծ���ǰ��ʼ���ˣ�

	while (1)
	{
		
	}
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
