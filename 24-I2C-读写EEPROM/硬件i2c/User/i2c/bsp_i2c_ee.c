/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   i2c EEPROM(AT24C02)应用函数bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./i2c/bsp_i2c_ee.h"
#include "./usart/bsp_usart.h"	
#include "bsp_SysTick.h"




uint16_t EEPROM_ADDRESS;    //定义eeprom

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   


static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


/**
  * @brief  I2C I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* 使能与 I2C 有关的时钟 */
	EEPROM_I2C_APBxClock_FUN ( EEPROM_I2C_CLK, ENABLE );    						//开启I2C的时钟
	EEPROM_I2C_GPIO_APBxClock_FUN ( EEPROM_I2C_GPIO_CLK, ENABLE );     //开启I2C外设GPIOB的时钟  实际对应的是I2C的GPIOB的时钟
	
    
  /* I2C_SCL、I2C_SDA*/
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN;     							//配置I2C的SCL引脚号为PB6
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//配置速率
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;		       							/* 开漏输出   （实际更改为其他配置也可以输出，
																																				此句有误，在野火视频中提到STM32其他版本的GPIO可以配置其他模式，
																																				这个F103不行,在1-STM32F10x-中文参考手册.pdf中
																																				第八章8.1.11 外设的GPIO配置中提到各种外设的GPIO口配置要求中为开漏输出）*/
  GPIO_Init(EEPROM_I2C_SCL_PORT, &GPIO_InitStructure);								//GPIOB初始化
	
  GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SDA_PIN;										//配置I2C的SDA引脚号为PB7
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//配置速率
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;		       							/* 开漏输出   （实际更改为其他配置也可以输出，
																																				此句有误，在野火视频中提到STM32其他版本的GPIO可以配置其他模式，
																																				这个F103不行,在1-STM32F10x-中文参考手册.pdf中
																																				第八章8.1.11 外设的GPIO配置中提到各种外设的GPIO口配置要求中为开漏输出）*/
  GPIO_Init(EEPROM_I2C_SDA_PORT, &GPIO_InitStructure);								//GPIOB初始化
		
	
}


/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Configu(void)			//I2C 工作模式配置
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				//配置模式
	
	/* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;			//配置2or（16/9）实际测试无需更改，两个配置起来效果差不多
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 	//I2C的本身的地址，为7位地址，这个只需要跟其他器件不一样即可
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;         		//应答使能
	 
	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //配置地址为7位地址
	
	/* 通信速率 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;					//配置速率
  
	/* I2C 初始化 */
  I2C_Init(EEPROM_I2Cx, &I2C_InitStructure);
  
	/* 使能 I2C */
  I2C_Cmd(EEPROM_I2Cx, ENABLE);   
}


/**
  * @brief  I2C 外设(EEPROM)初始化
  * @param  无
  * @retval 无
  */
void I2C_EE_Init(void)		//I2C 外设(EEPROM)初始化
{

  I2C_GPIO_Config(); 		//I2C  GPIO外设初始化
 
  I2C_Mode_Configu();		//I2C  初始化

/* 根据头文件i2c_ee.h中的定义来选择EEPROM的设备地址 */
#ifdef EEPROM_Block0_ADDRESS
  /* 选择 EEPROM Block0 来写入 */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS  
	/* 选择 EEPROM Block1 来写入 */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS  
	/* 选择 EEPROM Block2 来写入 */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS  
	/* 选择 EEPROM Block3 来写入 */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}


/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite) /*u8 u16 u32这个是在stm32f10x.h文件中进行定义了，
																																				实际就是uint8_t、uint16_t、uint32_t  */
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
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)  														/*u8 u16 u32这个是在stm32f10x.h文件中进行定义了，
																																									实际就是uint8_t、uint16_t、uint32_t  */
{
  /* Send STRAT condition */				
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);																							//产生起始信号

  I2CTimeout = I2CT_FLAG_TIMEOUT;  																										//等待超时
  /* Test on EV5 and clear it */																	
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  									//检测EV5事件，其中两个参数，一个是I2C1，一个是检测EV5
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);												//如果超时，就返回I2C等待超时信息
  } 
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;																											//等待超时
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);  		 //发送EEPROM的写地址
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))     //检测EV6事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);												//如果超时，就返回I2C等待超时信息1
  }  
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2Cx, WriteAddr);																								//主机发送EEPROM内部的字节写地址
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;																											//等待超时
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))       				//检测EV8_2事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);												//如果超时，就返回I2C等待超时信息2
  } 
  
  /* Send the byte to be written */
  I2C_SendData(EEPROM_I2Cx, *pBuffer); 																								//发送要写入的字节
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;  																										//等待写入完成
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))      				//检测EV8_2事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
  
  /* Send STOP condition */
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);																							//停止位
  
  return 1;
}


