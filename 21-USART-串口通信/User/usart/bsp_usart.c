#include "bsp_usart.h"
#include <stdio.h>


//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* Ƕ�������жϿ�������ѡ�� */
//	/* ��ʾ NVIC_PriorityGroupConfig() ����������ֻ��Ҫ����һ�����������ȼ�����*/
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  /* ����USARTΪ�ж�Դ */
//  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
//  /* �������ȼ�*/
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  /* �����ȼ� */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  /* ʹ���ж� */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  /* ��ʼ������NVIC */
//  NVIC_Init(&NVIC_InitStructure);
//}



void USART_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*����һ��USART_InitTypeDef���͵Ľṹ��*/
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					/*�������GPIO��ģʽ����1-STM32F10x-���Ĳο��ֲ�.pdf
																															�ڰ���8.1.11�����GPIO�������Ƽ�ʹ����������*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure); /*���GPIO�ĳ�ʼ��������ָ��Ҫ��GPIO������Ҫ�ȳ�ʼ�������������Ŀ�������ڽ�����˿ڽ������ã�
																															���о����������д��룬&GPIO_InitStructure�������˵����Ҫ��������˿ڵ���Ϣ��
																															����50Mhz�Լ����ʺ��ĸ��˿ڵ���Ϣ*/

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);			/*����û��������������Ϊ�����GPIO��ʼ���Ѿ����ú����ʣ�
																																PA9��PA10����GPIOA,�����������ٴ����ã���ʵ�Ͻ�һ��������Ҳ����ν*/
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;      /*���������ʵ����Ҫ���������ֵĲ�����һ�¼��ɣ���9600,115200��*/
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			/*��У��λ����9λ����У��λ����8λ��������Ǵ��������е�У��λ*/
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;					/*ֹͣλ��1��1.5��2���������ã��ڴ������ֿ�������*/
	// ����У��λ	
	USART_InitStructure.USART_Parity = USART_Parity_No;							/*У��λ�кü��֣���������������һ������*/
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 							/*ֹͣλ��1��1.5��2���������ã��ڴ������ֿ������֣�
																																��У�� ��no parity����У�� ��odd parity��żУ�� ��even parity��mark parity��
																																У��λʼ��Ϊ1space parity��У��λʼ��Ϊ0���������õ�����У��*/
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
//	// �����ж����ȼ�����
//	NVIC_Configuration();
//	
//	// ʹ�ܴ��ڽ����ж�
//	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}

/*���ڷ���һ���ֽ�*/
void Usart_SendByte(USART_TypeDef * pUSARTx,uint8_t data)     //����������һ���Ǵ��ںţ�һ����һ���ֽڵ�����
{
	USART_SendData(pUSARTx, data);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);  /*ʵ�ʾ��ǽ��⺯����װһ�£��ĳ��Լ��ܹ����ķ�ʽ*/
}


/*���ڷ���2���ֽ�*/
void Usart_SendHalfword(USART_TypeDef * pUSARTx,uint16_t data)   //����������һ���Ǵ��ںţ�һ����2���ֽڵ�����
{
	uint8_t temp_h,temp_l; 			//����߰�λ�͵Ͱ�λ����
	temp_h=(data&0xff00)>>8;    //ȡ�߰�λ����
	temp_l=(data&0xff);					//ȡ�Ͱ�λ����
	
	
	USART_SendData(pUSARTx, temp_h);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);//���͸߰�λ   /*���ﴦ���ʩ���ȷ��͸߰�λ�ٷ��Ͱ�λ*/
	
	
	USART_SendData(pUSARTx, temp_l);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);//���͵Ͱ�λ
}


/*���ڷ�8λ����*/
void Usart_SendArray(USART_TypeDef * pUSARTx,uint8_t *array,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		Usart_SendByte(pUSARTx,array[i]);
		
	}
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC)==RESET);
}

/*���ڷ����ַ���*/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}


///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

/*********************************************END OF FILE**********************/
