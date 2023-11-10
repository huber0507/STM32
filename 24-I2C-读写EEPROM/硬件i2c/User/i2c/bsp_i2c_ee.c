/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   i2c EEPROM(AT24C02)Ӧ�ú���bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./i2c/bsp_i2c_ee.h"
#include "./usart/bsp_usart.h"	
#include "bsp_SysTick.h"




uint16_t EEPROM_ADDRESS;    //����eeprom

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   


static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


/**
  * @brief  I2C I/O����
  * @param  ��
  * @retval ��
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* ʹ���� I2C �йص�ʱ�� */
	EEPROM_I2C_APBxClock_FUN ( EEPROM_I2C_CLK, ENABLE );    						//����I2C��ʱ��
	EEPROM_I2C_GPIO_APBxClock_FUN ( EEPROM_I2C_GPIO_CLK, ENABLE );     //����I2C����GPIOB��ʱ��  ʵ�ʶ�Ӧ����I2C��GPIOB��ʱ��
	
    
  /* I2C_SCL��I2C_SDA*/
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;     							//����I2C��SCL���ź�ΪPB6
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//��������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;		       							/* ��©���   ��ʵ�ʸ���Ϊ��������Ҳ���������
																																				�˾�������Ұ����Ƶ���ᵽSTM32�����汾��GPIO������������ģʽ��
																																				���F103����,��1-STM32F10x-���Ĳο��ֲ�.pdf��
																																				�ڰ���8.1.11 �����GPIO�������ᵽ���������GPIO������Ҫ����Ϊ��©�����*/
  GPIO_Init(EEPROM_I2C_SCL_PORT, &GPIO_InitStructure);								//GPIOB��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;										//����I2C��SDA���ź�ΪPB7
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//��������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;		       							/* ��©���   ��ʵ�ʸ���Ϊ��������Ҳ���������
																																				�˾�������Ұ����Ƶ���ᵽSTM32�����汾��GPIO������������ģʽ��
																																				���F103����,��1-STM32F10x-���Ĳο��ֲ�.pdf��
																																				�ڰ���8.1.11 �����GPIO�������ᵽ���������GPIO������Ҫ����Ϊ��©�����*/
  GPIO_Init(EEPROM_I2C_SDA_PORT, &GPIO_InitStructure);								//GPIOB��ʼ��
		
	
}


/**
  * @brief  I2C ����ģʽ����
  * @param  ��
  * @retval ��
  */
static void I2C_Mode_Configu(void)			//I2C ����ģʽ����
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				//����ģʽ
	
	/* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;			//����2or��16/9��ʵ�ʲ���������ģ�������������Ч�����
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 	//I2C�ı���ĵ�ַ��Ϊ7λ��ַ�����ֻ��Ҫ������������һ������
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;         		//Ӧ��ʹ��
	 
	/* I2C��Ѱַģʽ */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //���õ�ַΪ7λ��ַ
	
	/* ͨ������ */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;					//��������
  
	/* I2C ��ʼ�� */
  I2C_Init(EEPROM_I2Cx, &I2C_InitStructure);
  
	/* ʹ�� I2C */
  I2C_Cmd(EEPROM_I2Cx, ENABLE);   
}


/**
  * @brief  I2C ����(EEPROM)��ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_EE_Init(void)		//I2C ����(EEPROM)��ʼ��
{

  I2C_GPIO_Config(); 		//I2C  GPIO�����ʼ��
 
  I2C_Mode_Configu();		//I2C  ��ʼ��

/* ����ͷ�ļ�i2c_ee.h�еĶ�����ѡ��EEPROM���豸��ַ */
#ifdef EEPROM_Block0_ADDRESS
  /* ѡ�� EEPROM Block0 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS  
	/* ѡ�� EEPROM Block1 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS  
	/* ѡ�� EEPROM Block2 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS  
	/* ѡ�� EEPROM Block3 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}


/**
  * @brief   ���������е�����д��I2C EEPROM��
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite) /*u8 u16 u32�������stm32f10x.h�ļ��н��ж����ˣ�
																																				ʵ�ʾ���uint8_t��uint16_t��uint32_t  */
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}


/**
  * @brief   дһ���ֽڵ�I2C EEPROM��
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ 
  * @retval  ��
  */
uint32_t I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)  														/*u8 u16 u32�������stm32f10x.h�ļ��н��ж����ˣ�
																																									ʵ�ʾ���uint8_t��uint16_t��uint32_t  */
{
  /* Send STRAT condition */				
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																							//������ʼ�ź�

  I2CTimeout = I2CT_FLAG_TIMEOUT;  																										//�ȴ���ʱ
  /* Test on EV5 and clear it */																	
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  									//���EV5�¼�����������������һ����I2C1��һ���Ǽ��EV5
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);												//�����ʱ���ͷ���I2C�ȴ���ʱ��Ϣ
  } 
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;																											//�ȴ���ʱ
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);  		 //����EEPROM��д��ַ
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))     //���EV6�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);												//�����ʱ���ͷ���I2C�ȴ���ʱ��Ϣ1
  }  
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2Cx, WriteAddr);																								//��������EEPROM�ڲ����ֽ�д��ַ
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;																											//�ȴ���ʱ
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))       				//���EV8_2�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);												//�����ʱ���ͷ���I2C�ȴ���ʱ��Ϣ2
  } 
  
  /* Send the byte to be written */
  I2C_SendData(EEPROM_I2Cx, *pBuffer); 																								//����Ҫд����ֽ�
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;  																										//�ȴ�д�����
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))      				//���EV8_2�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
  
  /* Send STOP condition */
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);																							//ֹͣλ
  
  return 1;
}


