#ifndef __I2C_EE_H
#define	__I2C_EE_H


#include "stm32f10x.h"


/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* AT24C01/02ÿҳ��8���ֽ� 
 * AT24C04/08A/16Aÿҳ��16���ֽ� 
 */
	

#define EEPROM_DEV_ADDR			0xA0		/* 24xx02���豸��ַ */  /* A0/A1/A2 ��Ϊ0������EEPROM ��7 λ�豸��ַ�ǣ�101
																			0000b ����0x50������I2C ͨѶʱ�����ǵ�ַ����д��������һ�𹹳�һ��8 λ�����ҵ�
																			R/WλΪ0 ʱ����ʾд�������Լ���7 λ��ַ����ֵΪ��0xA0�������Ƹ�ֵΪI2C �豸
																			�ġ�д��ַ������R/WλΪ1 ʱ����ʾ�����򣬼���7 λ��ַ����ֵΪ��0xA1�������Ƹ�
																			ֵΪ������ַ��*/
#define EEPROM_PAGE_SIZE		  8			  /* 24xx02��ҳ���С */
#define EEPROM_SIZE				  256			  /* 24xx02������ */


uint8_t ee_CheckOk(void);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void ee_Erase(void);
uint8_t ee_Test(void);


#endif /* __I2C_EE_H */
