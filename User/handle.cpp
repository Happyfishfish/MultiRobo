#include "handle.h"
#include "pid.h"
#include "string.h"
#include <stdio.h>

#define bufferLenth 50

#define ABS(x) ((x) > 0 ? (x) : -(x))

extern char uartbuffer[bufferLenth];
extern int bufferPosition;

uint8_t dataNoArrive = 0;

extern "C" void delay_ms(uint32_t i);

//extern float Pitch, Roll, Yaw;
extern float integralYaw;
pid yawPID(0.5,0,0,0,100);
float YawTarget = 0;
float PIDresult;
extern int spx,spy,spz;
int movex,movey;
int movez;

extern "C" void Kinematic_Analysis(float Vx,float Vy,float Vz);
static int readUART(){
	static char command[20];
	memset(command,0,sizeof(command));
	if (bufferPosition < 11){
		//skip for now :)
		;
	}
	else{
		if (uartbuffer[bufferPosition-1] == 'E' && uartbuffer[bufferPosition-10] == 'S'){
			uartbuffer[bufferPosition-1] == '0';
			dataNoArrive = 0;
			//x,y frame
			//memcpy(command,(const void*)uartbuffer[bufferPosition-11],10);
			movex = (uartbuffer[bufferPosition-8] - '0') * 100 + (uartbuffer[bufferPosition-7] - '0') * 10 + (uartbuffer[bufferPosition-6] - '0');
			movey = (uartbuffer[bufferPosition-4] - '0') * 100 + (uartbuffer[bufferPosition-3] - '0') * 10 + (uartbuffer[bufferPosition-2] - '0');
			movex-=50;movey-=50;movex*=2;movey*=2;
			movex/=10;movey/=10;
			movey*=-1;
			//sscanf(command,"Sx%dy%d",&movex,&movey);
			
		}
		else if(uartbuffer[bufferPosition-1] == 'E' && uartbuffer[bufferPosition-6] == 'S'){
			uartbuffer[bufferPosition-1] == '0';
			dataNoArrive = 0;
			movez = (uartbuffer[bufferPosition-4] - '0') * 100 + (uartbuffer[bufferPosition-3] - '0') * 10 + (uartbuffer[bufferPosition-2] - '0');
			movez-=50;
			//z frame
			YawTarget = integralYaw + (movez + 0.0)/4.0;
			movez = 0;
		}
	}
	return 0;
}
int notstarted = 1;
int handle(){
	static float lastValue = -10;
	if (dataNoArrive > 190){
		movex = 0;movey = 0;
	}
	if (notstarted && ABS(integralYaw-lastValue) > 0.3){
		lastValue = integralYaw;
		Kinematic_Analysis(0,0,0);
		delay_ms(1000);
		return 1;
	}
	else if (notstarted){
		YawTarget = integralYaw;
		notstarted = 0;
	}

	static int speed = 0;
	speed++;
	speed = speed > 30 ? 0 : speed;
	PIDresult = yawPID.pid_run(YawTarget - integralYaw);
	readUART();
//	Kinematic_Analysis(0,0,2);
	Kinematic_Analysis(movex,movey,PIDresult);
	// Kinematic_Analysis(spx,spy,PIDresult);
	//Kinematic_Analysis(speed,0,0);
	return 0;
}
