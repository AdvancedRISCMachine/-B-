#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_nvic.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
int D4_Counter=0;
int D3_Counter=0;
int flag=1;
extern void LED();
extern void LEDIntHandler();
int main(void) {
    	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |SYSCTL_XTAL_16MHZ);
	SysTickPeriodSet(32000000);
	SysTickEnable();
	SysTickIntRegister(&LEDIntHandler);
	LED();
   	 UC1701Init(60000);
    	UC1701Clear();
	while(1)
	{
	}
}