/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)										//写入多个字节
{
  I2CTimeout = I2CT_LONG_TIMEOUT;																													//超时处理

  while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))   																	//I2C忙处理，超时之后返回超时信息4
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
  } 
  
  /* Send START condition */
  I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);			//发送起始信号
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;							//超时处理
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  //检测EV5事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);   				//超时之后返回超时信息5
  } 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);	//主机发送EEPROM的写地址
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//超时处理
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))    //检测EV6事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);	//超时之后返回超时信息6
  } 
  
  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(EEPROM_I2Cx, WriteAddr);  														//主机发送EEPROM内部的字节写地址

  I2CTimeout = I2CT_FLAG_TIMEOUT;						//超时处理
  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))    //检测EV8事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);	//超时之后返回超时信息7
  } 

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(EEPROM_I2Cx, *pBuffer);    //发送当前字节地址

    /* Point to the next byte to be written */
    pBuffer++;   														//写入当前字节的下一个字节地址
  
    I2CTimeout = I2CT_FLAG_TIMEOUT;						//超时处理

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))   //检测EV8事件
    {
      if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);	//超时之后返回超时信息8
    } 
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);   //发送停止位
  
  return 1;
}


/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
uint32_t I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
  
  I2CTimeout = I2CT_LONG_TIMEOUT;						//超时处理
  
  //*((u8 *)0x4001080c) |=0x80; 
  while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);	//超时之后返回超时信息9
   }
  
  /* Send START condition */
	 I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);   //发送起始位
  //*((u8 *)0x4001080c) &=~0x80;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//超时处理
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))   //检测EV5事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);	//超时之后返回超时信息10
   }
  
  /* Send EEPROM address for write */
	 I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);   //主机发送EEPROM的写地址

  I2CTimeout = I2CT_FLAG_TIMEOUT;						//超时处理
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))   //检测EV6事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);	//超时之后返回超时信息11
   }
    
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(EEPROM_I2Cx, ENABLE);   															//清除EV6

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2Cx, ReadAddr);  									  //主机发送EEPROM的内部的字节写地址 

   
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//超时处理
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))   //检测EV8事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);	//超时之后返回超时信息12
   }
    
  /* Send STRAT condition a second time */  
	 I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);   //发送起始信号
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//超时处理
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))   //检测EV5事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);	//超时之后返回超时信息13
   }
    
  /* Send EEPROM address for read */   //发送
	 I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Receiver);   //主机发送EEPROM的写地址
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;						//超时处理
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))   //检测EV6事件
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);	//超时之后返回超时信息14
   }
  
  /* While there is data to be read */
  while(NumByteToRead)  																				   //读数据
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(EEPROM_I2Cx, DISABLE);									//应答失败
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);												//发送停止位
    }

    /* Test on EV7 and clear it */    
    I2CTimeout = I2CT_LONG_TIMEOUT;						//超时处理
    
		while(I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  //检测EV7事件
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);	//超时之后返回超时信息3
		} 
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(EEPROM_I2Cx);						//从EEPROM读一个字节

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 																												//读下一个字节
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        																		//递减读字节
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(EEPROM_I2Cx, ENABLE);							//下次接收数据使能
  
    return 1;
}


/**
  * @brief  Wait for EEPROM Standby state 
  * @param  无
  * @retval 无
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
  * @brief  Basic management of the timeout situation.   //基本的超时条件管理
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  EEPROM_ERROR("I2C 等待超时!errorCode = %d",errorCode);   
  
  return 0;
}
/*********************************************END OF FILE**********************/

