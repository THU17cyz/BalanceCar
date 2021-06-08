#include "algorithm.h"
#include "control.h"
#include "ultrasonic.h"

enum BDemoStep g_bDemoStep = BD_FORWARD;
enum BRaceStep g_bRaceStep = BR_INIT;

int demoCount = 0;
int demoTurning = 0;
int demoIntervalCount = 0;
float demoMoveSpeed = 4;
float demoTurnSpeed = 5;
int demoTurnRoundCnt = 850;


void BTestMode(void)
{
	if (IsUltraOK()) UltraControl(1);
}

void BDemoMode(void)
{
	switch (g_bDemoStep) {
	case BD_INIT:
		break;
	case BD_FORWARD:
		demoCount += 1;
		Steer(0, demoMoveSpeed);
		if(demoCount>250){
			g_bDemoStep = BD_FPAUSE;
		}
		break;
	case BD_FPAUSE:
		demoCount += 1;
		Steer(0, 0);
		if(demoCount>350){
			g_bDemoStep = BD_BACKWARD;
		}
		break;
	case BD_BACKWARD:
		demoCount += 1;
		Steer(0, -demoMoveSpeed);
		if(demoCount>600){
			g_bDemoStep = BD_BPAUSE;
		}
		break;
	case BD_BPAUSE:
		demoCount += 1;
		Steer(0, 0);
		if(demoCount>700){
			g_bDemoStep = BD_LEFTWARD;
		}
		break;
	case BD_LEFTWARD:
		if(demoTurning==0){
			Steer(-demoTurnSpeed, 0);
			g_iLeftTurnRoundCnt = -demoTurnRoundCnt;
			g_iRightTurnRoundCnt = demoTurnRoundCnt;
			demoTurning = 1;
		}
		else{
			if((g_iLeftTurnRoundCnt > 0)&&(g_iRightTurnRoundCnt < 0))
			{
				g_bDemoStep = BD_LPAUSE;
				Steer(0,0);
				demoTurning = 0;
			}
			else{
				Steer(-demoTurnSpeed, 0);
			}
		}
		break;
	case BD_LPAUSE:
		demoIntervalCount += 1;
		Steer(0, 0);
		if(demoIntervalCount>100){
			demoIntervalCount = 0;
			g_bDemoStep = BD_RIGHTWARD;
		}
		break;
	case BD_RIGHTWARD:
		if(demoTurning==0){
			Steer(demoTurnSpeed, 0);
			g_iLeftTurnRoundCnt = demoTurnRoundCnt;
			g_iRightTurnRoundCnt = -demoTurnRoundCnt;
			demoTurning = 1;
		}
		else{
			if((g_iLeftTurnRoundCnt < 0)&&(g_iRightTurnRoundCnt > 0))
			{
				g_bDemoStep = BD_DONE;
				Steer(0,0);
				demoTurning = 0;
			}
			else{
				Steer(demoTurnSpeed, 0);
			}
		}
		break;
	case BD_DONE:
		Steer(0,0);
		break;
	default:
		break;
	}
}

void BRaceMode(void)
{
	switch (g_bRaceStep) {
	case BR_INIT:
		Steer(0, 4);
		g_bRaceStep = BR_AVOID;
		break;
	case BR_TRACE:
		break;
	case BR_AVOID:
		if(IsUltraOK()) UltraControl(1);
		break;
	case BR_DONE:
		break;
	default:
		break;
	}
}
