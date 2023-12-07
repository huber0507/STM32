#include "bsp_dma_mtm.h"






/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
 * const关键字将aSRC_Const_Buffer数组变量定义为常量类型
 * 表示数据存储在内部的FLASH中
 */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
															
/* 定义DMA传输目标存储器
 * 存储在内部的SRAM中																		
 */
uint32_t aDST_Buffer[BUFFER_SIZE];
																		
				
//typedef struct
//{
//  uint32_t DMA_PeripheralBaseAddr;   // 外设地址
//  uint32_t DMA_MemoryBaseAddr;       // 存储器地址
//  uint32_t DMA_DIR;                  // 传输方向
//  uint32_t DMA_BufferSize;           // 传输数目
//  uint32_t DMA_PeripheralInc;        // 外设地址增量模式
//  uint32_t DMA_MemoryInc;            // 存储器地址增量模式
//  uint32_t DMA_PeripheralDataSize;   // 外设数据宽度
//  uint32_t DMA_MemoryDataSize;       // 存储器数据宽度
//  uint32_t DMA_Mode;                 // 模式选择
//  uint32_t DMA_Priority;             // 通道优先级
//  uint32_t DMA_M2M;                  // 存储器到存储器模式
//}DMA_InitTypeDef;                    //这个是在stm32f10x_dma.h文件中有具体的初始化函数，这里不做赘述，
																			//也不单独再次编写，只是在这里做一个说明，这个初始化不写，官方已写好


/**
  * 判断指定长度的两个数据源是否完全相等，
  * 如果完全相等返回1，只要其中一对数据不相等返回0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* 数据长度递减 */
  while(BufferLength--)
  {
    /* 判断两个数据源是否对应相等 */
    if(*pBuffer != *pBuffer1)
    {
      /* 对应数据源不相等马上退出函数，并返回0 */
      return 0;
    }
    /* 递增两个数据源的地址指针 */
    pBuffer++;
    pBuffer1++;
  }
  /* 完成判断并且对应数据相对 */
  return 1;  
}
																		
void MtM_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;       				  													//定义DMA  MTM结构体
	RCC_AHBPeriphClockCmd(MTM_DMA_CLK,ENABLE);     														//打开DMA时钟   
	
	DMA_InitStruct.DMA_PeripheralBaseAddr= (uint32_t)aSRC_Const_Buffer;       //指定好外设基地址
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)aDST_Buffer;									//内存基地址
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;															//目标基地址
	
	DMA_InitStruct.DMA_BufferSize=BUFFER_SIZE;												//发送数据大小
	
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Enable;				//DMA外设使能
	
	DMA_InitStruct.DMA_MemoryInc=DMA_PeripheralDataSize_Word;					//DMA外设数据字节
	
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_MemoryInc_Enable;				//DMA1  内存使能
	
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;				//DMA 内存数据大小
	
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;													//DMA模式， 普通、循环、
	
	DMA_InitStruct.DMA_Priority=DMA_Priority_High ;										//优先级  高
	
	DMA_InitStruct.DMA_M2M=DMA_M2M_Enable;														//M2M使能
	
	DMA_Init(MTM_DMA_CHANNEL, &DMA_InitStruct);												//DMA初始化， 开通通道以及以上配置    关于硬件部分采用结构体定义  
	
	DMA_ClearFlag(MTM_DMA_FLAG_TC);																		//清除DMA标志位
	
	DMA_Cmd(MTM_DMA_CHANNEL, ENABLE);																	//初始化DMA

}	















