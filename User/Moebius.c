#include "sys.h"
#include "handle.h"
/**************************************************************************
���ߣ�ī��˹�Ƽ�
�ҵ��Ա�С�꣺https://moebius.taobao.com/
**************************************************************************/ 
u8 Flag_Left, Flag_Right, Flag_Direction = 0;		//����ң����صı���
u8 Flag_Stop = 1, Flag_Show = 0;					//ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder_A, Encoder_B, Encoder_C, Encoder_D;		//���������������
long int Position_A, Position_B, Position_C, Position_D, Rate_A, Rate_B, Rate_C, Rate_D; //PID������ر���
int Encoder_A_EXTI;									//ͨ���ⲿ�ж϶�ȡ�ı���������                       
long int Motor_A, Motor_B, Motor_C, Motor_D;		//���PWM����
long int Target_A = 6, Target_B = 6, Target_C = 6, Target_D = 6;	//���Ŀ��ֵ
int Voltage;										//��ص�ѹ������صı���
float Show_Data_Mb;									//ȫ����ʾ������������ʾ��Ҫ�鿴������                         
u8 delay_50, delay_flag;							//��ʱ��ر���
u8 Run_Flag = 0;  									//����ң����ر���������״̬��־λ
u8 rxbuf[8], Urxbuf[8], CAN_ON_Flag = 0, Usart_ON_Flag = 0, PS2_ON_Flag = 0, Usart_Flag, PID_Send, Flash_Send;  //CAN�ʹ��ڿ�����ر���
u8 txbuf[8], txbuf2[8], Turn_Flag;					//CAN������ر���
float Pitch, Roll, Yaw, Move_X, Move_Y, Move_Z;		//����ǶȺ�XYZ��Ŀ���ٶ�
u16 PID_Parameter[10], Flash_Parameter[10];			//Flash�������
float	Position_KP = 6, Position_KI = 0, Position_KD = 3;	//λ�ÿ���PID����
float Velocity_KP = 10, Velocity_KI = 10;			//�ٶȿ���PID����
int RC_Velocity = 30, RC_Position = 1000;			//����ң�ص��ٶȺ�λ��ֵ
int PS2_LX, PS2_LY, PS2_RX, PS2_RY, PS2_KEY;
int Gryo_Z;
int	hrcounter = 0;;
char uartbuffer[50];
void PrintQueue(void);
/*************************************************
Function: Peripheral_Init()
Description: Init system peripheral
Input:  void
Output: void
Return: void
*************************************************/
void Peripheral_Init()
{
	LED_Init();						//=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	KEY_Init();						//=====������ʼ��

	USART1_Init(115200);				//=====���ڳ�ʼ��
	USART2_Init(115200);				//=====��������
	
	MiniBalance_PWM_Init(7199,0);	//=====�������

	Adc_Init();
	
	PS2_Init();

#if MPU6xxx_ENABLE
	IIC_Init();
	MPU6050_initialize();           //=====MPU6050��ʼ��	
	DMP_Init();                     //=====��ʼ��DMP   
#endif
	
#if OLED_DISPLAY_ENABLE
	OLED_Init();					//=====OLED��ʼ��
#endif
	
#if ENCODER_ENABLE					//=====��������ʼ��
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	Encoder_Init_TIM4();
	Encoder_Init_TIM5();
#endif
	EXTI15_Init();
}

/*************************************************
Function: main()
Description: Main function entry
Input:  void
Output: void
Return: void
*************************************************/
int asdf = 0;
int main(void)
{
		int i;
	
	
	Peripheral_Init();

//	for(i = 0;i<15;i++)
	delay_ms(500);
	while (1)
	{	
		handle();
		asdf++;
//		printf("%s",uartbuffer);
//		printf("usart1 online");
		delay_ms(500);
	}
}
