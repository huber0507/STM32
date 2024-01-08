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


#define OLED_ADDRESS	0x78 													//ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78

void I2C_Configuration(void);    										//����CPU��Ӳ��I2C
void I2C_WriteByte(uint8_t addr,uint8_t data);			//��Ĵ�����ַдһ��byte������
void WriteCmd(unsigned char I2C_Command);						//д����
void WriteDat(unsigned char I2C_Data);							//д����
void OLED_Init(void);																 //OLED��ʼ��
void OLED_SetPos(unsigned char x, unsigned char y);		//������ʼ������  
void OLED_Fill(unsigned char fill_Data);						//ȫ�����
void OLED_CLS(void);																//����
void OLED_ON(void);																	//����
void OLED_OFF(void);																//˯��
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);		 //��ʾ�ַ���(�����С��6*8��8*16����)
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);																	 //��ʾ����(������Ҫ��ȡģ��Ȼ��ŵ�codetab.h��)
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);			 //BMPͼƬ

#endif
