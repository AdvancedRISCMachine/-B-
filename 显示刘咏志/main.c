#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"

int key,u;
int PA3;
int PF0;
void KeyIntHandler();
int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);



	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3);
	
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//解锁

	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;//确认
	
 	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;//重新锁定
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,1<<0);
	
	GPIOIntRegister(GPIO_PORTA_BASE, KeyIntHandler);

	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_2,GPIO_FALLING_EDGE); //PA2
	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_FALLING_EDGE); //PA3
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_2);//指定GPIO中断源
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_3);
	while(1)
	{
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
//		SysCtlDelay(2000000);
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
//		SysCtlDelay(2000000);
//		if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
	}
}
void KeyIntHandler()
{
	key=HWREG(GPIO_PORTA_BASE + GPIO_O_MIS);
	if(key==8||key==12) PF0=0;
	else if(key==4) PF0=1;
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_0,PF0);
	GPIOIntClear(GPIO_PORTA_BASE,GPIO_INT_PIN_2|GPIO_INT_PIN_3);
}
