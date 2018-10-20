#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
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
#include "LCDlyz.h"
#include "math.h"

extern float Uphase,Iphase;
extern float fre;

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif
uint32_t lastvalue=0,nowvalue;
float duty,fremesr;
unsigned char t;

void Timer2IntHandler(void);
void Timer1IntHandler(void)
{
	//	EXSSENTIAL !!!!!!!!!!!!!
	//	OTHERWISE IntDefaultHandler(void)
	//	Go into an infinite loop.!!!!!!!!!!!
	TimerIntClear(TIMER1_BASE, TIMER_CAPB_EVENT);
	//	fre_count=(TimerValueGet(TIMER2_BASE, TIMER_A));
}
int
main(void)
{
	uint32_t ui32Period;
	duty=0.5;
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

//	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
	GPIOPinConfigure(GPIO_PC4_WT0CCP0);//GPIO_PC4_WT0CCP0


	GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);

	TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR |TIMER_CFG_A_CAP_TIME_UP));
	TimerControlEvent(WTIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
	//捕获模式，A定时器，上升沿捕获
	IntEnable(INT_WTIMER0A);
	//使能TIMER2A
	TimerIntEnable(WTIMER0_BASE, TIMER_CAPA_EVENT);
	//定时器A捕获事件触发中断
	IntMasterEnable();
	//master interrupt enable API for all interrupts
	TimerEnable(WTIMER0_BASE, TIMER_A);
	//TIMER2A开始计数，当计数值等于TimerLoadSet，触发中断
	TimerIntRegister(WTIMER0_BASE, TIMER_A,Timer2IntHandler);
	///////////////////////////////////////////////////////////////////////////
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinConfigure(GPIO_PF3_T1CCP1);

	GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_3);

	TimerConfigure(TIMER1_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM);
	ui32Period=SysCtlClockGet() /50000;
	TimerLoadSet(TIMER1_BASE, TIMER_B,ui32Period-1);
	TimerMatchSet(TIMER1_BASE, TIMER_B,(int)(duty*ui32Period)-1);

	IntEnable(INT_TIMER1B);  //Enables a timer interrupt
	TimerIntEnable(TIMER1_BASE, TIMER_CAPB_EVENT);   //Enables individual timer interrupt sources
	IntMasterEnable();  //Enables the processor interrupt
	TimerIntRegister(TIMER1_BASE,TIMER_B,Timer1IntHandler);
	TimerEnable(TIMER1_BASE, TIMER_B);

	UC1701Init(60000);
	UC1701Clear();
	fre=fremesr;
	screen22();
	//	SysCtlDelay( SysCtlClockGet()/1000);
	while(1)
	{
		if((int)(fremesr*1000) != (int)(fre*1000))
		{
			fre=fremesr;
			screen22();
		}
	}
}

void Timer2IntHandler(void)
{
	TimerIntClear(WTIMER0_BASE, TIMER_CAPA_EVENT);
	nowvalue=(TimerValueGet(WTIMER0_BASE, TIMER_A));
	fremesr=(float)( SysCtlClockGet()/(nowvalue-lastvalue) );//Hz

	lastvalue=(TimerValueGet(WTIMER0_BASE, TIMER_A));
}
