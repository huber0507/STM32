#include "hc_sr04.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"           //ʹ�õ���bվ���ƴ����ʱ����,�����Լ��ñ�������ʱ����

uint8_t flag=0;				//���ڼ�¼�ж��ź��������ػ����½���
uint32_t number=0;			//��¼��ʱ���жϵĴ���
uint32_t times=0;			//��¼�����źŵĳ���ʱ��

static void HC_SR04_GPIO_Init(void)
{
    
    //��ʼ��GPIO��,Trigʹ���������,Echoʹ�ø�������
    HC_SR04_SCK_APBxClock_FUN (HC_SR04_GPIO_CLK,ENABLE);        //ʹ��GPIOA������ʱ��
    GPIO_InitTypeDef itd;
    itd.GPIO_Mode=GPIO_Mode_Out_PP;                             //ѡ���������ģʽ
    itd.GPIO_Pin=HC_SR04_TRIG_PIN;                                    //ѡ��GPIO_Pin_6
    itd.GPIO_Speed=GPIO_Speed_50MHz;                            //Ĭ��ѡ��50MHz
    GPIO_Init(HC_SR04_GPIO_PORT ,&itd);
    
    itd.GPIO_Mode=GPIO_Mode_IN_FLOATING;                        //ѡ�񸡿�����ģʽ
    itd.GPIO_Pin=HC_SR04_ECHO_PIN;                                    //ѡ��GPIO_Pin_7
    GPIO_Init(HC_SR04_GPIO_PORT ,&itd);
    
    //AFIOӳ���ж�����
    HC_SR04_SCK_APBxClock_FUN(HC_SR04_GPIO_AFIO_CLK,ENABLE);         //ʹ��AFIO������ʱ��
    HC_SR04_GPIO_EXTI_FUN(HC_SR04_GPIO_ITSOURCE,HC_SR04_GPIO_ITCHANNEL);  //ѡ���ⲿ�ж�Դ���ж�ͨ��
	
}



static void HC_SR04_EXTI_Init(void)
{
    //EXTI�ж�����
    EXTI_InitTypeDef itd1;
    itd1.EXTI_Line=HC_SR04_EXTI_LINE;                                  //echoʹ�õĶ˿�7,���ѡ��7���ж���
    itd1.EXTI_LineCmd=ENABLE;
    itd1.EXTI_Mode=EXTI_Mode_Interrupt;
    itd1.EXTI_Trigger=EXTI_Trigger_Rising_Falling;              //�����غ��½��ض������ж�
    EXTI_Init(&itd1);
	
}



static void HC_SR04_NVIC_Init(void)
{

    //NVIC�����ⲿ�жϵ��ж����ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //ָ���жϷ���
    NVIC_InitTypeDef itd2;
    itd2.NVIC_IRQChannel=HC_SR04_NVIC_IRQChannel;                          //ʹ�õĶ˿�7,���ѡ���������
    itd2.NVIC_IRQChannelCmd=ENABLE;
    itd2.NVIC_IRQChannelPreemptionPriority=2;                   //��ռ���ȼ�
    itd2.NVIC_IRQChannelSubPriority=2;                          //��Ӧ���ȼ� 
    NVIC_Init(&itd2);

}



static void HC_SR04_TIM_Init(void)
{
    //���ö�ʱ��
    HC_SR04_TIM_SCK_APBxClock_FUN(HC_SR04_TIM_SCK,ENABLE);
    TIM_TimeBaseInitTypeDef itd3;
    itd3.TIM_ClockDivision=TIM_CKD_DIV1;                        //ʹ��ʱ�ӷ�Ƶ1
    itd3.TIM_CounterMode=TIM_CounterMode_Up;                    //���ϼ���
    //72MHz/72/100=1000,ÿ�붨ʱ������1000��,���ÿ������Ϊ100us
    itd3.TIM_Period=72-1;                                       //Ԥ��Ƶϵ��
    itd3.TIM_Prescaler=100-1;                                   //�Զ���װ��
    itd3.TIM_RepetitionCounter=0;                               //�ò��������߼���ʱ��ʹ��
    TIM_TimeBaseInit(HC_SR04_TIM,&itd3);
    TIM_ITConfig(HC_SR04_TIM,TIM_IT_Update,ENABLE);                    //ʹ���ж�����ź�
    TIM_InternalClockConfig(HC_SR04_TIM);                              //ѡ���ڲ�ʱ��    
}


