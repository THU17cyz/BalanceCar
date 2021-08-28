/**********************************************************************

��    ��:  ���� ���Դ ��ҵ��
	
***********************************************************************/


#ifndef __ALGORITHM_H
#define __ALGORITHM_H

#define B_MODE B_RACE_MODE	// �ڴ��޸�����ģʽ

enum BMode {
	B_TEST_MODE = 0,	// ����ģʽ
	B_DEMO_MODE = 1,	// ��ʾģʽ
	B_RACE_MODE = 2,	// ����ģʽ
};

enum BTestStep {
	BT_TEST_0 = 0,
	BT_TEST_1 = 1,
	BT_TEST_2 = 2,
};

enum BDemoStep {
	BD_INIT = 0,
	BD_FORWARD = 1,
	BD_FPAUSE = 2,
	BD_BACKWARD = 3,
	BD_BPAUSE = 4,
	BD_LEFTWARD = 5,
	BD_LPAUSE = 6, 
	BD_RIGHTWARD = 7,
	BD_DONE = 8,
	BD_REFINE = 9,
	BD_LFRONT = 10,
	BD_RFRONT = 11,
};

enum BRaceStep {
	BR_INIT = 0,
	BR_TRACE = 1,
	BR_AVOID = 2,
	BR_DONE = 3,
};

void BTestMode(void);
void BDemoMode(void);
void BRaceMode(void);

#endif
