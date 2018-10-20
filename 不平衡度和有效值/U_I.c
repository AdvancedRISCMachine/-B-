#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

#define N 1280

float GetUrms(float *Ui)
{
	float SumUrms=0;
	int i=0;
	for(;i<N;i++)
		SumUrms+=Ui[i]*Ui[i];
	return sqrtf(SumUrms/N);
}
float GetIrms(float *Ii)
{
	float SumIrms=0;
	int i=0;
	for(;i<10;i++)
		SumIrms+=Ii[i]*Ii[i];
	return sqrtf(SumIrms/10);
}
float GetImbalance(float *Uabc)
{
	float Labc,Imbalance;
	float Sum4=0,Sum2=0;
	int i=0;
	for(;i<3;i++)
	{
		Sum4+=powf(Uabc[i],4);
		Sum2+=powf(Uabc[i],2);
	}
	Sum2=Sum2*Sum2;
	Labc=Sum4/Sum2;
	Imbalance=sqrtf((1-sqrtf(3-6*Labc))/(1+sqrtf(3-6*Labc)));
	return Imbalance;
}
