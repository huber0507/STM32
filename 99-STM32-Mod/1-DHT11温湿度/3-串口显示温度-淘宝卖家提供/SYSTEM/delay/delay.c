#include "delay.h"

u32 SysTic_count = 100;
//使用系统滴答计时器精准延时
 void Delay_ms(u16 ms)
 {
	 SysTic_count = ms;
   
	 SysTick_Config(72000);			//默认设置为sysclock，1s计数72000000次，则1ms中断一次
	 while( SysTic_count!= 0);//等待计时完成
	 SysTick->CTRL = 0;  //关闭滴答计时器
 }
 
 /* 定时微秒级函数 */
 void Delay_us(u32 us)
 {
	 SysTic_count = us;
   
	 SysTick_Config(72);			//默认设置为sysclock，1s计数72000000次，则1us中断一次
	 while( SysTic_count!= 0);//等待计时完成
	 SysTick->CTRL = 0;  //关闭滴答计时器
 }

 /* sysTick中断处理函数 */
 void SysTick_Handler(void)
 {
	 if(SysTic_count != 0) SysTic_count--;
 }






































