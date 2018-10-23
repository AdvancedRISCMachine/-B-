#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/udma.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"
#include "uc1701.h"
extern void prifloat(unsigned char ucLine, unsigned char ucRow,float value, int n);
//*****************************************************************************
//Attention
//TM4C123 NMI unlock - To those who want to use PF0 and PD7, be reminded that these pins defaults as NMI ! ! !
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//*****************************************************************************
//
// Global variables
//
//
//
//*****************************************************************************
int a=0;
float d1,d2;
float f1,f;
char F[]={
		//"频",
		0x40,0x7C,0x40,0xFF,0x48,0x6C,0x4A,0xF2,
		0x12,0x1A,0xD6,0x12,0x12,0xFB,0x12,0x00,
		0x08,0x87,0x40,0x2F,0x10,0x0F,0x80,0x8F,
		0x40,0x20,0x1F,0x20,0x40,0xCF,0x00,0x00,

		//"率",
		0x00,0x14,0xA4,0x44,0x04,0x24,0xB5,0x6E,
		0x24,0x94,0x04,0x44,0xA6,0x14,0x00,0x00,
		0x08,0x09,0x08,0x08,0x09,0x09,0x09,0xFF,
		0x09,0x09,0x0B,0x08,0x08,0x0D,0x08,0x00
};
//*****************************************************************************
//
// The interrupt application.
// Timer interrupt
//
//
//*****************************************************************************
//void prifloat(unsigned char ucLine, unsigned char ucRow,float value, int n);
//显示
void screen1(void)
{
	UC1701Clear();
	UC1701ChineseDispaly(0, 0, 2,F);
	UC1701CharDispaly(0, 4, ":");
	//UC1701DisplayN(0,5,f);
	prifloat(0,5,f,2);
	UC1701CharDispaly(0, 12, "Hz");
}
void Timer0IntHandler(void)
{
	TimerIntClear(WTIMER0_BASE, TIMER_TIMA_TIMEOUT);
		//f=(a-1)/((d2-d1)/SysCtlClockGet());
	f1=(float)((d2-d1)/SysCtlClockGet());
	f=(a-1)/f1;
		screen1();
		a=0;
}
void Timer1IntHandler(void)
{
	TimerIntClear(WTIMER1_BASE, TIMER_CAPA_EVENT);
		if(a==0)
		{
			d1=TimerValueGet(WTIMER1_BASE,TIMER_A);
		}
		d2=TimerValueGet(WTIMER1_BASE,TIMER_A);
		a++;
}

//*****************************************************************************
//
// The main application.
// PF3---T1CCP1
// Breathe light
//
//*****************************************************************************
int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
    UC1701Init(60000);
    UC1701Clear();
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	//设置输入
	GPIOPinConfigure(GPIO_PC6_WT1CCP0);
	GPIOPinTypeTimer(GPIO_PORTC_BASE,GPIO_PIN_6);
	//边沿计时
	TimerDisable(WTIMER1_BASE,TIMER_A);
	TimerConfigure(WTIMER1_BASE, TIMER_CFG_A_CAP_TIME_UP|TIMER_CFG_SPLIT_PAIR );
	TimerControlEvent(WTIMER1_BASE,TIMER_A,TIMER_EVENT_POS_EDGE);
	TimerLoadSet(WTIMER1_BASE, TIMER_A,SysCtlClockGet()*10);
	IntEnable(INT_WTIMER1A);
	TimerIntEnable(WTIMER1_BASE, TIMER_CAPA_EVENT);
	IntMasterEnable();
	TimerIntRegister(WTIMER1_BASE,TIMER_A,Timer1IntHandler);
	TimerEnable(WTIMER1_BASE, TIMER_A);
	//
	//周期计时
	TimerDisable(WTIMER0_BASE,TIMER_A);
	TimerConfigure(WTIMER0_BASE, TIMER_CFG_A_PERIODIC|TIMER_CFG_SPLIT_PAIR );
	TimerLoadSet(WTIMER0_BASE,TIMER_A, SysCtlClockGet()*10);
	IntEnable(INT_WTIMER0A);
	TimerIntEnable(WTIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerIntRegister(WTIMER0_BASE,TIMER_A,Timer0IntHandler);
	TimerEnable(WTIMER0_BASE, TIMER_A);
	//
	while(1)
	{

	}
}

void prifloat(unsigned char ucLine, unsigned char ucRow,float value, int n)
{
	int i=0,temp,intfloValue,intValue;
	float floValue;
	intValue=(int)floorf(value);
	temp=intValue;
	for(;temp!=0;i++)
		temp=temp/10;

	floValue=value-intValue;
	intfloValue=(int)roundf(floValue*pow(10,n));//自然实现四舍五入
	UC1701DisplayN(ucLine,ucRow,intValue);
	UC1701CharDispaly(ucLine,ucRow+i,".");
	UC1701DisplayN(ucLine,ucRow+i+1,intfloValue);
}
