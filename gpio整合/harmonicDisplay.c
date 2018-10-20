#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "ui/ui.c"
#include "ui/font.h"
#include "ui/uc1701.c"
#include "ui/hw_uc1701.h"
#include "system/str_convert.c"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
void screen_harmonic(void)
{

	int a[10]={20,17,18,29,12,23,35,54,43,27};
	uiDisplayInit();
	uiDisplayClear();
	uiGRAMClear();
	uiGRAMClear();
	int i;
	for(i=0;i<10;i++)
	{
		uiDisplayDrawFrame(10*i,60-a[i],10*i+2,60);
	}
	uiDisplayerRefresh();

	SysCtlDelay(SysCtlClockGet()/5);//ÑÓÊ±200ms

}
