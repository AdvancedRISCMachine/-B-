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

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_nvic.h"
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
#include "driverlib/uart.h"
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
// The main application.
// UART0RX-PA0
// UART0TX-PA1
//
//*****************************************************************************

float u1_rms_2s=1.11,u2_rms_2s=2.22,u3_rms_2s=3.33,i1_rms_2s=4.44,i2_rms_2s=5.55,i3_rms_2s=6.66,
voltage_A_harmonic_2s=7.77,current_A_harmonic_2s=8.88,voltage_B_harmonic_2s=9.99,current_B_harmonic_2s=10.11,voltage_C_harmonic_2s=11.11,current_C_harmonic_2s=12.11,
active_power_2s=13.11,reactive_power_2s=14.11,apparent_power_2s=15.11,power_factor_2s=16.11,
frequency_10s=17.11,voltage_deviation_10s=18.11,voltage_unblance_10s=19,current_unblance_10s=20.11;

void UART0IntHandler(void)
{
	UARTIntClear(UART0_BASE,UART_INT_RX);
	while(UARTCharsAvail(UART0_BASE))
	{
		UARTCharPut(UART1_BASE,UARTCharGet(UART0_BASE) );
	}

}

void UART1IntHandler(void)
{
	UARTIntClear(UART1_BASE,UART_INT_RX);
	while(UARTCharsAvail(UART1_BASE))
	{
		UARTCharPut(UART0_BASE,UARTCharGet(UART1_BASE) );
	}
}

void ConfigureUART0(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTFIFOEnable(UART0_BASE);
	UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX1_8,UART_FIFO_RX1_8);
	IntEnable(INT_UART0);
	UARTIntEnable(UART0_BASE,UART_INT_RX);
//    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);
//    UARTStdioConfig(0,115200,SysCtlClockGet());
	UARTIntRegister(UART0_BASE,UART0IntHandler);
	IntMasterEnable();
}

void ConfigureUART1(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 38400,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTFIFOEnable(UART1_BASE);
	UARTFIFOLevelSet(UART1_BASE,UART_FIFO_TX1_8,UART_FIFO_RX1_8);
	IntEnable(INT_UART1);
	UARTIntEnable(UART1_BASE,UART_INT_RX);
    UARTClockSourceSet(UART1_BASE, UART_CLOCK_SYSTEM);
    UARTStdioConfig(1,38400,SysCtlClockGet());
	UARTIntRegister(UART1_BASE,UART1IntHandler);
	IntMasterEnable();
}

void UARTStringPut(uint32_t , unsigned char *,unsigned char);
void Bluetooth_Setup(void);
void Bluetooth_Tfloat(float ,int );
void PCshow(void);

int
main(void)
{
	//
	//设置系统时钟为50MHz
	//
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
			SYSCTL_XTAL_16MHZ);
	ConfigureUART0();
	ConfigureUART1();
//	Bluetooth_Setup();
	while(1)
	{
		PCshow();
		Bluetooth_Tfloat(3.1415926,3);
	    	SysCtlDelay((SysCtlClockGet())/3);
	}
}

void UARTStringPut(uint32_t ui32Base, unsigned char *p,unsigned char length)
{
	while(length--)
	{
		while(UARTBusy(ui32Base));
		UARTCharPut(ui32Base,*p++);
	}
}

void Bluetooth_Tfloat(float m,int n)
{
	int mz,mx,i;
	float temp;
	mz=(int)m;
	temp=m-mz;
	for(i=0;i<n;i++)
	{
		temp=temp*10;
	}
	mx=(int)temp;
	UARTprintf("%d",mz);
	UARTprintf(".");
	UARTprintf("%d",mx);
	UARTprintf("\r\n");
}
void PCshow(void)
{
	UARTStringPut(UART1_BASE,"u1_rms_2s:",10);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(u1_rms_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"u2_rms_2s:",10);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(u2_rms_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"u3_rms_2s:",10);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(u3_rms_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"i1_rms_2s:",10);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(i1_rms_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"i2_rms_2s:",10);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(i2_rms_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"i3_rms_2s:",10);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(i3_rms_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"voltage_A_harmonic_2s:",22);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(voltage_A_harmonic_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"voltage_B_harmonic_2s:",22);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(voltage_B_harmonic_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"voltage_C_harmonic_2s:",22);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(voltage_C_harmonic_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"current_A_harmonic_2s:",22);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(current_A_harmonic_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"current_B_harmonic_2s:",22);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(current_B_harmonic_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"current_C_harmonic_2s:",22);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(current_C_harmonic_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"active_power_2s:",16);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(active_power_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"reactive_power_2s:",18);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(reactive_power_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"apparent_power_2s:",18);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(apparent_power_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"power_factor_2s:",16);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(power_factor_2s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"frequency_10s:",14);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(frequency_10s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"voltage_deviation_10s:",22);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(voltage_deviation_10s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"voltage_unblance_10s:",21);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(voltage_unblance_10s,2);
	SysCtlDelay(1000000);

	UARTStringPut(UART1_BASE,"current_unblance_10s:",21);
	SysCtlDelay(1000000);
	Bluetooth_Tfloat(current_unblance_10s,2);
	SysCtlDelay(1000000);

}
void Bluetooth_Setup(void)
{
	UARTStringPut(UART1_BASE,"AT+ORGL\r\n",9);
	SysCtlDelay(1000000);
	UARTStringPut(UART1_BASE,"AT+ROLE=0\r\n",11);
	SysCtlDelay(1000000);
	UARTStringPut(UART1_BASE,"AT+NAME=XY\r\n",12);
	SysCtlDelay(1000000);
	UARTStringPut(UART1_BASE,"AT+PSWD=1234\r\n",14);
	SysCtlDelay(1000000);

}
