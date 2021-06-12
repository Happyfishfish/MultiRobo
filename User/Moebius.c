#include "sys.h"
#include "handle.h"
#include "pstwo.h"
/**************************************************************************
?????i??????
?????????https://moebius.taobao.com/
**************************************************************************/ 
u8 Flag_Left, Flag_Right, Flag_Direction = 0;		//??????????i???
u8 Flag_Stop = 1, Flag_Show = 0;					//???????? ??????? I???? ?????
int Encoder_A, Encoder_B, Encoder_C, Encoder_D;		//???????????????
long int Position_A, Position_B, Position_C, Position_D, Rate_A, Rate_B, Rate_C, Rate_D; //PID??????????
int Encoder_A_EXTI;									//???????????i?????????                       
long int Motor_A, Motor_B, Motor_C, Motor_D;		//???PWM????
long int Target_A = 6, Target_B = 6, Target_C = 6, Target_D = 6;	//???????
int Voltage;										//????????????i???
float Show_Data_Mb;									//??????????????????????????????                         
u8 delay_50, delay_flag;							//?????????
u8 Run_Flag = 0;  									//?????????????????????????
u8 rxbuf[8], Urxbuf[8], CAN_ON_Flag = 0, Usart_ON_Flag = 0, PS2_ON_Flag = 0, Usart_Flag, PID_Send, Flash_Send;  //CAN??????????????
u8 txbuf[8], txbuf2[8], Turn_Flag;					//CAN??????????
float Pitch, Roll, Yaw, Move_X, Move_Y, Move_Z;		//???????XYZ????????
u16 PID_Parameter[10], Flash_Parameter[10];			//Flash???????
float	Position_KP = 6, Position_KI = 0, Position_KD = 3;	//??????PID????
float Velocity_KP = 10, Velocity_KI = 10;			//??????PID????
int RC_Velocity = 30, RC_Position = 1000;			//????????????????
int Gryo_Z;
int henxian , shuxian;
int spx,spy,spz;
int sumf,sumb;
int movemode = 0;
int	hrcounter = 0;;
char uartbuffer[50];
int32_t hal_nowtick = 0;
void PrintQueue(void);
/*************************************************
Function: Peripheral_Init()
Description: Init system peripheral
Input:  void
Output: void
Return: void
*************************************************/

void SysTick_Init( void )
{
    /* SystemFrequency / 1000    1ms

     * SystemFrequency / 100000  10us

     * SystemFrequency / 1000000 1us

     */
    SysTick_Config(SystemCoreClock / 1000);
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}



void SysTick_Handler(void){
	hal_nowtick++;
}

void Peripheral_Init()
{
	LED_Init();						//=====??'???? LED ???????????
	KEY_Init();						//=====??????'??
	SysTick_Init();
	

	USART1_Init(115200);				//=====?????'??
	USART2_Init(115200);				//=====????????
	
	MiniBalance_PWM_Init(7199,0);	//=====???????

	Adc_Init();

#if MPU6xxx_ENABLE
	IIC_Init();
	MPU6050_initialize();           //=====MPU6050??'??	
	DMP_Init();                     //=====??'??DMP   
#endif
	
#if OLED_DISPLAY_ENABLE
	OLED_Init();					//=====OLED??'??
#endif
	
#if ENCODER_ENABLE					//=====????????'??
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
int readflag = 0;
int movecount = 0;
int a,b,c,d,a1,b1,c1,d1;
extern float YawTarget;
int justTurned = 0;
void SearchRun(void)
{
	a = SEARCH_Ml_IO*1;a1 =	SEARCH_Ml_IO1*1;
	b = SEARCH_L_IO*20;b1 = SEARCH_L_IO1*20;
	c = SEARCH_R_IO*30;c1 = SEARCH_R_IO1*30;
	d = SEARCH_Mr_IO*100;d1 = SEARCH_Mr_IO1*100;
	sumf = a+b+c+d;sumb = a1+b1+c1+d1;
	if((SEARCH_Ml_IO == BLACK_AREA) && (SEARCH_L_IO == BLACK_AREA) && (SEARCH_R_IO == BLACK_AREA) && (SEARCH_Mr_IO == BLACK_AREA))
	{
		if (readflag == 0 && justTurned ==0)
		{henxian++;readflag = 1;}
		if (henxian == 2)
		{movemode = !movemode;movecount++;henxian =0;justTurned = 20000;}
		return;
	}
	else
	{
		readflag = 0;
		if( sumf > 60){spy = -1;}
		if( sumf < 40){spy = 1;}
		if(sumf == 50){spy = 0;}
//		if(sumf == sumb)
//		{
//			spz = 0;
//			if(sumf > 25){spy = -1;}
//			if(sumf < 25){spy = 1;}
//			if(sumf == 50){spy = 0;}
//			sumf = 0;sumb = 0;
//		}
//		if((sumf != sumb) && (sumb !=151))
//		{
//			if(sumf < sumb)
//			{
//				spz = 1;
//			}
//			if(sumf > sumb)
//			{
//				spz = -1;
//			}
//			sumf = 0;sumb = 0;
//		}
	}
}
void modechoose()
{
	static int added = 0;
	justTurned > 0 ? justTurned-- : justTurned;
	if (movemode == 0 && movecount < 15)
	{spx = 2;added = 0;}
	if (movemode == 1 && movecount < 15){
		if ((!added) && (justTurned == 0)){	
			added = 1;
			YawTarget += 90;
			movemode = 0;
		}
	}
	if (movecount >= 15)
	{spx = 0;spy = 0;spz = 0;}
}
int main(void)
{
	Peripheral_Init();
	Kinematic_Analysis(0,0,0);
	delay_ms(5000);
	while (1)
	{	
		handle();
		SearchRun();
		modechoose();
		asdf++;
	}
}
