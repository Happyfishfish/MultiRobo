#include "Systick.h"

void delay_us(u32 i)
{
	u32 temp;
	SysTick->LOAD=9*i;		 //������װ��ֵ, 72MHZʱ
	SysTick->CTRL=0X01;		 //ʹ�ܣ����������޶����������ⲿʱ��Դ
	SysTick->VAL=0;		   	 //���������
	do
	{
		temp=SysTick->CTRL;		   //��ȡ��ǰ������ֵ
	}
	while((temp&0x01)&&(!(temp&(1<<16))));	 //�ȴ�ʱ�䵽��
	SysTick->CTRL=0;	//�رռ�����
	SysTick->VAL=0;		//��ռ�����
}

void delay_ms(u32 i)
{
	u16 j = 0;
	while(i--){
		j = 4000;
		while(j--);
	}
//	u32 temp;
//	SysTick->LOAD=9000*i;	  //������װ��ֵ, 72MHZʱ
//	SysTick->VAL=0;			//���������
//	SysTick->CTRL=0X01;		//ʹ�ܣ����������޶����������ⲿʱ��Դ
//	
//	do
//	{
//		temp=SysTick->CTRL;	   //��ȡ��ǰ������ֵ
//	}
//	while((temp&0x01)&&(!(temp&(1<<16))));	//�ȴ�ʱ�䵽��
//	SysTick->CTRL=0;	//�رռ�����
//	SysTick->VAL=0;		//��ռ�����
}
