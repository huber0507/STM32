#include "bsp_weiclkconfig.h"   //一开始这里添加文件就搞错，无法添加.h文件，是因为我这里.h文件名称写错了

void HES_SetSysClk(uint32_t RCC_PLLMul_x)
{
		//HSE就绪成功返回变量
	ErrorStatus HSEStatus;
	//把RCC寄存器复位成复位值
	RCC_DeInit();

	
	//使能HSE
	RCC_HSEConfig(RCC_HSE_ON);
	
	//等待HSE就绪  
	HSEStatus=RCC_WaitForHSEStartUp();
	
	if(HSEStatus==SUCCESS)
	{
		// 使能FLASH 预存取缓冲区   视频中注释是使能预取指
		
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		 // SYSCLK周期与闪存访问时间的比例设置，这里统一设置成2，设置成0与1如果出错程序就死掉了
		FLASH_SetLatency(FLASH_Latency_2);		
		
		 // AHB预分频因子设置为1分频，HCLK = SYSCLK 
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    // APB2预分频因子设置为1分频，PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    // APB1预分频因子设置为1分频，PCLK1 = HCLK/2 
    RCC_PCLK1Config(RCC_HCLK_Div2);
		
//-----------------设置各种频率主要就是在这里设置-------------------//
    // 设置PLL时钟来源为HSE，设置PLL倍频因子
		
				// 配置PLLCLK =HSE（实际就是8Mhz）* RCC_PLLMul_x
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);
//------------------------------------------------------------------//

    // 使能PLL 
    RCC_PLLCmd(ENABLE);
		
		 // 等待 PLL稳定
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		// 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK   这里是选择系统时钟
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
    while (RCC_GetSYSCLKSource() != 0x08);

		
	}
	else
	{
		/* 如果HSE启动失败，用户在这里添加处理错误的代码  */
	}
}



void HEI_SetSysClk(uint32_t RCC_PLLMul_x)
{
		//HSE就绪成功返回变量
	__IO uint32_t HSIStatus=0;
	//把RCC寄存器复位成复位值
	RCC_DeInit();

	
	//使能HSE
	RCC_HSICmd(ENABLE);
	
	HSIStatus = RCC->CR & RCC_CR_HSIRDY;
	
	
	if(HSIStatus==RCC_CR_HSIRDY)
	{
		// 使能FLASH 预存取缓冲区   视频中注释是使能预取指
		
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		 // SYSCLK周期与闪存访问时间的比例设置，这里统一设置成2，设置成0与1如果出错程序就死掉了
		FLASH_SetLatency(FLASH_Latency_2);		
		
		 // AHB预分频因子设置为1分频，HCLK = SYSCLK 
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    // APB2预分频因子设置为1分频，PCLK2 = HCLK
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    // APB1预分频因子设置为1分频，PCLK1 = HCLK/2 
    RCC_PCLK1Config(RCC_HCLK_Div2);
		
//-----------------设置各种频率主要就是在这里设置-------------------//
    // 设置PLL时钟来源为HSE，设置PLL倍频因子
		
				// 配置PLLCLK =HSE（实际就是8Mhz）* RCC_PLLMul_x
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_x);
//------------------------------------------------------------------//

    // 使能PLL 
    RCC_PLLCmd(ENABLE);
		
		 // 等待 PLL稳定
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		// 当PLL稳定之后，把PLL时钟切换为系统时钟SYSCLK   这里是选择系统时钟
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
    while (RCC_GetSYSCLKSource() != 0x08);

		
	}
	else
	{
		/* 如果HSI启动失败，用户在这里添加处理错误的代码  */
	}
}
