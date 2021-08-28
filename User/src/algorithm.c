/**********************************************************************

作    者:  黄翔 田丰源 从业臻
	
***********************************************************************/


#include "algorithm.h"
#include "control.h"
#include "infrare.h"
#include "ultrasonic.h"
#include "stm32f10x_gpio.h"
#include "manage.h"


/*******************
***** 测试模式 *****
********************/
void BTestMode(void)
{
	static enum BTestStep bTestStep = BT_TEST_0;
	static int spCnt = 0;
	static int spThr = 20;
	// static int stopInertia = 10;

	// static int last_La = 1000;
	// static int last_Ra = 1000;
	// static int last_Lc = 1000;
	// static int last_Rc = 1000;
	// static int etc = 5;

	static int avoidTime = 0;
	static const int avoidTimeThr = 20;

	switch (bTestStep) {
	case BT_TEST_0:
	{
		// 测试阶段-0
		g_CarRunningMode = ULTRA_AVOID_MODE;
		avoidTime++;

		UltraControl(1);


		if(avoidTime > avoidTimeThr && g_fCarAngle > -1 && g_fCarAngle < 1) 
		{
				spCnt += (La << 1), spCnt += (Ra << 1), spCnt += (Lc << 1), spCnt += (Rc << 1);
		}
		if (spCnt > 0)
			spCnt -= 2;
		if (spCnt > spThr)
			bTestStep = BT_TEST_1;

		/*
		last_La = La == 1 ? 0 : last_La + 1;
		last_Ra = Ra == 1 ? 0 : last_Ra + 1;
		last_Lc = Lc == 1 ? 0 : last_Lc + 1;
		last_Rc = Rc == 1 ? 0 : last_Rc + 1;
		if (avoidTime > avoidTimeThr && last_La < etc && last_Ra < etc && last_Lc < etc && last_Rc < etc)
			bTestStep = BT_TEST_1;
		*/
		break;
	}
	case BT_TEST_1:
	{
		// 测试阶段-1
		Steer(0, 0);
		break;
	}
	case BT_TEST_2:
	{
		// 测试阶段-2
		Steer(0, 0);
		break;
	}
	default:
		break;
	}
}


