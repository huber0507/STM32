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

/* AT24C01/02每页有8个字节 
 * AT24C04/08A/16A每页有16个字节 
 */
	

#define EEPROM_DEV_ADDR			0xA0		/* 24xx02的设备地址 */  /* A0/A1/A2 均为0，所以EEPROM 的7 位设备地址是：101
																			0000b ，即0x50。由于I2C 通讯时常常是地址跟读写方向连在一起构成一个8 位数，且当
																			R/W位为0 时，表示写方向，所以加上7 位地址，其值为“0xA0”，常称该值为I2C 设备
																			的“写地址”；当R/W位为1 时，表示读方向，加上7 位地址，其值为“0xA1”，常称该
																			值为“读地址”*/
#define EEPROM_PAGE_SIZE		  8			  /* 24xx02的页面大小 */
#define EEPROM_SIZE				  256			  /* 24xx02总容量 */


uint8_t ee_CheckOk(void);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void ee_Erase(void);
uint8_t ee_Test(void);


#endif /* __I2C_EE_H */
