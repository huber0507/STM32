#include "bsp_weiclkconfig.h"   //һ��ʼ��������ļ��͸���޷����.h�ļ�������Ϊ������.h�ļ�����д����

void HES_SetSysClk(uint32_t RCC_PLLMul_x)
{
		//HSE�����ɹ����ر���
	ErrorStatus HSEStatus;
	//��RCC�Ĵ�����λ�ɸ�λֵ
	RCC_DeInit();

	
	//ʹ��HSE
	RCC_HSEConfig(RCC_HSE_ON);
	
	//�ȴ�HSE����  
	HSEStatus=RCC_WaitForHSEStartUp();
	
	if(HSEStatus==SUCCESS)
	{
		// ʹ��FLASH Ԥ��ȡ������   ��Ƶ��ע����ʹ��Ԥȡָ
		
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		 // SYSCLK�������������ʱ��ı������ã�����ͳһ���ó�2�����ó�0��1�����������������
		FLASH_SetLatency(FLASH_Latency_2);		
		
		 // AHBԤ��Ƶ��������Ϊ1��Ƶ��HCLK = SYSCLK 
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    // APB2Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    // APB1Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK1 = HCLK/2 
    RCC_PCLK1Config(RCC_HCLK_Div2);
		
//-----------------���ø���Ƶ����Ҫ��������������-------------------//
    // ����PLLʱ����ԴΪHSE������PLL��Ƶ����
		
				// ����PLLCLK =HSE��ʵ�ʾ���8Mhz��* RCC_PLLMul_x
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);
//------------------------------------------------------------------//

    // ʹ��PLL 
    RCC_PLLCmd(ENABLE);
		
		 // �ȴ� PLL�ȶ�
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		// ��PLL�ȶ�֮�󣬰�PLLʱ���л�Ϊϵͳʱ��SYSCLK   ������ѡ��ϵͳʱ��
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
    while (RCC_GetSYSCLKSource() != 0x08);

		
	}
	else
	{
		/* ���HSE����ʧ�ܣ��û���������Ӵ������Ĵ���  */
	}
}



void HEI_SetSysClk(uint32_t RCC_PLLMul_x)
{
		//HSE�����ɹ����ر���
	__IO uint32_t HSIStatus=0;
	//��RCC�Ĵ�����λ�ɸ�λֵ
	RCC_DeInit();

	
	//ʹ��HSE
	RCC_HSICmd(ENABLE);
	
	HSIStatus = RCC->CR & RCC_CR_HSIRDY;
	
	
	if(HSIStatus==RCC_CR_HSIRDY)
	{
		// ʹ��FLASH Ԥ��ȡ������   ��Ƶ��ע����ʹ��Ԥȡָ
		
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		 // SYSCLK�������������ʱ��ı������ã�����ͳһ���ó�2�����ó�0��1�����������������
		FLASH_SetLatency(FLASH_Latency_2);		
		
		 // AHBԤ��Ƶ��������Ϊ1��Ƶ��HCLK = SYSCLK 
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    // APB2Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    // APB1Ԥ��Ƶ��������Ϊ1��Ƶ��PCLK1 = HCLK/2 
    RCC_PCLK1Config(RCC_HCLK_Div2);
		
//-----------------���ø���Ƶ����Ҫ��������������-------------------//
    // ����PLLʱ����ԴΪHSE������PLL��Ƶ����
		
				// ����PLLCLK =HSE��ʵ�ʾ���8Mhz��* RCC_PLLMul_x
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_x);
//------------------------------------------------------------------//

    // ʹ��PLL 
    RCC_PLLCmd(ENABLE);
		
		 // �ȴ� PLL�ȶ�
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		// ��PLL�ȶ�֮�󣬰�PLLʱ���л�Ϊϵͳʱ��SYSCLK   ������ѡ��ϵͳʱ��
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
    while (RCC_GetSYSCLKSource() != 0x08);

		
	}
	else
	{
		/* ���HSI����ʧ�ܣ��û���������Ӵ������Ĵ���  */
	}
}
