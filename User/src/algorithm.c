#include "algorithm.h"
#include "control.h"
#include "infrare.h"
#include "ultrasonic.h"
#include "stm32f10x_gpio.h"



/*******************
***** ����ģʽ *****
********************/
void BTestMode(void)
{
	static enum BTestStep bTestStep = BT_TEST_0;
	static int spCnt = 0;
	static int spThr = 20;
	static int stopInertia = 10;


	//static int last_La = 1000;
	//static int last_Ra = 1000;
	//static int last_Lc = 1000;
	//static int last_Rc = 1000;
	//static int etc = 5;
	

	switch (bTestStep) {
	case BT_TEST_0:
	{
		// ���Խ׶�-0
		Steer(0, 5);
		spCnt += (La << 1), spCnt += (Ra << 1), spCnt += (Lc << 1), spCnt += (Rc << 1);
		if (spCnt > 0)
			spCnt--;
		if (spCnt > spThr)
			bTestStep = BT_TEST_1;
		//last_La = La == 1 ? 0 : last_La + 1;
		//last_Ra = Ra == 1 ? 0 : last_Ra + 1;
		//last_Lc = Lc == 1 ? 0 : last_Lc + 1;
		//last_Rc = Rc == 1 ? 0 : last_Rc + 1;
		//if (last_La < etc && last_Ra < etc && last_Lc < etc && last_Rc < etc)
		//	bTestStep = BT_TEST_1;
		break;
	}
	case BT_TEST_1:
	{
		// ���Խ׶�-1
		if (stopInertia > 0)
			Steer(0, 4), stopInertia--;
		else
			Steer(0, 0);
		break;
	}
	case BT_TEST_2:
	{
		// ���Խ׶�-2
		Steer(0, 0);
		break;
	}
	default:
		break;
	}
}


/*******************
***** ��ʾģʽ *****
********************/
void BDemoMode(void)
{
	static enum BDemoStep bDemoStep = BD_INIT;
	static int demoCount = 0;
	static int demoTurning = 0;
	static int demoIntervalCount = 0;
	static float demoMoveSpeed = 4;
	static float demoTurnSpeed = 5;
	static int demoTurnRoundCnt = 850;

	switch (bDemoStep) {
	case BD_INIT:
	{
		// ��ʼ���׶�
		bDemoStep = BD_FORWARD;
		break;
	}

	case BD_FORWARD:
	{
		// ǰ���׶�
		Steer(0, demoMoveSpeed);
		if (++demoCount > 250)
			bDemoStep = BD_FPAUSE;
		break;
	}

	case BD_FPAUSE:
	{
		// ��ͣ�׶�
		Steer(0, 0);
		if (++demoCount > 350)
			bDemoStep = BD_BACKWARD;
		break;
	}

	case BD_BACKWARD:
	{
		// ���˽׶�
		Steer(0, -demoMoveSpeed);
		if (++demoCount > 600)
			bDemoStep = BD_BPAUSE;
		break;
	}
	case BD_BPAUSE:
	{
		// ��ͣ�׶�
		Steer(0, 0);
		if (++demoCount > 700)
			bDemoStep = BD_LEFTWARD;
		break;
	}

	case BD_LEFTWARD:
	{
		// ��ת�׶�
		if (demoTurning == 0) {
			Steer(-demoTurnSpeed, 0);
			g_iLeftTurnRoundCnt = -demoTurnRoundCnt;
			g_iRightTurnRoundCnt = demoTurnRoundCnt;
			demoTurning = 1;
		}
		else {
			if ((g_iLeftTurnRoundCnt > 0) && (g_iRightTurnRoundCnt < 0))
			{
				bDemoStep = BD_LPAUSE;
				Steer(0, 0);
				demoTurning = 0;
			}
			else {
				Steer(-demoTurnSpeed, 0);
			}
		}
		break;
	}
	case BD_LPAUSE:
	{
		// ��ͣ�׶�
		demoIntervalCount += 1;
		Steer(0, 0);
		if (demoIntervalCount > 100) {
			demoIntervalCount = 0;
			bDemoStep = BD_RIGHTWARD;
		}
		break;
	}
	case BD_RIGHTWARD:
	{
		// ��ת�׶�
		if (demoTurning == 0) {
			Steer(demoTurnSpeed, 0);
			g_iLeftTurnRoundCnt = demoTurnRoundCnt;
			g_iRightTurnRoundCnt = -demoTurnRoundCnt;
			demoTurning = 1;
		}
		else {
			if ((g_iLeftTurnRoundCnt < 0) && (g_iRightTurnRoundCnt > 0))
			{
				bDemoStep = BD_DONE;
				Steer(0, 0);
				demoTurning = 0;
			}
			else {
				Steer(demoTurnSpeed, 0);
			}
		}
		break;
	}
	case BD_DONE:
	{
		// ��ɽ׶�
		Steer(0, 0);
		break;
	}
	default:
		break;
	}
}


/*******************
***** ����ģʽ *****
********************/
void BRaceMode(void)
{
	static enum BRaceStep bRaceStep = BR_INIT;
	
	static int last_La = 1000;
	static int last_Ra = 1000;
	static int last_Lc = 1000;
	static int last_Rc = 1000;
	static int etc = 5;

	static int spCnt = 0;
	static int spThr = 20;
	static int stopInertia = 10;

	switch (bRaceStep) {
	case BR_INIT:
	{
		// ��ʼ���׶�
		bRaceStep = BR_TRACE;
		break;
	}
	case BR_TRACE:
	{
		// ѭ���׶�
		float direct = 0;
		float speed = 0;
		if (Lc)
			direct = -10;
		// else if (result & Lb)
		//	 direct = -6;
		else if (La)
			direct = -4;
		else if (Rc)
			direct = 10;
		// else if (Rb)
			// direct = 6;
		else if (Ra)
			direct = 4;
		else
			direct = 0.0;
		speed = 3;
		Steer(direct, speed);

		last_La = La == 1 ? 0 : last_La + 1;
		last_Ra = Ra == 1 ? 0 : last_Ra + 1;
		last_Lc = Lc == 1 ? 0 : last_Lc + 1;
		last_Rc = Rc == 1 ? 0 : last_Rc + 1;
		if (last_La < etc && last_Ra < etc && last_Lc < etc && last_Rc < etc)
			bRaceStep = BR_AVOID;
		break;
	}
	case BR_AVOID:
	{
		// ���Ͻ׶�
		bRaceStep = BR_DONE;	// TODO ���ԣ��ڴ˽׶�ֱ���������


		spCnt += (La << 1), spCnt += (Ra << 1), spCnt += (Lc << 1), spCnt += (Rc << 1);
		if (spCnt > 0)
			spCnt--;
		if (spCnt > spThr)
			bTestStep = BT_TEST_1;


		break;
	}
	case BR_DONE:
	{
		// ��ɽ׶�
		Steer(0, 0);
		break;
	}
	default:
		break;
	}
}




