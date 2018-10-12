#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#define TARGET_IS_TM4C123_RB1

#ifndef M_PI
#define M_PI                    3.14159265358979323846
#endif

#define SERIES_LENGTH 512  //128*4
#define SAMPLE_LENGTH 128
#define TN 10

extern float GetUrms(float *Ui);
extern float GetIrms(float *Ii);
extern float GetImbalance(float *Uabc);

float gSeriesData[SERIES_LENGTH*TN],gADCSeriesData[SAMPLE_LENGTH*TN];

int32_t i32DataCount = 0,iADCCount=0;

int main(void)
{
	float fRadians,Uabc[3]={215,200,220};
	float Imbalance,Urms;

	ROM_FPULazyStackingEnable();
	ROM_FPUEnable();

	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	fRadians = ((2 * M_PI) / SERIES_LENGTH);

	while(i32DataCount < SERIES_LENGTH*TN)
	{
		gSeriesData[i32DataCount] = sinf(fRadians * i32DataCount);//128*4  10T
		i32DataCount++;
	}
	while(iADCCount < SAMPLE_LENGTH*TN)
	{
		gADCSeriesData[iADCCount] = gSeriesData[iADCCount*4];
		iADCCount++;
	}
	Imbalance=GetImbalance(Uabc);
	Urms=GetUrms(gADCSeriesData);
	while(1)
	{
	}
}
