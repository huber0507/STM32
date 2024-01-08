/**
  ******************************************************************************
  * @file    main.c
  * @author  WEI
  * @version V1.0
  * @date    2024��1��7��
  * @brief   HC-SR04���������Գ���
  ******************************************************************************
  * @attention
  * 0.96��OLED SCL --> 5V
  *	�����ɫ�� --> GND	
  *	�����ɫ�� --> GPIO_Pin_6
  * �����ɫ�� --> 5V
  *	�����ɫ�� --> GND	
  *	�����ɫ�� --> GPIO_Pin_6	
  * �����ɫ�� --> 5V
  *	�����ɫ�� --> GND	
  *	�����ɫ�� --> GPIO_Pin_6	
  ******************************************************************************
  */
  
#include "stm32f10x.h"                  // Device header
#include "Delay.h"           //ʹ�õ���bվ���ƴ����ʱ����,�����Լ��ñ�������ʱ����
#include "OLED.h"            //ʹ�õ���bվ���ƴ��OLED��������,����չʾ�����,��������Ӧ�ĵط�����Ϊ����ͨ��չʾ�����ԵĴ���������
 
uint8_t flag=0;				//���ڼ�¼�ж��ź��������ػ����½���
uint32_t number=0;			//��¼��ʱ���жϵĴ���
uint32_t times=0;			//��¼�����źŵĳ���ʱ��
 


