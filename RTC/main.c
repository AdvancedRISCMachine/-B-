//*****************************************************************************
//
// 
// 
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_comp.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/hibernate.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/udma.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"
#include "driverlib/comp.h"
#include "uc1701.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
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
// global variable
//
//*****************************************************************************
unsigned char flag;
int sec=0,min=0,k3=0,k4=0,rtc_flag=0,time_flag=0,start_flag=0,config_flag=1;
//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void screen(void)
{
	UC1701Clear();
	UC1701DisplayN(0,0,min);
		UC1701CharDispaly(0, 4, ":");
		//UC1701DisplayN(0,5,f);
		UC1701DisplayN(0,5,sec);
}

void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, SysCtlClockGet());
}

void rtc_init(void)
{
    uint32_t rtc_tick;
    
    struct tm rtc_time; 
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    
    HibernateEnableExpClk(SysCtlClockGet());
		
    HibernateClockConfig(HIBERNATE_OSC_LOWDRIVE);
    HibernateRTCEnable();
    
    // current time:
    rtc_time.tm_year    = 114;
    rtc_time.tm_mon     = 7;
    rtc_time.tm_mday    = 21;
    rtc_time.tm_hour    = 1;
    rtc_time.tm_min     = min;
    rtc_time.tm_sec     = sec;
    
    rtc_tick = umktime(&rtc_time);
    HibernateRTCSet(rtc_tick);
    
//    memset(&rtc_time, 0, sizeof(struct tm));
    
}

void rtc_get(struct tm *rtc_time)
{
    uint32_t rtc_tick;
    
    rtc_tick = HibernateRTCGet();
    ulocaltime(rtc_tick, rtc_time);
}

void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	if(config_flag)
	{
	if(GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_3)==0&&k3==0)
	{
		k3++;
	}
	if(GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_3)!=0&&k3==1)
	{
		k3=0;
		if(time_flag)
		{
			if(min<59)
			{
				min++;
			}
			else{min=0;}

		}
		else
		{
			if(sec<59)
			{
				sec++;
			}
			else{sec=0;}
		}
	screen();
	}

	if(GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_2)==0&&k4==0)
	{
		k4++;
	}
	if(GPIOPinRead(GPIO_PORTA_BASE,GPIO_PIN_2)!=0&&k4==1)
	{
		k4=0;
		if(time_flag==0)
		{
			time_flag++;
		}
		else
		{
			time_flag=0;
			start_flag=1;
			config_flag=0;
		}
	}
	}

}
void timer_init(void)
{
	uint32_t ui32Period;
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	ui32Period = (SysCtlClockGet()/1000);
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	IntEnable(INT_TIMER0A);  //Enables a timer interrupt
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);   //Enables individual timer interrupt sources
	IntMasterEnable();  //Enables the processor interrupt
	TimerIntRegister(TIMER0_BASE,TIMER_A,Timer0IntHandler);
	TimerEnable(TIMER0_BASE, TIMER_A);
}
void GPIO_init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
//	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);

	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,GPIO_PIN_3|GPIO_PIN_2);
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

}
//*****************************************************************************
//
// The main application.  
// HIB-RTC
// LED-PA4
//
//*****************************************************************************
int
main(void)
{
    struct tm rtc_now;
    
    int rtc_last_sec;
	
  	//
    // Set the system clock to run at 50MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    UC1701Init(60000);
    UC1701Clear();

//    rtc_init();
   GPIO_init();
			timer_init();
    //
    // Initialize the UART as a console for text I/O.
    //
    ConfigureUART();    
	screen();
    while(1)
    {
    	if(start_flag)
    	{
    		rtc_init();
    		start_flag=0;

    		//					  UARTprintf("%d-%d-%d", rtc_now.tm_year + 1900, rtc_now.tm_mon, rtc_now.tm_mday);
    		//			      UARTprintf("%02d:%02d:%02d\n", rtc_now.tm_hour, rtc_now.tm_min, rtc_now.tm_sec);
    		do
    		{
    			rtc_get(&rtc_now);
    			if (rtc_last_sec != rtc_now.tm_sec)
    			{
    				rtc_last_sec = rtc_now.tm_sec;
    				UC1701Clear();
    				UC1701DisplayN(0,0,rtc_now.tm_min);
    				UC1701CharDispaly(0, 4, ":");
    				UC1701DisplayN(0,5,rtc_now.tm_sec);
    			}
    		}while(config_flag==0);
    	}
    }
}

