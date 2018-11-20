

#include "fft.h"
#include <math.h>
#include <stdlib.h>
#include "stdint.h"




int fft(complex *x, int N)
{
	int i,j,l,k,ip;
	static int M = 0;
	static int le,le2;
	static FFT_TYPE sR,sI,tR,tI,uR,uI;

	M = (int)(log(N) / log(2));

	/*
	 * bit reversal sorting
	 */
	l = N / 2;
	j = l;
	//BitReverse(x,x,N,M);
    for (i=1; i<=N-2; i++) {
        if (i < j) {
            tR = x[j].real;
			tI = x[j].img;
            x[j].real = x[i].real;
			x[j].img = x[i].img;
            x[i].real = tR;
			x[i].img = tI;
		}
		k = l;
		while (k <= j) {
            j = j - k;
			k = k / 2;
		}
		j = j + k;
	}

	/*
	 * For Loops
	 */
	for (l=1; l<=M; l++) {   /* loop for ceil{log2(N)} */
		le = (int)pow(2,l);
		le2 = (int)(le / 2);
		uR = 1;
		uI = 0;
		sR = cos(PI / le2);
		sI = -sin(PI / le2);
		for (j=1; j<=le2; j++) {   /* loop for each sub DFT */
			//jm1 = j - 1;
			for (i=j-1; i<=N-1; i+=le) {  /* loop for each butterfly */
				ip = i + le2;
				tR = x[ip].real * uR - x[ip].img * uI;
				tI = x[ip].real * uI + x[ip].img * uR;
				x[ip].real = x[i].real - tR;
				x[ip].img = x[i].img - tI;
				x[i].real += tR;
				x[i].img += tI;
			}  /* Next i */
			tR = uR;
			uR = tR * sR - uI * sI;
			uI = tR * sI + uI *sR;
		} /* Next j */
	} /* Next l */

	return 0;
}


complex Mul(complex a,complex b)//复数相乘
{
	complex c;
	c.real=a.real*b.real-a.img*b.img;
	c.img=a.real*b.img+a.img*b.real;
	return c;
}

complex Plus(complex a,complex b)//复数相加
{
	complex c;
	c.real=a.real+b.real;
	c.img=a.img+b.img;
	return c;
}

complex Sub(complex a,complex b)//复数相减
{
	complex c;
	c.real=a.real-b.real;
	c.img=a.img-b.img;
	return c;
}
/*
 *  Code below is an example of using FFT and IFFT.
 */
#define  SAMPLE_NODES              (256)
complex x[SAMPLE_NODES];
float INPUT[SAMPLE_NODES];
int OUTPUT[SAMPLE_NODES];
float sum_fft[256],ave_fft[11];

static void MakeInput()
{
	int i;
	for ( i=0;i<SAMPLE_NODES;i++ )
	{
		x[i].real = (sin(PI*8*i/SAMPLE_NODES)+sin(PI*2*i/SAMPLE_NODES));
		x[i].img = 0.0f;
		INPUT[i]=x[i].real;
	}
}
static void MakeOutput()
{
	int i;

	for ( i=0;i<SAMPLE_NODES;i++ )
	{
		OUTPUT[i] = sqrt(x[i].real*x[i].real + x[i].img*x[i].img)*1024;
	}
}


void Sum_fft(complex *x)
{
	int i;
	fft(x,256);
	for ( i=0;i<256;i++ )
	 {
		 sum_fft[i]+=sqrt(x[i].real*x[i].real + x[i].img*x[i].img)*1024;
	 }
}
void AVE_fft()
{
	int i;
	for ( i=0;i<11;i++ )
		 {
			 ave_fft[i]=sum_fft[i]/10;
		 }
}
void ex_fft(void)
{
	int j=0;

	for(j=0;j<10;j++)
	{	MakeInput();
		Sum_fft(x);
	}
	AVE_fft();
//	fft(x,256);
//	MakeOutput();
//	ifft(x,128);
//	MakeOutput();
}
