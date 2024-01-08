#ifndef __OLED_H
#define __OLED_H

//void OLED_Init(void);
//void OLED_Clear(void);
//void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
//void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
//void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
//void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
//void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
//void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);


#define OLED_ADDRESS	0x78 													//通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void I2C_Configuration(void);    										//配置CPU的硬件I2C
void I2C_WriteByte(uint8_t addr,uint8_t data);			//向寄存器地址写一个byte的数据
void WriteCmd(unsigned char I2C_Command);						//写命令
void WriteDat(unsigned char I2C_Data);							//写数据
void OLED_Init(void);																 //OLED初始化
void OLED_SetPos(unsigned char x, unsigned char y);		//设置起始点坐标  
void OLED_Fill(unsigned char fill_Data);						//全屏填充
void OLED_CLS(void);																//清屏
void OLED_ON(void);																	//唤醒
void OLED_OFF(void);																//睡眠
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);		 //显示字符串(字体大小有6*8和8*16两种)
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);																	 //显示中文(中文需要先取模，然后放到codetab.h中)
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);			 //BMP图片

#endif
