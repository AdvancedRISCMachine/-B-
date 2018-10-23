#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/udma.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"
void
Timer0IntHandler(void)
{
	TimerIntClear(WTIMER0_BASE, TIMER_TIMA_TIMEOUT);
	if (UARTCharGet(UART0_BASE)=='A')
	{
		//UARTCharPut(UART0_BASE, UARTCharGet(UART0_BASE));
		UARTCharPut(UART0_BASE, 'H');
		UARTCharPut(UART0_BASE, 'E');
		UARTCharPut(UART0_BASE, 'L');
		UARTCharPut(UART0_BASE, 'L');
		UARTCharPut(UART0_BASE, 'O');
	}

}
void Timer2IntHandler(void)
{
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	if(UARTCharGet(UART0_BASE)=='s')
		{
			if(UARTCharGet(UART0_BASE)=='e')
			{
				if(UARTCharGet(UART0_BASE)=='n')
				{
					if(UARTCharGet(UART0_BASE)=='d')
					{
						UARTCharPut(UART0_BASE, 'H');
						UARTCharPut(UART0_BASE, 'E');
						UARTCharPut(UART0_BASE, 'L');
						UARTCharPut(UART0_BASE, 'L');
						UARTCharPut(UART0_BASE, 'O');
					}
				}
			}
		}
}
void Timer1IntHandler(void)
{
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//	UARTCharPut(UART0_BASE, 'H');
//	UARTCharPut(UART0_BASE, 'E');
//	UARTCharPut(UART0_BASE, 'L');
//	UARTCharPut(UART0_BASE, 'L');
//	UARTCharPut(UART0_BASE, 'O');
}
int main(void)
{
	//char cThisChar;
	uint32_t ui32Period;
	//�����豸ʱ��Ϊ 16MHz��ʱ��ԴΪ�ⲿ����
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
			SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	//��ʱ������
	TimerConfigure(WTIMER0_BASE, TIMER_CFG_PERIODIC);//���ڼ���
	ui32Period = (SysCtlClockGet()); //100ms
	TimerLoadSet(WTIMER0_BASE, TIMER_A, ui32Period );

	//�ж�����
	IntEnable(INT_WTIMER0A);  //Enables a timer interrupt
	TimerIntEnable(WTIMER0_BASE, TIMER_TIMA_TIMEOUT);   //Enables individual timer interrupt sources
	IntMasterEnable();  //Enables the processor interrupt
	TimerIntRegister(WTIMER0_BASE,TIMER_A,Timer0IntHandler);
	TimerEnable(WTIMER0_BASE, TIMER_A);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);//���ڼ���
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() -1);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerIntRegister(TIMER0_BASE,TIMER_A,Timer1IntHandler);
	TimerEnable(TIMER0_BASE, TIMER_A);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);//���ڼ���
	TimerLoadSet(TIMER1_BASE, TIMER_A,SysCtlClockGet() );
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerIntRegister(TIMER1_BASE,TIMER_A,Timer2IntHandler);
	TimerEnable(TIMER1_BASE, TIMER_A);
	//����ʹ��
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//GPIO ��������
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//���� UART ����
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));

	//���͡������ַ���ʾ�豸��ʼ���ɹ����ȴ������ַ�
	UARTCharPut(UART0_BASE, '!');
	//	do{
	//		cThisChar = UARTCharGet(UART0_BASE);
	//		//�����յ����ַ����ͣ��ж��Ƿ�Ϊ��\r���͡�\n�������������ѭ�������������һѭ��
	//		UARTCharPut(UART0_BASE, cThisChar);
	//	}
	//while((cThisChar != '\n') && (cThisChar != '\r'));
	while(1)
	{
	}
}


