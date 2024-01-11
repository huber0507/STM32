#include "hc_sr04.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"           //使用的是b站江科大的延时函数,可以自己用别的替代延时功能

uint8_t flag=0;				//用于记录中断信号是上升沿还是下降沿
uint32_t number=0;			//记录定时器中断的次数
uint32_t times=0;			//记录回响信号的持续时间

static void HC_SR04_GPIO_Init(void)
{
    
    //初始化GPIO口,Trig使用推挽输出,Echo使用浮空输入
    HC_SR04_SCK_APBxClock_FUN (HC_SR04_GPIO_CLK,ENABLE);        //使能GPIOA的外设时钟
    GPIO_InitTypeDef itd;
    itd.GPIO_Mode=GPIO_Mode_Out_PP;                             //选择推挽输出模式
    itd.GPIO_Pin=HC_SR04_TRIG_PIN;                                    //选择GPIO_Pin_6
    itd.GPIO_Speed=GPIO_Speed_50MHz;                            //默认选择50MHz
    GPIO_Init(HC_SR04_GPIO_PORT ,&itd);
    
    itd.GPIO_Mode=GPIO_Mode_IN_FLOATING;                        //选择浮空输入模式
    itd.GPIO_Pin=HC_SR04_ECHO_PIN;                                    //选择GPIO_Pin_7
    GPIO_Init(HC_SR04_GPIO_PORT ,&itd);
    
    //AFIO映射中断引脚
    HC_SR04_SCK_APBxClock_FUN(HC_SR04_GPIO_AFIO_CLK,ENABLE);         //使能AFIO的外设时针
    HC_SR04_GPIO_EXTI_FUN(HC_SR04_GPIO_ITSOURCE,HC_SR04_GPIO_ITCHANNEL);  //选择外部中断源和中断通道
	
}



static void HC_SR04_EXTI_Init(void)
{
    //EXTI中断配置
    EXTI_InitTypeDef itd1;
    itd1.EXTI_Line=HC_SR04_EXTI_LINE;                                  //echo使用的端口7,因此选择7号中断线
    itd1.EXTI_LineCmd=ENABLE;
    itd1.EXTI_Mode=EXTI_Mode_Interrupt;
    itd1.EXTI_Trigger=EXTI_Trigger_Rising_Falling;              //上升沿和下降沿都触发中断
    EXTI_Init(&itd1);
	
}



static void HC_SR04_NVIC_Init(void)
{

    //NVIC分配外部中断的中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //指定中断分组
    NVIC_InitTypeDef itd2;
    itd2.NVIC_IRQChannel=HC_SR04_NVIC_IRQChannel;                          //使用的端口7,因此选择这个参数
    itd2.NVIC_IRQChannelCmd=ENABLE;
    itd2.NVIC_IRQChannelPreemptionPriority=2;                   //抢占优先级
    itd2.NVIC_IRQChannelSubPriority=2;                          //响应优先级 
    NVIC_Init(&itd2);

}



static void HC_SR04_TIM_Init(void)
{
    //配置定时器
    HC_SR04_TIM_SCK_APBxClock_FUN(HC_SR04_TIM_SCK,ENABLE);
    TIM_TimeBaseInitTypeDef itd3;
    itd3.TIM_ClockDivision=TIM_CKD_DIV1;                        //使用时钟分频1
    itd3.TIM_CounterMode=TIM_CounterMode_Up;                    //向上计数
    //72MHz/72/100=1000,每秒定时器计数1000个,因此每个计数为100us
    itd3.TIM_Period=72-1;                                       //预分频系数
    itd3.TIM_Prescaler=100-1;                                   //自动重装器
    itd3.TIM_RepetitionCounter=0;                               //该参数仅给高级定时器使用
    TIM_TimeBaseInit(HC_SR04_TIM,&itd3);
    TIM_ITConfig(HC_SR04_TIM,TIM_IT_Update,ENABLE);                    //使能中断输出信号
    TIM_InternalClockConfig(HC_SR04_TIM);                              //选择内部时钟    
}


static void HC_SR04_NVIC_TIM_Init(void)
{
    
    //NVIC分配定时器的中断优先级
    NVIC_InitTypeDef itd4;
    itd4.NVIC_IRQChannel=TIM2_IRQn;                             //指定Tim2的中断通道
    itd4.NVIC_IRQChannelCmd=ENABLE;
    itd4.NVIC_IRQChannelPreemptionPriority=1;                   //抢占优先级
    itd4.NVIC_IRQChannelSubPriority=1;                          //响应优先级
    NVIC_Init(&itd4);
}



//定时器中断函数
void TIM2_IRQHandler(void){
    if(SET==TIM_GetITStatus(HC_SR04_TIM,TIM_FLAG_Update)){
        number++;                                   //每次中断将次数++
        TIM_ClearITPendingBit(HC_SR04_TIM,TIM_FLAG_Update);
    }
}
 
//外部中断函数
void EXTI9_5_IRQHandler(void){
	
    if(SET==EXTI_GetITStatus(HC_SR04_EXTI_LINE)){
        if(flag==0){
            //上升沿即回响电平开始,打开计数器
            number=0;flag=1;
            TIM_SetCounter(HC_SR04_TIM,0);
            TIM_Cmd(HC_SR04_TIM,ENABLE);
            
        }else{
            //下降沿即回响电平结束,统计高电平持续时长
            TIM_Cmd(HC_SR04_TIM,DISABLE);
            flag=0;
            times=number*100+TIM_GetCounter(HC_SR04_TIM);  //得到回响的高电平持续的us
        }
        EXTI_ClearITPendingBit(HC_SR04_EXTI_LINE);
    }
}

/**
  * 函    数：HC_SR04 TRIG写高低电平
  * 参    数：要写入TRIG的电平值，范围：0/1
  * 返 回 值：无
  * 说    明：当上层函数需要写TRIG时，此函数会被调用
  *           用户需要根据参数传入的值，将TRIG置为高电平或者低电平
  *           当参数传入0时，置TRIG为低电平，当参数传入1时，置TRIG为高电平
  */
void HC_SR04_W_Trig(uint8_t BitValue)
{
	/*根据BitValue的值，将TRIG置高电平或者低电平*/
	GPIO_WriteBit(HC_SR04_GPIO_PORT , HC_SR04_TRIG_PIN , (BitAction)BitValue);
	
	/*如果单片机速度过快，可在此添加适量延时，以避免超出I2C通信的最大速度*/
	//...
}

unsigned int distance_mearsure_FUN(void)
{
			uint32_t distance=0;	
        for(int i=0;i<10;++i){              //每次取10次测距数据,取平均值减少误差
            HC_SR04_W_Trig(1);
						DelayUs(15); 											 //根据说明书,需要提供至少10us的高电平
            HC_SR04_W_Trig(0);
						DelayMs(65);										 //根据说明书,每个周期至少需要等待60ms
            distance+=(times/5.8);          //根据说明书提供的公式,获取单位为mm的距离
        }
        distance/=10;			
				return distance;			
}
				
				
void HC_SR04_Init(void)
{
		HC_SR04_GPIO_Init();
		HC_SR04_EXTI_Init();
		HC_SR04_NVIC_Init();
		HC_SR04_TIM_Init();
		HC_SR04_NVIC_TIM_Init();
}

