
#ifndef __BSP_LED_H
#define __BSP_LED_H


//#define EN_LED_GPIO_CLK()	{__HAL_RCC_GPIOH_CLK_ENABLE();\
//							__GPIOH_CLK_ENABLE;}
#define EN_LED_GPIO_CLK()	__HAL_RCC_GPIOH_CLK_ENABLE()

//��λms
#define LED_OFF_SHORT		3
#define LED_OFF_MEDIUM		5
#define LED_OFF_LONG		7


#define LED_ON_SHORT		3
#define LED_ON_MEDIUM		5
#define LED_ON_LONG			7

#define LED_ON		1
#define LED_OFF		0

#define LED_MULTI_COLOR   1//Ұ��Ķ�ɫ�ʵƣ���ֹ����һ��ɫ�ʳ���
#define LED_IS_LOW_LEVEL_ACTIVE		1  //�Ƿ��ǵ͵�ƽ����

#define LED_RED_PIN GPIO_PIN_10
#define LED_RED_PORT GPIOH

#define LED_GREEN_PIN GPIO_PIN_11
#define LED_GREEN_PORT GPIOH

#define LED_BLUE_PIN GPIO_PIN_12
#define LED_BLUE_PORT GPIOH

#define LED_NUM		3

typedef enum{
	LED_BLINK_SLOW = 0,
	LED_BLINK_GENERAL,
	LED_BLINK_FAST,
}LedOprate_e;

typedef enum{
	LED_RED = 0,
	LED_GREEN,
	LED_BLUE,
}LedID_e;

typedef enum{
	LED_BLINK_CYCLIC = 0, //ѭ��ģʽ
	LED_BLINK_TIMES,  //���
}LED_BLINK_MODE_e;

typedef struct{
	LED_BLINK_MODE_e Mode;
	uint8_t Times; //On-Off Ϊһ��ѭ������
	uint8_t State;		//��ǰ״̬
	uint8_t OnReload;		//����״̬��װֵ
	uint8_t OnCnt;			//on״̬�ļƴ�
	uint8_t OffReload;
	uint8_t OffCnt;
}LedBlink_t;

void Bsp_LedInit(void);
void Bsp_LedOn(LedID_e _eLedName);
void Bsp_LedOff(LedID_e _eLedName);
uint8_t Bsp_LedGet(LedID_e _eLedName);
void Bsp_LedToggle(LedID_e _eLedName);
void Bsp_LedTest(void);
void Bsp_LedBlink(LedID_e _eLedName, LedOprate_e _eMode, uint8_t _ucTimes);
void Bsp_LedBlinkOff(LedID_e _eLedName);
void Bsp_LedLoop100ms(void);



#endif //__BSP_LED_H