static void HC_SR04_NVIC_TIM_Init(void)
{
    
    //NVIC���䶨ʱ�����ж����ȼ�
    NVIC_InitTypeDef itd4;
    itd4.NVIC_IRQChannel=TIM2_IRQn;                             //ָ��Tim2���ж�ͨ��
    itd4.NVIC_IRQChannelCmd=ENABLE;
    itd4.NVIC_IRQChannelPreemptionPriority=1;                   //��ռ���ȼ�
    itd4.NVIC_IRQChannelSubPriority=1;                          //��Ӧ���ȼ�
    NVIC_Init(&itd4);
}



//��ʱ���жϺ���
void TIM2_IRQHandler(void){
    if(SET==TIM_GetITStatus(HC_SR04_TIM,TIM_FLAG_Update)){
        number++;                                   //ÿ���жϽ�����++
        TIM_ClearITPendingBit(HC_SR04_TIM,TIM_FLAG_Update);
    }
}
 
//�ⲿ�жϺ���
void EXTI9_5_IRQHandler(void){
	
    if(SET==EXTI_GetITStatus(HC_SR04_EXTI_LINE)){
        if(flag==0){
            //�����ؼ������ƽ��ʼ,�򿪼�����
            number=0;flag=1;
            TIM_SetCounter(HC_SR04_TIM,0);
            TIM_Cmd(HC_SR04_TIM,ENABLE);
            
        }else{
            //�½��ؼ������ƽ����,ͳ�Ƹߵ�ƽ����ʱ��
            TIM_Cmd(HC_SR04_TIM,DISABLE);
            flag=0;
            times=number*100+TIM_GetCounter(HC_SR04_TIM);  //�õ�����ĸߵ�ƽ������us
        }
        EXTI_ClearITPendingBit(HC_SR04_EXTI_LINE);
    }
}

/**
  * ��    ����HC_SR04 TRIGд�ߵ͵�ƽ
  * ��    ����Ҫд��TRIG�ĵ�ƽֵ����Χ��0/1
  * �� �� ֵ����
  * ˵    �������ϲ㺯����ҪдTRIGʱ���˺����ᱻ����
  *           �û���Ҫ���ݲ��������ֵ����TRIG��Ϊ�ߵ�ƽ���ߵ͵�ƽ
  *           ����������0ʱ����TRIGΪ�͵�ƽ������������1ʱ����TRIGΪ�ߵ�ƽ
  */
void HC_SR04_W_Trig(uint8_t BitValue)
{
	/*����BitValue��ֵ����TRIG�øߵ�ƽ���ߵ͵�ƽ*/
	GPIO_WriteBit(HC_SR04_GPIO_PORT , HC_SR04_TRIG_PIN , (BitAction)BitValue);
	
	/*�����Ƭ���ٶȹ��죬���ڴ����������ʱ���Ա��ⳬ��I2Cͨ�ŵ�����ٶ�*/
	//...
}

unsigned int distance_mearsure_FUN(void)
{
			uint32_t distance=0;	
        for(int i=0;i<10;++i){              //ÿ��ȡ10�β������,ȡƽ��ֵ�������
            HC_SR04_W_Trig(1);
						DelayUs(15); 											 //����˵����,��Ҫ�ṩ����10us�ĸߵ�ƽ
            HC_SR04_W_Trig(0);
						DelayMs(65);										 //����˵����,ÿ������������Ҫ�ȴ�60ms
            distance+=(times/5.8);          //����˵�����ṩ�Ĺ�ʽ,��ȡ��λΪmm�ľ���
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