/**
  * @brief   ��EEPROM��һ��дѭ���п���д����ֽڣ���һ��д����ֽ���
  *          ���ܳ���EEPROMҳ�Ĵ�С��AT24C02ÿҳ��8���ֽ�
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
uint32_t I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)										//д�����ֽ�
{
  I2CTimeout = I2CT_LONG_TIMEOUT;																													//��ʱ����

  while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))   																	//I2Cæ������ʱ֮�󷵻س�ʱ��Ϣ4
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
  } 
  
  /* Send START condition */
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);			//������ʼ�ź�
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;							//��ʱ����
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  //���EV5�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);   				//��ʱ֮�󷵻س�ʱ��Ϣ5
  } 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);	//��������EEPROM��д��ַ
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//��ʱ����
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))    //���EV6�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);	//��ʱ֮�󷵻س�ʱ��Ϣ6
  } 
  
  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(EEPROM_I2Cx, WriteAddr);  														//��������EEPROM�ڲ����ֽ�д��ַ

  I2CTimeout = I2CT_FLAG_TIMEOUT;						//��ʱ����
  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))    //���EV8�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);	//��ʱ֮�󷵻س�ʱ��Ϣ7
  } 

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(EEPROM_I2Cx, *pBuffer);    //���͵�ǰ�ֽڵ�ַ

    /* Point to the next byte to be written */
    pBuffer++;   														//д�뵱ǰ�ֽڵ���һ���ֽڵ�ַ
  
    I2CTimeout = I2CT_FLAG_TIMEOUT;						//��ʱ����

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))   //���EV8�¼�
    {
      if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);	//��ʱ֮�󷵻س�ʱ��Ϣ8
    } 
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);   //����ֹͣλ
  
  return 1;
}


/**
  * @brief   ��EEPROM�����ȡһ������ 
  * @param   
  *		@arg pBuffer:��Ŵ�EEPROM��ȡ�����ݵĻ�����ָ��
  *		@arg WriteAddr:�������ݵ�EEPROM�ĵ�ַ
  *     @arg NumByteToWrite:Ҫ��EEPROM��ȡ���ֽ���
  * @retval  ��
  */
uint32_t I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
  
  I2CTimeout = I2CT_LONG_TIMEOUT;						//��ʱ����
  
  //*((u8 *)0x4001080c) |=0x80; 
  while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);	//��ʱ֮�󷵻س�ʱ��Ϣ9
   }
  
  /* Send START condition */
	 I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);   //������ʼλ
  //*((u8 *)0x4001080c) &=~0x80;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//��ʱ����
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))   //���EV5�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);	//��ʱ֮�󷵻س�ʱ��Ϣ10
   }
  
  /* Send EEPROM address for write */
	 I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);   //��������EEPROM��д��ַ

  I2CTimeout = I2CT_FLAG_TIMEOUT;						//��ʱ����
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))   //���EV6�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);	//��ʱ֮�󷵻س�ʱ��Ϣ11
   }
    
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(EEPROM_I2Cx, ENABLE);   															//���EV6

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2Cx, ReadAddr);  									  //��������EEPROM���ڲ����ֽ�д��ַ 

   
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//��ʱ����
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))   //���EV8�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);	//��ʱ֮�󷵻س�ʱ��Ϣ12
   }
    
  /* Send STRAT condition a second time */  
	 I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);   //������ʼ�ź�
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//��ʱ����
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))   //���EV5�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);	//��ʱ֮�󷵻س�ʱ��Ϣ13
   }
    
  /* Send EEPROM address for read */   //����
	 I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Receiver);   //��������EEPROM��д��ַ
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//��ʱ����
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))   //���EV6�¼�
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);	//��ʱ֮�󷵻س�ʱ��Ϣ14
   }
  
  /* While there is data to be read */
  while(NumByteToRead)  																				   //������
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);									//Ӧ��ʧ��
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);												//����ֹͣλ
    }

    /* Test on EV7 and clear it */    
    I2CTimeout = I2CT_LONG_TIMEOUT;						//��ʱ����
    
		while(I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  //���EV7�¼�
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);	//��ʱ֮�󷵻س�ʱ��Ϣ3
		} 
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(EEPROM_I2Cx);						//��EEPROM��һ���ֽ�

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 																												//����һ���ֽ�
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        																		//�ݼ����ֽ�
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);							//�´ν�������ʹ��
  
    return 1;
}


/**
  * @brief  Wait for EEPROM Standby state 
  * @param  ��
  * @retval ��
  */
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(EEPROM_I2Cx, I2C_FLAG_AF);
    /* STOP condition */    
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE); 
}




/**
  * @brief  Basic management of the timeout situation.   //�����ĳ�ʱ��������
  * @param  errorCode��������룬����������λ���ĸ����ڳ���.
  * @retval ����0����ʾIIC��ȡʧ��.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  EEPROM_ERROR("I2C �ȴ���ʱ!errorCode = %d",errorCode);   
  
  return 0;
}
/*********************************************END OF FILE**********************/

