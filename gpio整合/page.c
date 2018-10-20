
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/ssi.h"
#include "ui/uc1701.h"
#include "inc/hw_types.h"
#include "inc/hw_nvic.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"
#include "page.h"
#include "display.h"
int page=1;

void Key1IntHandler()
{
	uint32_t IntStatus_raw;
	IntStatus_raw = GPIOIntStatus(GPIO_PORTA_BASE, 0); //读GPIORIS寄存器
	if(page>=2)
	{
		if(IntStatus_raw & GPIO_INT_PIN_3)  // PA3引脚有中断事件
		{
			page--;
			key();
			GPIOIntClear(GPIO_PORTA_BASE,GPIO_INT_PIN_3);//清除中断
		}
	}
	if(page<=7)
	{
		if(IntStatus_raw & GPIO_INT_PIN_2)  // PA2引脚有中断事件
		{
			page++;
			key();
			GPIOIntClear(GPIO_PORTA_BASE,GPIO_INT_PIN_2);//清除中断
		}
	}
}
void Key2IntHandler()
{
	if((page>=5)&&(page<8))
	{
		page=1;
	}
	else if (page==8)
	{
		page=5;
	}
	else
	{
		page=1;
	}
	key();
	GPIOIntClear(GPIO_PORTD_BASE,GPIO_INT_PIN_7);
}
void Key3IntHandler()
{
	if(page<=4)
	{
		page=5;
	}
	else if((page>4)&&(page<8))
	{
		page=8;
	}
	key();
	GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_4);
}
void KeyConfigure()
{
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
}
void key()
{
	if(page==1) {screen11(); }
	if(page==2) {screen12(); }
	if(page==3) {screen13(); }
	if(page==4) {screen14(); }
	if(page==5) {screen21(); }
	if(page==6) {screen22(); }
	if(page==7) {screen23(); }
	if(page==8) {screen_harmonic(); }


}
