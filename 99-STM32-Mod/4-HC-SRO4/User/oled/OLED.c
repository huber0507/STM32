#include "stm32f10x.h"
#include "OLED_Font.h"
#include "OLED.h"            //使用的是b站江科大的OLED驱动代码,用于展示测距结果,可以在相应的地方更换为串口通信展示到电脑的串口助手上
#include "delay.h"

///*引脚配置*/
//#define OLED_W_SCL(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
//#define OLED_W_SDA(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))

/*引脚初始化*/
//void OLED_I2C_Init(void)
//{
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	OLED_W_SCL(1);
//	OLED_W_SDA(1);
//}


void I2C_Configuration(void)			//配置CPU的硬件I2C
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/*STM32F103C8T6芯片的硬件I2C: PB6 -- SCL; PB7 -- SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C1);//使用I2C1
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//主机的I2C地址,随便写的
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}


void I2C_WriteByte(uint8_t addr,uint8_t data)   //向寄存器地址写一个byte的数据
{
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C1, data);//发送数据
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
}

void WriteCmd(unsigned char I2C_Command)//写命令
{
	I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
	I2C_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void)   //OLED初始化
{
	DelayMs(100); //这里的延时很重要
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标  
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)    //唤醒
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)   //睡眠
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)  //显示字符串(字体大小有6*8和8*16两种)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}



//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)   //显示中文(中文需要先取模，然后放到codetab.h中)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])  //BMP图片
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}


///**
//  * @brief  I2C开始
//  * @param  无
//  * @retval 无
//  */
//void OLED_I2C_Start(void)
//{
//	OLED_W_SDA(1);
//	OLED_W_SCL(1);
//	OLED_W_SDA(0);
//	OLED_W_SCL(0);
//}

///**
//  * @brief  I2C停止
//  * @param  无
//  * @retval 无
//  */
//void OLED_I2C_Stop(void)
//{
//	OLED_W_SDA(0);
//	OLED_W_SCL(1);
//	OLED_W_SDA(1);
//}

///**
//  * @brief  I2C发送一个字节
//  * @param  Byte 要发送的一个字节
//  * @retval 无
//  */
//void OLED_I2C_SendByte(uint8_t Byte)
//{
//	uint8_t i;
//	for (i = 0; i < 8; i++)
//	{
//		OLED_W_SDA(Byte & (0x80 >> i));
//		OLED_W_SCL(1);
//		OLED_W_SCL(0);
//	}
//	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
//	OLED_W_SCL(0);
//}

///**
//  * @brief  OLED写命令
//  * @param  Command 要写入的命令
//  * @retval 无
//  */
//void OLED_WriteCommand(uint8_t Command)
//{
//	OLED_I2C_Start();
//	OLED_I2C_SendByte(0x78);		//从机地址
//	OLED_I2C_SendByte(0x00);		//写命令
//	OLED_I2C_SendByte(Command); 
//	OLED_I2C_Stop();
//}

///**
//  * @brief  OLED写数据
//  * @param  Data 要写入的数据
//  * @retval 无
//  */
//void OLED_WriteData(uint8_t Data)
//{
//	OLED_I2C_Start();
//	OLED_I2C_SendByte(0x78);		//从机地址
//	OLED_I2C_SendByte(0x40);		//写数据
//	OLED_I2C_SendByte(Data);
//	OLED_I2C_Stop();
//}

///**
//  * @brief  OLED设置光标位置
//  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
//  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
//  * @retval 无
//  */
//void OLED_SetCursor(uint8_t Y, uint8_t X)
//{
//	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
//	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
//	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
//}

///**
//  * @brief  OLED清屏
//  * @param  无
//  * @retval 无
//  */
//void OLED_Clear(void)
//{  
//	uint8_t i, j;
//	for (j = 0; j < 8; j++)
//	{
//		OLED_SetCursor(j, 0);
//		for(i = 0; i < 128; i++)
//		{
//			OLED_WriteData(0x00);
//		}
//	}
//}

///**
//  * @brief  OLED显示一个字符
//  * @param  Line 行位置，范围：1~4
//  * @param  Column 列位置，范围：1~16
//  * @param  Char 要显示的一个字符，范围：ASCII可见字符
//  * @retval 无
//  */
//void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
//{      	
//	uint8_t i;
//	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
//	for (i = 0; i < 8; i++)
//	{
//		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
//	}
//	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
//	for (i = 0; i < 8; i++)
//	{
//		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
//	}
//}

