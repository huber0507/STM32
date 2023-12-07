#include "bsp_usart.h"
#include <stdio.h>


//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* 嵌套向量中断控制器组选择 */
//	/* 提示 NVIC_PriorityGroupConfig() 在整个工程只需要调用一次来配置优先级分组*/
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  /* 配置USART为中断源 */
//  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
//  /* 抢断优先级*/
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  /* 子优先级 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  /* 使能中断 */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  /* 初始化配置NVIC */
//  NVIC_Init(&NVIC_InitStructure);
//}



void USART_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*定义一个USART_InitTypeDef类型的结构体*/
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					/*关于这个GPIO的模式，在1-STM32F10x-中文参考手册.pdf
																															第八章8.1.11外设的GPIO配置有推荐使用哪种配置*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure); /*这个GPIO的初始化函数是指我要打开GPIO，必须要先初始化，这个函数的目的是用于将这个端口进行配置，
																															其中就是上面三行代码，&GPIO_InitStructure这个就是说我需要配置这个端口的信息，
																															比如50Mhz以及速率和哪个端口的信息*/

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);			/*这里没有配置速率是因为上面的GPIO初始化已经配置好速率，
																																PA9和PA10都是GPIOA,故这里无需再次配置，其实严谨一点在配置也无所谓*/
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;      /*这个波特率实际需要跟串口助手的波特率一致即可，如9600,115200等*/
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			/*有校验位就是9位，无校验位就是8位，这里就是串口助手中的校验位*/
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;					/*停止位有1和1.5跟2这三个配置，在串口助手可以体现*/
	// 配置校验位	
	USART_InitStructure.USART_Parity = USART_Parity_No;							/*校验位有好几种，跟串口助手配置一样，如*/
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 							/*停止位有1和1.5跟2这三个配置，在串口助手可以体现，
																																无校验 （no parity）奇校验 （odd parity）偶校验 （even parity）mark parity：
																																校验位始终为1space parity：校验位始终为0；这里配置的是无校验*/
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
//	// 串口中断优先级配置
//	NVIC_Configuration();
//	
//	// 使能串口接收中断
//	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

/*串口发送一个字节*/
void Usart_SendByte(USART_TypeDef * pUSARTx,uint8_t data)     //两个参数，一个是串口号，一个是一个字节的数据
{
	USART_SendData(pUSARTx, data);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);  /*实际就是将库函数包装一下，改成自己能够理解的方式*/
}


/*串口发送2个字节*/
void Usart_SendHalfword(USART_TypeDef * pUSARTx,uint16_t data)   //两个参数，一个是串口号，一个是2个字节的数据
{
	uint8_t temp_h,temp_l; 			//定义高八位和低八位参数
	temp_h=(data&0xff00)>>8;    //取高八位数据
	temp_l=(data&0xff);					//取低八位数据
	
	
	USART_SendData(pUSARTx, temp_h);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);//发送高八位   /*这里处理措施是先发送高八位再发低八位*/
	
	
	USART_SendData(pUSARTx, temp_l);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);//发送低八位
}


/*串口发8位数组*/
void Usart_SendArray(USART_TypeDef * pUSARTx,uint8_t *array,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		Usart_SendByte(pUSARTx,array[i]);
		
	}
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC)==RESET);
}

/*串口发送字符串*/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}


///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

/*********************************************END OF FILE**********************/
