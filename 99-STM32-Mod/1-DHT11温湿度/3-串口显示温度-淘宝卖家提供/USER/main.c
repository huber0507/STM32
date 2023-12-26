#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "DHT11.h"

/*
*读取温湿度传感器DHT11的值，并用串口打印出来
*/

void clock_init(void);

u8 temp = 0,humi = 0;


/**************************************************************************
函数名：int main(void)
参数说明：无
返回值：无
函数作用：主函数
***************************************************************************/
 int main(void)
 {	
	 
   clock_init();
	 uart_init(115200);		//初始化串口
	 printf("wecome to DHT11");
	 
	 
	 //初始化DHT11(有BUG，第一次上电总是失败，按一下复位按钮又能进了)
	 if(!DHT11_Init()){
		 printf("\r\n EEROR! THE DHT11 HAS NO RESPOND...");
		 //while(1);
	 }
	 printf("\r\n THE DHT11 HAS RESPOND");
	 Delay_ms(10);		//这里延时10ms主要是因为，刚刚接收到响应信息，要等DHT11发送完信息
	 
	 while(1)
	 {
		 if(DHT11_Read_Data(&temp,&humi))
			printf("\r\n temp:%d,humi:%d",temp,humi);
		 else
		 //printf("\r\n EEROR! THE DHT11 HAS NO RESPOND...");//由于是库函数编程，不能准确把握函数的执行时间，
																													//所以会经常出现这条警告信息
		 Delay_ms(100);
	 }
	  
 }
 
 
/**************************************************************************
函数名：void clock_init(void)
参数说明：无
返回值：无
函数作用：开启高速外部时钟，
ADCCLK设置为12MHZ，	SYSCLK设置为72Mhz，PCLK1设置为36MHZ，PKLC2设置为72mhz
***************************************************************************/
 void clock_init(void)
{
	RCC->CR = 0x1010000;
	RCC->CFGR = 0x1DC402;
}

 
 
 
	