///**
//  * @brief  OLED显示字符串
//  * @param  Line 起始行位置，范围：1~4
//  * @param  Column 起始列位置，范围：1~16
//  * @param  String 要显示的字符串，范围：ASCII可见字符
//  * @retval 无
//  */
//void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
//{
//	uint8_t i;
//	for (i = 0; String[i] != '\0'; i++)
//	{
//		OLED_ShowChar(Line, Column + i, String[i]);
//	}
//}

///**
//  * @brief  OLED次方函数
//  * @retval 返回值等于X的Y次方
//  */
//uint32_t OLED_Pow(uint32_t X, uint32_t Y)
//{
//	uint32_t Result = 1;
//	while (Y--)
//	{
//		Result *= X;
//	}
//	return Result;
//}

///**
//  * @brief  OLED显示数字（十进制，正数）
//  * @param  Line 起始行位置，范围：1~4
//  * @param  Column 起始列位置，范围：1~16
//  * @param  Number 要显示的数字，范围：0~4294967295
//  * @param  Length 要显示数字的长度，范围：1~10
//  * @retval 无
//  */
//void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
//{
//	uint8_t i;
//	for (i = 0; i < Length; i++)							
//	{
//		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
//	}
//}

///**
//  * @brief  OLED显示数字（十进制，带符号数）
//  * @param  Line 起始行位置，范围：1~4
//  * @param  Column 起始列位置，范围：1~16
//  * @param  Number 要显示的数字，范围：-2147483648~2147483647
//  * @param  Length 要显示数字的长度，范围：1~10
//  * @retval 无
//  */
//void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
//{
//	uint8_t i;
//	uint32_t Number1;
//	if (Number >= 0)
//	{
//		OLED_ShowChar(Line, Column, '+');
//		Number1 = Number;
//	}
//	else
//	{
//		OLED_ShowChar(Line, Column, '-');
//		Number1 = -Number;
//	}
//	for (i = 0; i < Length; i++)							
//	{
//		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
//	}
//}

///**
//  * @brief  OLED显示数字（十六进制，正数）
//  * @param  Line 起始行位置，范围：1~4
//  * @param  Column 起始列位置，范围：1~16
//  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
//  * @param  Length 要显示数字的长度，范围：1~8
//  * @retval 无
//  */
//void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
//{
//	uint8_t i, SingleNumber;
//	for (i = 0; i < Length; i++)							
//	{
//		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
//		if (SingleNumber < 10)
//		{
//			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
//		}
//		else
//		{
//			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
//		}
//	}
//}

///**
//  * @brief  OLED显示数字（二进制，正数）
//  * @param  Line 起始行位置，范围：1~4
//  * @param  Column 起始列位置，范围：1~16
//  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
//  * @param  Length 要显示数字的长度，范围：1~16
//  * @retval 无
//  */
//void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
//{
//	uint8_t i;
//	for (i = 0; i < Length; i++)							
//	{
//		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
//	}
//}

///**
//  * @brief  OLED初始化
//  * @param  无
//  * @retval 无
//  */
//void OLED_Init(void)
//{
//	uint32_t i, j;
//	
//	for (i = 0; i < 1000; i++)			//上电延时
//	{
//		for (j = 0; j < 1000; j++);
//	}
//	
//	OLED_I2C_Init();			//端口初始化
//	
//	OLED_WriteCommand(0xAE);	//关闭显示
//	
//	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
//	OLED_WriteCommand(0x80);
//	
//	OLED_WriteCommand(0xA8);	//设置多路复用率
//	OLED_WriteCommand(0x3F);
//	
//	OLED_WriteCommand(0xD3);	//设置显示偏移
//	OLED_WriteCommand(0x00);
//	
//	OLED_WriteCommand(0x40);	//设置显示开始行
//	
//	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
//	
//	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

//	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
//	OLED_WriteCommand(0x12);
//	
//	OLED_WriteCommand(0x81);	//设置对比度控制
//	OLED_WriteCommand(0xCF);

//	OLED_WriteCommand(0xD9);	//设置预充电周期
//	OLED_WriteCommand(0xF1);

//	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
//	OLED_WriteCommand(0x30);

//	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

//	OLED_WriteCommand(0xA6);	//设置正常/倒转显示

//	OLED_WriteCommand(0x8D);	//设置充电泵
//	OLED_WriteCommand(0x14);

//	OLED_WriteCommand(0xAF);	//开启显示
//		
//	OLED_Clear();				//OLED清屏
//}
