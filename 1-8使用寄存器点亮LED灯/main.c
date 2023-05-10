#include "stm32f10x.h" 

int main(void)
{
	//开启GPIOB端口时钟
	RCC_APB2ENR |=(1<<3);
	
	//点亮绿灯 野火霸道开发板的绿灯是PB0
	
	//清空控制PB0的端口位
	GPIOB_CRL &= ~( 0x0F<< (4*0));	
	//配置PB0为通用推挽输出，速度10M
	GPIOB_CRL   |=(1<<(4*0));
	//PB0输出低电平
	GPIOB_ODR   |=(1<<0);
	
	//点亮蓝灯 野火霸道开发板的蓝灯是PB1
	
	//清空控制PB1的端口位
	GPIOB_ODR &=~(1<<0);
	//配置PB1为通用推挽输出，速度10M
	GPIOB_CRL |= (1<<4*1);

	// PB1 输出 低电平
	GPIOB_ODR &= ~(1<<1);
	
	//点亮红灯 野火霸道开发板的红灯是PB5
	
	//清空控制PB5的端口位
	GPIOB_CRL &= ~( 0x0F<< (4*5));	
	// 配置PB5为通用推挽输出，速度为10M
	GPIOB_CRL |= (1<<4*5);

	// PB5 输出 低电平
	GPIOB_ODR &= ~(1<<5);
	
	while(1);
}




//函数为空，目的是为了骗过编译器不报错

void SystemInit(void)
{
	
}
