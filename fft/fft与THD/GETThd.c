#include <stdint.h>
#include <stdbool.h>
#include <complex.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "GETThd.h"


#define N 10

float GetTHDU(float *VH,float V1)
{
	float thd_u,sum_vh;
	sum_vh=0;
	int i;
	for(i=2;i<N+1;i++)
	{
		sum_vh+=VH[i]*VH[i];
	}
	thd_u= sqrtf(sum_vh)/V1;
	return thd_u;
}
float GetTHDI(float *IH,float I1)
{
	float thd_i,sum_ih;
	sum_ih=0;
	int i;
	for(i=2;i<N+1;i++)
	{
		sum_ih+=IH[i]*IH[i];
	}
	thd_i= sqrtf(sum_ih)/I1;
	return thd_i;
}
float GetHR_Uh(float VH,float V1)
{
	float hr_uh;
	hr_uh= VH/V1;
	return hr_uh;
}
float GetHR_Ih(float IH,float I1)
{
	float hr_ih;
	hr_ih= IH/I1;
	return hr_ih;
}

