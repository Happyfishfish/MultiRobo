#include "sys.h"
#include "handle.h"
#include "pstwo.h"
/**************************************************************************
作者：墨比斯科技
我的淘宝小店：https://moebius.taobao.com/
**************************************************************************/ 
u8 Flag_Left, Flag_Right, Flag_Direction = 0;		//蓝牙遥控相关的变量
u8 Flag_Stop = 1, Flag_Show = 0;					//停止标志位和 显示标志位 默认停止 显示打开
int Encoder_A, Encoder_B, Encoder_C, Encoder_D;		//编码器的脉冲计数
long int Position_A, Position_B, Position_C, Position_D, Rate_A, Rate_B, Rate_C, Rate_D; //PID控制相关变量
int Encoder_A_EXTI;									//通过外部中断读取的编码器数据                       
long int Motor_A, Motor_B, Motor_C, Motor_D;		//电机PWM变量
long int Target_A = 6, Target_B = 6, Target_C = 6, Target_D = 6;	//电机目标值
int Voltage;										//电池电压采样相关的变量
float Show_Data_Mb;									//全局显示变量，用于显示需要查看的数据                         
u8 delay_50, delay_flag;							//延时相关变量
u8 Run_Flag = 0;  									//蓝牙遥控相关变量和运行状态标志位
u8 rxbuf[8], Urxbuf[8], CAN_ON_Flag = 0, Usart_ON_Flag = 0, PS2_ON_Flag = 0, Usart_Flag, PID_Send, Flash_Send;  //CAN和串口控制相关变量
u8 txbuf[8], txbuf2[8], Turn_Flag;					//CAN发送相关变量
float Pitch, Roll, Yaw, Move_X, Move_Y, Move_Z;		//三轴角度和XYZ轴目标速度
u16 PID_Parameter[10], Flash_Parameter[10];			//Flash相关数组
float	Position_KP = 6, Position_KI = 0, Position_KD = 3;	//位置控制PID参数
float Velocity_KP = 10, Velocity_KI = 10;			//速度控制PID参数
int RC_Velocity = 30, RC_Position = 1000;			//设置遥控的速度和位置值
int Gryo_Z;
int henxian , shuxian;
int spx,spy;
int movemode = 0;
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

void SysTick_Init( void )
{
    /* SystemFrequency / 1000    1ms

     * SystemFrequency / 100000  10us

     * SystemFrequency / 1000000 1us

     */
    SysTick_Config(SystemCoreClock / 100000);
    SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


void Peripheral_Init()
{
	LED_Init();						//=====初始化与 LED 连接的硬件接口
	KEY_Init();						//=====按键初始化
	SysTick_Init();
	

	USART1_Init(115200);				//=====串口初始化
	USART2_Init(115200);				//=====蓝牙串口
	
	MiniBalance_PWM_Init(7199,0);	//=====电机驱动

	Adc_Init();

#if MPU6xxx_ENABLE
	IIC_Init();
	MPU6050_initialize();           //=====MPU6050初始化	
	DMP_Init();                     //=====初始化DMP   
#endif
	
#if OLED_DISPLAY_ENABLE
	OLED_Init();					//=====OLED初始化
#endif
	
#if ENCODER_ENABLE					//=====编码器初始化
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
int a,b,c,d;
void SearchRun(void)
{
	//四路都检测到
	
	a = SEARCH_Ml_IO;
	b = SEARCH_L_IO;
	c = SEARCH_R_IO;
	d = SEARCH_Mr_IO;
	if((SEARCH_Ml_IO == BLACK_AREA) && (SEARCH_L_IO == BLACK_AREA) && (SEARCH_R_IO == BLACK_AREA) && (SEARCH_Mr_IO == BLACK_AREA))
	{
		if (readflag == 0)
		{henxian++;readflag = 1;}
		if (henxian == 2)
		{movemode = !movemode;movecount++;henxian =0;}
		return;
	}
	if((SEARCH_R_IO == WHITE_AREA) && (SEARCH_Mr_IO == WHITE_AREA))
	{
		readflag = 0;
		return;
	}
}
void modechoose()
{
	if (movemode == 0 && movecount < 10)
	{spx = 2;spy = 0;}
	if (movemode == 1 && movecount < 10)
	{spx = -2;spy = 0;}
	if (movecount >= 10)
	{spx = 0;spy = 0;}
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
