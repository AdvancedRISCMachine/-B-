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
#include "utils/uartstdio.h"

void Timer2IntHandler(void);

#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

int flag=1;
volatile int count,fre_count=0;
float fremesr,a;
uint32_t lastvalue,firstvalue=0,delta,u;
uint32_t ui32Period;
void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(WTIMER1_BASE, TIMER_TIMA_TIMEOUT);
	delta=lastvalue-firstvalue;
	a=(SysCtlClockGet()/(float)delta);
	u=fre_count-1;
	fremesr=(float)(a*u);
	fre_count=0;

	firstvalue=(TimerValueGet(WTIMER0_BASE, TIMER_A));

}

void
ConfigureUART(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTStdioConfig(0, 115200, SysCtlClockGet());
}


//*****************************************************************************
//
// The main application.
// counter
// led2-PA4
// PF4---T2CCP0 Timer 2 timer A
//*****************************************************************************
int
main(void)
{
	//	GPIO_PC6_WT1CCP0

	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//50M
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
	TimerConfigure(WTIMER1_BASE, TIMER_CFG_PERIODIC);

	ui32Period = (SysCtlClockGet()*10); //10s
	TimerLoadSet(WTIMER1_BASE, TIMER_A, ui32Period -1);

	IntEnable(INT_WTIMER1A);  //Enables a timer interrupt
	TimerIntEnable(WTIMER1_BASE, TIMER_TIMA_TIMEOUT);   //Enables individual timer interrupt sources
	IntMasterEnable();  //Enables the processor interrupt

	TimerIntRegister(WTIMER1_BASE, TIMER_A,Timer0IntHandler);
	TimerEnable(WTIMER1_BASE, TIMER_A);

	//
	// the general purpose timer and CCP pin using.
	// GPIO_PC4_WT0CCP0
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);
	GPIOPinConfigure(GPIO_PC4_WT0CCP0);

	TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR |TIMER_CFG_A_CAP_TIME_UP));
	TimerControlEvent(WTIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);

//	TimerLoadSet(WTIMER0_BASE, TIMER_A, 0xFFFF -1);
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
	////////////


	ConfigureUART();

	while(1)
	{

	}
}
void Timer2IntHandler(void)
{
	TimerIntClear(WTIMER0_BASE, TIMER_CAPA_EVENT);

	lastvalue=(TimerValueGet(WTIMER0_BASE, TIMER_A));

	fre_count+=1;
	if(flag)
	{
		flag=0;
		firstvalue=(TimerValueGet(WTIMER0_BASE, TIMER_A));
	}
//	UARTprintf("A%d\r\n B%d\r\n", lastvalue/1000000,flag*50);
}
