#include "stm32f10x.h" 

int main(void)
{
	//����GPIOB�˿�ʱ��
	RCC_APB2ENR |=(1<<3);
	//��տ���PB0�Ķ˿�λ
	GPIOB_CRL   &= ~(<<(4*0));
	//����PB0Ϊͨ������������ٶ�10M
	GPIOB_CRL   |=(1<<(4*0));
	//PB0����͵�ƽ
	GPIOB_ODR   |=(0<<0);
	
	while(1);
}




//����Ϊ�գ�Ŀ����Ϊ��ƭ��������������

void SystemInit(void)
{
	
}