/*******************
***** 演示模式 *****
********************/
void BDemoMode(void)
{
	static enum BDemoStep bDemoStep = BD_INIT;
	static int demoCount = 0;
	static int demoTurning = 0;
	static int demoIntervalCount = 0;
	static float demoMoveSpeed = 4;
	static float demoTurnSpeed = 5;
	static int demoTurnRoundCnt = 1000;
	
	static int diff = 0;
	static int old_diff = 0;
	static int refine_dir = -1; // 0:左 1:右
	static int cur_state = -1;

	static int move_after_turn_cnt = 0;
	static int MOVE_COUNT = 20; //20

	switch (bDemoStep) {
	case BD_INIT:
	{
		// 初始化阶段
		Steer(0, 0);
		bDemoStep = BD_FORWARD;
		break;
	}

	case BD_FORWARD:
	{
		// 前进阶段
		diff = g_iLeftTurnRoundCnt - g_iRightTurnRoundCnt;
		diff = diff > 0 ? diff : -diff;
		
		if(diff > 5 || diff - old_diff > 2)
		{
			if(g_iLeftTurnRoundCnt - g_iRightTurnRoundCnt > 0)
			{
				bDemoStep = BD_REFINE;
				refine_dir = 0;
				cur_state = BD_FORWARD;
			}
			else {
				bDemoStep = BD_REFINE;
				refine_dir = 1;
				cur_state = BD_FORWARD;
			}
		}
		old_diff = diff;
		Steer(0, demoMoveSpeed);

		if (++demoCount > 210)
			bDemoStep = BD_FPAUSE;
		break;
	}

	case BD_FPAUSE:
	{
		// 暂停阶段
		Steer(0, 0);
		diff = g_iLeftTurnRoundCnt - g_iRightTurnRoundCnt;
		diff = diff > 0 ? diff : -diff;
		old_diff = diff;
		if (++demoCount > 310)
			bDemoStep = BD_BACKWARD;
		break;
	}

	case BD_BACKWARD:
	{
		// 后退阶段
		diff = g_iLeftTurnRoundCnt - g_iRightTurnRoundCnt;
		diff = diff > 0 ? diff : -diff;
		if(diff > 5 || diff - old_diff > 2)
		{
			if(g_iLeftTurnRoundCnt - g_iRightTurnRoundCnt > 0)
			{
				bDemoStep = BD_REFINE;
				refine_dir = 0;
				cur_state = BD_BACKWARD;
			}
			else
			{
				bDemoStep = BD_REFINE;
				refine_dir = 1;
				cur_state = BD_BACKWARD;
			}
		}
		old_diff = diff;
		Steer(0, -demoMoveSpeed);
		if (++demoCount > 560)
			bDemoStep = BD_BPAUSE;
		break;
	}
	case BD_BPAUSE:
	{
		// 暂停阶段
		Steer(0, 0);
		if (++demoCount > 660)
			bDemoStep = BD_LEFTWARD;
		break;
	}

	case BD_LEFTWARD:
	{
		// 左转阶段
		if (demoTurning == 0)
		{
			Steer(-demoTurnSpeed, 4);
			g_iLeftTurnRoundCnt = -demoTurnRoundCnt;
			g_iRightTurnRoundCnt = demoTurnRoundCnt;
			demoTurning = 1;
		}
		else {
			// 使用差 >/< 0的转弯结束判断更准确
			//if ((g_iLeftTurnRoundCnt > 0) && (g_iRightTurnRoundCnt < 0))
			if (g_iLeftTurnRoundCnt - g_iRightTurnRoundCnt > 0)
			{
				bDemoStep = BD_LFRONT;
				Steer(0, 4);
				demoTurning = 0;
			}
			else
			{
				Steer(-demoTurnSpeed, 4);
			}
		}
		break;
	}
	case BD_LFRONT:
	{
		if (++move_after_turn_cnt > MOVE_COUNT)
		{
			bDemoStep = BD_LPAUSE;
			move_after_turn_cnt = 0;
		}
		Steer(0, 2);
		break;
	}
	case BD_LPAUSE:
	{
		// 暂停阶段
		demoIntervalCount += 1;
		Steer(0, 0);
		if (demoIntervalCount > 100)
		{
			demoIntervalCount = 0;
			bDemoStep = BD_RIGHTWARD;
		}
		break;
	}
	case BD_RIGHTWARD:
	{
		// 右转阶段
		if (demoTurning == 0)
		{
			Steer(demoTurnSpeed, 4);
			demoTurnRoundCnt = 1100;
			g_iLeftTurnRoundCnt = demoTurnRoundCnt;
			g_iRightTurnRoundCnt = -demoTurnRoundCnt;
			demoTurning = 1;
		}
		else 
		{
			if (g_iLeftTurnRoundCnt - g_iRightTurnRoundCnt < 0)
			{
				bDemoStep = BD_RFRONT;
				Steer(0, 4);
				demoTurning = 2;
			}
			else
			{
				Steer(demoTurnSpeed, 4);
			}
		}
		break;
	}
	case BD_RFRONT:
	{
		if (++move_after_turn_cnt > MOVE_COUNT)
		{
			bDemoStep = BD_DONE;
			move_after_turn_cnt = 0;
		}
		Steer(0, 2);
		break;
	}
	case BD_DONE:
	{
		// 完成阶段
		Steer(0, 0);
		break;
	}
	case BD_REFINE:
	{
		float speed = cur_state == BD_FORWARD ? demoMoveSpeed : -demoMoveSpeed;
		demoCount += 1;
		if(refine_dir == 0)
		{
			Steer(2, speed);
			bDemoStep = cur_state;
		}
		else if(refine_dir == 1)
		{
			Steer(-2, speed);
			bDemoStep = cur_state;
		}
		break;
	}
	default:
		break;
	}
}