/*****************************************END****************************************/

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
		unsigned char i;
		extern const unsigned char BMP1[];
	
		DelayInit();
		I2C_Configuration();
		OLED_Init();
    
    //��ʼ��GPIO��,Trigʹ���������,Echoʹ�ø�������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);        //ʹ��GPIOA������ʱ��
    GPIO_InitTypeDef itd;
    itd.GPIO_Mode=GPIO_Mode_Out_PP;                             //ѡ���������ģʽ
    itd.GPIO_Pin=GPIO_Pin_6;                                    //ѡ��GPIO_Pin_6
    itd.GPIO_Speed=GPIO_Speed_50MHz;                            //Ĭ��ѡ��50MHz
    GPIO_Init(GPIOA,&itd);
    
    itd.GPIO_Mode=GPIO_Mode_IN_FLOATING;                        //ѡ�񸡿�����ģʽ
    itd.GPIO_Pin=GPIO_Pin_7;                                    //ѡ��GPIO_Pin_7
    GPIO_Init(GPIOA,&itd);
    
    //AFIOӳ���ж�����
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);         //ʹ��AFIO������ʱ��
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);  //ѡ���ⲿ�ж�Դ���ж�ͨ��
    
    //EXTI�ж�����
    EXTI_InitTypeDef itd1;
    itd1.EXTI_Line=EXTI_Line7;                                  //echoʹ�õĶ˿�7,���ѡ��7���ж���
    itd1.EXTI_LineCmd=ENABLE;
    itd1.EXTI_Mode=EXTI_Mode_Interrupt;
    itd1.EXTI_Trigger=EXTI_Trigger_Rising_Falling;              //�����غ��½��ض������ж�
    EXTI_Init(&itd1);
    
    //NVIC�����ⲿ�жϵ��ж����ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //ָ���жϷ���
    NVIC_InitTypeDef itd2;
    itd2.NVIC_IRQChannel=EXTI9_5_IRQn;                          //ʹ�õĶ˿�7,���ѡ���������
    itd2.NVIC_IRQChannelCmd=ENABLE;
    itd2.NVIC_IRQChannelPreemptionPriority=2;                   //��ռ���ȼ�
    itd2.NVIC_IRQChannelSubPriority=2;                          //��Ӧ���ȼ� 
    NVIC_Init(&itd2);
    
    //���ö�ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    TIM_TimeBaseInitTypeDef itd3;
    itd3.TIM_ClockDivision=TIM_CKD_DIV1;                        //ʹ��ʱ�ӷ�Ƶ1
    itd3.TIM_CounterMode=TIM_CounterMode_Up;                    //���ϼ���
    //72MHz/72/100=1000,ÿ�붨ʱ������1000��,���ÿ������Ϊ100us
    itd3.TIM_Period=72-1;                                       //Ԥ��Ƶϵ��
    itd3.TIM_Prescaler=100-1;                                   //�Զ���װ��
    itd3.TIM_RepetitionCounter=0;                               //�ò��������߼���ʱ��ʹ��
    TIM_TimeBaseInit(TIM2,&itd3);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                    //ʹ���ж�����ź�
    TIM_InternalClockConfig(TIM2);                              //ѡ���ڲ�ʱ��    
    
    //NVIC���䶨ʱ�����ж����ȼ�
    NVIC_InitTypeDef itd4;
    itd4.NVIC_IRQChannel=TIM2_IRQn;                             //ָ��Tim2���ж�ͨ��
    itd4.NVIC_IRQChannelCmd=ENABLE;
    itd4.NVIC_IRQChannelPreemptionPriority=1;                   //��ռ���ȼ�
    itd4.NVIC_IRQChannelSubPriority=1;                          //��Ӧ���ȼ�
    NVIC_Init(&itd4);
    
    uint32_t distance;
    while(1){
        distance=0;
        for(int i=0;i<10;++i){              //ÿ��ȡ10�β������,ȡƽ��ֵ�������
            GPIO_SetBits(GPIOA,GPIO_Pin_6);
//            Delay_us(15);                   //����˵����,��Ҫ�ṩ����10us�ĸߵ�ƽ
						DelayUs(15);
            GPIO_ResetBits(GPIOA,GPIO_Pin_6);
//            Delay_ms(65);                   //����˵����,ÿ������������Ҫ�ȴ�60ms
						DelayMs(65);										 //����˵����,ÿ������������Ҫ�ȴ�60ms
            distance+=(times/5.8);          //����˵�����ṩ�Ĺ�ʽ,��ȡ��λΪmm�ľ���
        }
        distance/=10;
				
//				OLED_Fill(0xFF);//ȫ������
//				DelayS(2);
//				OLED_Fill(0x00);//ȫ����
//				DelayS(2);
//				for(i=0;i<5;i++)
//				{
//					OLED_ShowCN(22+i*16,0,i);//������ʾ����
//				}
//				DelayS(2);
//				OLED_ShowStr(0,3,"HelTec Automation",1);//����6*8�ַ�
				OLED_ShowStr(0,4,"WMX0325E",2);				//����8*16�ַ�
				DelayS(2);
//				OLED_CLS();//����
//				OLED_OFF();//����OLED����
//				DelayS(2);
//				OLED_ON();//����OLED���ߺ���
//				OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
//				DelayS(2);
				
        OLED_ShowNum(2,1,distance,4);
    }
}


 
//��ʱ���жϺ���
void TIM2_IRQHandler(void){
    if(SET==TIM_GetITStatus(TIM2,TIM_FLAG_Update)){
        number++;                                   //ÿ���жϽ�����++
        TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);
    }
}
 
//�ⲿ�жϺ���
void EXTI9_5_IRQHandler(void){
    if(SET==EXTI_GetITStatus(EXTI_Line7)){
        if(flag==0){
            //�����ؼ������ƽ��ʼ,�򿪼�����
            number=0;flag=1;
            TIM_SetCounter(TIM2,0);
            TIM_Cmd(TIM2,ENABLE);
            
        }else{
            //�½��ؼ������ƽ����,ͳ�Ƹߵ�ƽ����ʱ��
            TIM_Cmd(TIM2,DISABLE);
            flag=0;
            times=number*100+TIM_GetCounter(TIM2);  //�õ�����ĸߵ�ƽ������us
        }
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
}
/*********************************************END OF FILE**********************/
