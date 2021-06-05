#include "handle.h"
#include "pid.h"
#include "string.h"
#include <stdio.h>
#define bufferLenth 50

#define ABS(x) ((x) > 0 ? (x) : -(x))

extern char uartbuffer[bufferLenth];
extern int bufferPosition;

uint8_t dataNoArrive = 0;

extern float Pitch, Roll, Yaw;
pid yawPID(0.5,0,0,0,100);
float YawTarget = 15;
float PIDresult;
extern int spx,spy;
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
			dataNoArrive = 0;
			//x,y frame
			//memcpy(command,(const void*)uartbuffer[bufferPosition-11],10);
			movex = (uartbuffer[bufferPosition-8] - '0') * 100 + (uartbuffer[bufferPosition-7] - '0') * 10 + (uartbuffer[bufferPosition-6] - '0');
			movey = (uartbuffer[bufferPosition-4] - '0') * 100 + (uartbuffer[bufferPosition-3] - '0') * 10 + (uartbuffer[bufferPosition-2] - '0');
			movex-=50;movey-=50;movex*=2;movey*=2;
			movex/=10;movey/=10;
			//sscanf(command,"Sx%dy%d",&movex,&movey);
			
		}
		else if(uartbuffer[bufferPosition-1] == 'E' && uartbuffer[bufferPosition-6] == 'S'){
			dataNoArrive = 0;
			movez = (uartbuffer[bufferPosition-4] - '0') * 100 + (uartbuffer[bufferPosition-3] - '0') * 10 + (uartbuffer[bufferPosition-2] - '0');
			//z frame
//			YawTarget += movez/100.0;
			movez = 0;
		}
	}
	return 0;
}
int notstarted = 1;
int handle(){
	if (dataNoArrive > 190){
		movex = 0;movey = 0;
	}
	if (notstarted && ABS(Yaw-11) > 1){
		Kinematic_Analysis(0,0,0);
		return 1;
	}
	else{
		notstarted = 0;
	}

	static int speed = 0;
	speed++;
	speed = speed > 30 ? 0 : speed;
	PIDresult = yawPID.pid_run(YawTarget - Yaw);
	readUART();
	//Kinematic_Analysis(movex,movey,PIDresult);
	Kinematic_Analysis(spx,0,0);
	//Kinematic_Analysis(speed,0,0);
	return 0;
}
