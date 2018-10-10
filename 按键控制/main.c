#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/ssi.h"
#include "uc1701.h"
#include "inc/hw_types.h"
#include "inc/hw_nvic.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"


extern void screen11();
extern void screen12();
extern void screen13();
extern void screen21();
extern void screen22();
extern void screen23();
extern void screen31();
extern void screen32();
extern void screen33();
extern void key();

int i=1;

void Key1IntHandler()
{
	uint32_t IntStatus_raw;
	IntStatus_raw = GPIOIntStatus(GPIO_PORTA_BASE, 0); //读GPIORIS寄存器
	if(i>=2)
	{
		if(IntStatus_raw & GPIO_INT_PIN_3)  // PA3引脚有中断事件
		{
		i--;
		key();
			GPIOIntClear(GPIO_PORTA_BASE,GPIO_INT_PIN_3);//清除中断
		}
	}
	if(i<=8)
	{
		if(IntStatus_raw & GPIO_INT_PIN_2)  // PA2引脚有中断事件
		{
			i++;
			key();
			GPIOIntClear(GPIO_PORTA_BASE,GPIO_INT_PIN_2);//清除中断
		}
	}
}
void Key2IntHandler()
{
	if(i>=4)
	{
		i=i-3;
		key();
		GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_7);
	}
}
void Key3IntHandler()
{
	if(i<=6)
	{
		i=i+3;
		key();
		GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
	}
}
int
main(void)
{
	/*HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//解锁

	        HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;//确认

	        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;//重新锁定*/


    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;//解锁

	        HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;//确认

	        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;//重新锁定
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,GPIO_PIN_3|GPIO_PIN_2);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOIntRegister(GPIO_PORTA_BASE, Key1IntHandler);
    GPIOIntRegister(GPIO_PORTD_BASE, Key2IntHandler);
    GPIOIntRegister(GPIO_PORTF_BASE, Key3IntHandler);
	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_3,GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_3);
	GPIOIntTypeSet(GPIO_PORTA_BASE,GPIO_PIN_2,GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_2);
	GPIOIntTypeSet(GPIO_PORTD_BASE,GPIO_PIN_7,GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTD_BASE,GPIO_PIN_7);
	GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_4,GPIO_FALLING_EDGE);
	GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_4);

    UC1701Init(60000);
    UC1701Clear();
    screen11();
    SysCtlDelay(1000000);

    while(1)
    {

    }
}
void screen11()
{

    UC1701Clear();
    UC1701CharDispaly(0, 1, "NO.1");

}
void screen12()
{

    UC1701Clear();
    UC1701CharDispaly(0, 1, "NO.2?");

}
void screen13()
{

    UC1701Clear();
    UC1701CharDispaly(0, 1, "NO.3?");
}
void screen21()
{

    UC1701Clear();
    UC1701CharDispaly(0, 1, "NO.4?");
}
void screen22()
{

    UC1701Clear();
    UC1701CharDispaly(0, 1, "NO.5?");
}
void screen23()
{

    UC1701Clear();
    UC1701CharDispaly(0, 1, "NO.6?");
}
void screen31()
{

    UC1701Clear();
    UC1701CharDispaly(0, 1, "NO.7?");
}
void screen32()
{

    UC1701Clear();
    UC1701CharDispaly(0, 1, "NO.8?");
}
void screen33()
{

    UC1701Clear();
    UC1701CharDispaly(0, 1, "NO.9?");
}
void key()
{
	if(i==1) {screen11(); }
	if(i==2) {screen12(); }
	if(i==3) {screen13(); }
	if(i==4) {screen21(); }
	if(i==5) {screen22(); }
	if(i==6) {screen23(); }
	if(i==7) {screen31(); }
	if(i==8) {screen32(); }
	if(i==9) {screen33(); }

}
