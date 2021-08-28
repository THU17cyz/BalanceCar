/**********************************************************************
��Ȩ���У�	���ش��¿Ƽ���2017.
��		����	http://www.miaowlabs.com
��		����	https://miaowlabs.taobao.com/
�� �� ��: 	  display.c
��    ��:   ����ʵ����
��    д:   ��ҵ�� ���Դ ����
��		��:   3.00
�������:   2017.03.01
��		Ҫ: 	


***********************************************************************/

#include "stdio.h"
#include "oled.h"
#include "bmp.h"
#include "control.h"
#include "ultrasonic.h"
#include "mpu6050.h"
#include "common.h"
#include "manage.h"
#include "bsp.h"
#include "stm32f10x_gpio.h"
#include "infrare.h"


extern unsigned short BatVol;



/*
	��ʾlogo
*/
void ShowHomePageInit(void)
{
	OLED_DrawBMP(0,0,128,8,LOGO);  //ͼƬ��ʾ
	delay_ms(1000);
	OLED_Clear();
}


/*
	oled��ҳˢ�º���
	������ˢ�£�����һ��ˢ��ʱ�����
*/

void ShowHomePage(void)
{
	unsigned char buff[32]={0};
	static char step = 0;

	step++;
	if(step >= 6)step = 0;

	//�ֲ�ִ�У����̵���ˢ��ʱ��
	if(step == 0)
	{
		if (g_CarRunningMode == INFRARED_TRACE_MODE)
			snprintf((char*)buff, 21, "Infrare");
		else if (g_CarRunningMode == ULTRA_AVOID_MODE)
			snprintf((char*)buff, 21, "Ultra");
		else
			snprintf((char*)buff, 21, "Done");
		
		OLED_ShowString(0, 0, buff);
		
	}

	if(step == 1)
	{
		if(IsUltraOK())
			snprintf((char*)buff, 21,  "Distance:  %d(cm)       ", Distance);
		else
			snprintf((char*)buff, 21,  "Distance:  %s(cm)       ", "xx");
		
		OLED_ShowString(0, 1, buff);
	}

	if(step == 2)
	{
		if (g_CarRunningMode == INFRARED_TRACE_MODE)
			snprintf((char*)buff, 21, "La:  %d         ", La);
		else if (g_CarRunningMode == ULTRA_AVOID_MODE)
			snprintf((char*)buff, 21,  "EncoLeft:  %d         ",g_s16LeftMotorPulse);
		
		OLED_ShowString(0, 2, buff);
	}
	if(step == 3)
	{
		if (g_CarRunningMode == INFRARED_TRACE_MODE)
			snprintf((char*)buff, 21, "Ra:  %d         ", Ra);
		else if (g_CarRunningMode == ULTRA_AVOID_MODE)
			snprintf((char*)buff, 21, "diff: %d ", g_s32DiffCountAfterTurn);
		// snprintf((char*)buff, 21, "EncoRight: %d         ",g_s16RightMotorPulse);
	
		OLED_ShowString(0, 3, buff);
	}
	
	if(step == 4)
	{
		if (g_CarRunningMode == INFRARED_TRACE_MODE)
		  snprintf((char*)buff, 21, "Lc:  %d         ", Lc);
		else if (g_CarRunningMode == ULTRA_AVOID_MODE)
			snprintf((char*)buff, 21, "Angle:     %0.1f      ", g_fCarAngle);
		
		OLED_ShowString(0, 4, buff);
	}
	
	if(step == 5)
	{
		if (g_CarRunningMode == INFRARED_TRACE_MODE)
			snprintf((char*)buff, 21, "Rc:  %d         ", Rc);
		else if (g_CarRunningMode == ULTRA_AVOID_MODE)
			snprintf((char*)buff, 21, "Battery:   %0.1f(V)      ", g_BatVolt/100.0);
		
		OLED_ShowString(0, 5, buff);		
	}
}