/*******************
***** 竞赛模式 *****
********************/
void BRaceMode(void)
{
	static enum BRaceStep bRaceStep = BR_INIT;

	// 四个红外传感器上次检测到黑线距今的时间差（间隔数）
	static int last_La = 1000;
	static int last_Ra = 1000;
	static int last_Lc = 1000;
	static int last_Rc = 1000;
	
	// 上面的间隔数的阈值
	static int etc = 5;

	// 避障中每个红外传感器检测到黑线时spCnt+=1，且该变量随时间衰减
	static int spCnt = 0;
	
	// spCnt到达该阈值时结束避障
	static int spThr = 20;
	
	// 结束时不立刻停下，再前进很小一段
	static int stopInertia = 50;

	// 模式切换的时间差
	static int traceTime = 0;
	static const int traceTimeThr = 50;
	static int avoidTime = 0;
	static const int avoidTimeThr = 100;
	
	// static int all_one = 0;

	switch (bRaceStep)
	{
	case BR_INIT:
	{
		// 初始化阶段
		bRaceStep = BR_TRACE;
		//Steer(0, 6);
		break;
	}
	case BR_TRACE:
	{	
		// 寻迹阶段
		float direct = 0;
		
		// 速度较慢，防止偏航
		float speed = 2;

		g_CarRunningMode = INFRARED_TRACE_MODE;
		traceTime++;
		
		// 更鲁棒？的判断条件
		if (Lc && !Rc)
		{
			direct = -10;
			speed = 3;
		}
		else if (Rc && !Lc)
		{
			direct = 10;
			speed = 3;
		}
		else if (La)
		{
			direct = -4;
			speed = 3;
		}
		else if (Ra)
		{
			direct = 4;
			speed = 3;
		}

		last_La = La == 1 ? 0 : last_La + 1;
		last_Ra = Ra == 1 ? 0 : last_Ra + 1;
		last_Lc = Lc == 1 ? 0 : last_Lc + 1;
		last_Rc = Rc == 1 ? 0 : last_Rc + 1;
		
		// 一定的时间差防止进入巡线立刻结束，若一段时间内四个传感器接连检测到黑线则进入避障
		if (traceTime > traceTimeThr && last_La < etc && last_Ra < etc && last_Lc < etc && last_Rc < etc)
		{
			bRaceStep = BR_AVOID;
			Steer(0,0);
			break;
		}
		Steer(direct, speed);

		break;
	}
	case BR_AVOID:
	{
		// 避障阶段
		g_CarRunningMode = ULTRA_AVOID_MODE;
		avoidTime++;

		UltraControl(1);
		
		// 曾经尝试的结束判断
		/*
		if((g_fCarAngle > -7) && (g_fCarAngle < 7))
		{
			if(La == 1 && Ra == 1 && Lc == 1 && Rc == 1){
				all_one += 1;
				if (avoidTime > 100 &&  all_one > 5)
					bRaceStep = BR_DONE;
			}
			else{
				all_one = 0;
			}
		}
		*/
		/*
		if((g_fCarAngle > -5) && (g_fCarAngle < 5))
		{
			last_La = La == 1 ? 0 : last_La + 1;
			last_Ra = Ra == 1 ? 0 : last_Ra + 1;
			last_Lc = Lc == 1 ? 0 : last_Lc + 1;
			last_Rc = Rc == 1 ? 0 : last_Rc + 1;
		}
		if (avoidTime > avoidTimeThr && last_La < etc && last_Ra < etc && last_Lc < etc && last_Rc < etc)
			bRaceStep = BR_DONE;
		*/

    //最终的结束判断
		// 一定的时间差防止进入避障立刻结束，一定的角度限制防止车仰头误判
		if(avoidTime > avoidTimeThr && g_fCarAngle > -1 && g_fCarAngle < 1) 
		{
				spCnt += (La << 1), spCnt += (Ra << 1), spCnt += (Lc << 1), spCnt += (Rc << 1);
		}
		if (spCnt > 0)
			spCnt -= 2;
		if (spCnt > spThr)
			bRaceStep = BR_DONE;
		
		// 一定时间没有避障自动结束（保底）
	  if (time_elapsed_after_turn > 1000)
		{
			bRaceStep = BR_DONE;
			stopInertia = 0;
		}
		break;
	}
	case BR_DONE:
	{
		// 完成阶段
		g_CarRunningMode = CONTROL_MODE;

    // 很短的减速再停下		
		if (stopInertia > 0)
			Steer(0, 4), stopInertia--;
		else
			Steer(0, 0);
		break;
	}
	default:
		break;
	}
}
