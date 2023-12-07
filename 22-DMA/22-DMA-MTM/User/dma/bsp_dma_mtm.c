#include "bsp_dma_mtm.h"






/* ����aSRC_Const_Buffer������ΪDMA��������Դ
 * const�ؼ��ֽ�aSRC_Const_Buffer�����������Ϊ��������
 * ��ʾ���ݴ洢���ڲ���FLASH��
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
															
/* ����DMA����Ŀ��洢��
 * �洢���ڲ���SRAM��																		
 */
uint32_t aDST_Buffer[BUFFER_SIZE];
																		
				
//typedef struct
//{
//  uint32_t DMA_PeripheralBaseAddr;   // �����ַ
//  uint32_t DMA_MemoryBaseAddr;       // �洢����ַ
//  uint32_t DMA_DIR;                  // ���䷽��
//  uint32_t DMA_BufferSize;           // ������Ŀ
//  uint32_t DMA_PeripheralInc;        // �����ַ����ģʽ
//  uint32_t DMA_MemoryInc;            // �洢����ַ����ģʽ
//  uint32_t DMA_PeripheralDataSize;   // �������ݿ��
//  uint32_t DMA_MemoryDataSize;       // �洢�����ݿ��
//  uint32_t DMA_Mode;                 // ģʽѡ��
//  uint32_t DMA_Priority;             // ͨ�����ȼ�
//  uint32_t DMA_M2M;                  // �洢�����洢��ģʽ
//}DMA_InitTypeDef;                    //�������stm32f10x_dma.h�ļ����о���ĳ�ʼ�����������ﲻ��׸����
																			//Ҳ�������ٴα�д��ֻ����������һ��˵���������ʼ����д���ٷ���д��


/**
  * �ж�ָ�����ȵ���������Դ�Ƿ���ȫ��ȣ�
  * �����ȫ��ȷ���1��ֻҪ����һ�����ݲ���ȷ���0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* ���ݳ��ȵݼ� */
  while(BufferLength--)
  {
    /* �ж���������Դ�Ƿ��Ӧ��� */
    if(*pBuffer != *pBuffer1)
    {
      /* ��Ӧ����Դ����������˳�������������0 */
      return 0;
    }
    /* ������������Դ�ĵ�ַָ�� */
    pBuffer++;
    pBuffer1++;
  }
  /* ����жϲ��Ҷ�Ӧ������� */
  return 1;  
}
																		
void MtM_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;       				  													//����DMA  MTM�ṹ��
	RCC_AHBPeriphClockCmd(MTM_DMA_CLK,ENABLE);     														//��DMAʱ��   
	
	DMA_InitStruct.DMA_PeripheralBaseAddr= (uint32_t)aSRC_Const_Buffer;       //ָ�����������ַ
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)aDST_Buffer;									//�ڴ����ַ
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;															//Ŀ�����ַ
	
	DMA_InitStruct.DMA_BufferSize=BUFFER_SIZE;												//�������ݴ�С
	
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Enable;				//DMA����ʹ��
	
	DMA_InitStruct.DMA_MemoryInc=DMA_PeripheralDataSize_Word;					//DMA���������ֽ�
	
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_MemoryInc_Enable;				//DMA1  �ڴ�ʹ��
	
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;				//DMA �ڴ����ݴ�С
	
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;													//DMAģʽ�� ��ͨ��ѭ����
	
	DMA_InitStruct.DMA_Priority=DMA_Priority_High ;										//���ȼ�  ��
	
	DMA_InitStruct.DMA_M2M=DMA_M2M_Enable;														//M2Mʹ��
	
	DMA_Init(MTM_DMA_CHANNEL, &DMA_InitStruct);												//DMA��ʼ���� ��ͨͨ���Լ���������    ����Ӳ�����ֲ��ýṹ�嶨��  
	
	DMA_ClearFlag(MTM_DMA_FLAG_TC);																		//���DMA��־λ
	
	DMA_Cmd(MTM_DMA_CHANNEL, ENABLE);																	//��ʼ��DMA

}	















