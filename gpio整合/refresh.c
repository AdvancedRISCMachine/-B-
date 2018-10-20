#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "refresh.h"
#include "display.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_nvic.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "ui/uc1701.h"
int D4_Counter=0;
int D3_Counter=0;
int flag=1;
void LED()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_4);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE,GPIO_PIN_6);
}
void LEDIntHandler()
{
	/*if(screen11==1)screen11();
	if(screen12==1)screen12();
	if(screen13==1)screen13();
	if(screen14==1)screen14();
	if(screen21==1)screen21();
	if(screen22==1)screen22();
	if(screen23==1)screen23();
	if(screen_harmonic==1)screen_harmonic();
*/
	if(flag==1)
	{
		if(D4_Counter)
		{
			D4_Counter--;
			GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6,1<<6);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6,0<<6);
			D4_Counter=2;
		}
		if(D3_Counter)
		{
			D3_Counter--;
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0<<4);
			D3_Counter=12;
		}
	}
	if(flag==0)
	{
		GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_6,1<<6);
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,1<<4);
	}
}
LEDFlash(void)
{
	SysTickPeriodSet(32000000);
	SysTickEnable();
	SysTickIntRegister(&LEDIntHandler);
	LED();
//暂时未和数据同步标识
}
