

#include "fft.h"
#include <math.h>
#include <stdlib.h>
#include "stdint.h"


static void BitReverse(complex *x, complex *r, int n, int l)
{
	int i = 0;
	int j = 0;
	short stk = 0;
	static complex *temp = 0;
	temp = (complex *)malloc(sizeof(complex) * n);
	if (!temp)
	{
		return;
	}
	for(i=0; i<n; i++)
	{
		stk = 0;
		j = 0;
		do
		{
			stk |= (i>>(j++)) & 0x01;
			if(j<l)
			{
				stk <<= 1;
			}
		}
		while(j<l);
		if(stk < n)
		{
			temp[stk] = x[i];
		}
	}

	for (i=0; i<n; i++)
	{
		r[i] = temp[i];
	}
	free(temp);
}

int fft(complex *x, int N)
{
	int i,j,l,ip;
	static int M = 0;
	static int le,le2;
	static FFT_TYPE sR,sI,tR,tI,uR,uI;
	M = (int)(log(N) / log(2));

	BitReverse(x,x,N,M);

	for (l=1; l<=M; l++)
	{

		le = (int)pow(2,l);
		le2 = (int)(le / 2);
		uR = 1;
		uI = 0;
		sR = cos(PI / le2);
		sI = -sin(PI / le2);
		for (j=1; j<=le2; j++)
		{

			for (i=j-1; i<=N-1; i+=le)
			{

				ip = i + le2;
				tR = x[ip].real * uR - x[ip].img * uI;
				tI = x[ip].real * uI + x[ip].img * uR;
				x[ip].real = x[i].real - tR;
				x[ip].img = x[i].img - tI;
				x[i].real += tR;
				x[i].img += tI;
			}
			tR = uR;
			uR = tR * sR - uI * sI;
			uI = tR * sI + uI *sR;
		} /* Next j */
	} /* Next l */
	return 0;
}

int ifft(complex *x,int N)//暂不能使用

{

	int i;
	complex temp;
	int L,M=log2(N);
	int k,j,StepLength,Bank,r;
	double ps;
	complex W;
	BitReverse(x,x,N,M);
	for (L=1;L<=M;L++)

	{
		StepLength=pow(2,L);
		Bank=StepLength/2;
		for(j=0;j<=Bank-1;j++)
		{
			r=pow(2,M-L)*j;
			ps=2*PI/N*r;
			W.real=cos(ps);
			W.img=sin(ps);//改变旋转因子
			for(k=j;k<=N-1;k=k+StepLength)
			{
				complex x_temp;
				x_temp=Mul(W,x[k+Bank]);
				temp=Plus(x[k],x_temp);
				x[k+Bank]=Sub(x[k],x_temp);
				x[k].real=temp.real;
				x[k].img=temp.img;
			}
		}
	}
	for(k=0;k<N;k++)
	{
		x[k].real=x[k].real/N;
		x[k].img=x[k].img/N;
	}
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
#define  SAMPLE_NODES              (128)
complex x[SAMPLE_NODES];
int INPUT[SAMPLE_NODES];
int OUTPUT[SAMPLE_NODES];

static void MakeInput()
{
	int i;
	for ( i=0;i<SAMPLE_NODES;i++ )
	{
		x[i].real = sin(PI*2*i/SAMPLE_NODES);
		x[i].img = 0.0f;
		INPUT[i]=sin(PI*2*i/SAMPLE_NODES)*1024;
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

void ex_fft(void)
{
	MakeInput();

	fft(x,128);
	MakeOutput();
//	ifft(x,128);
//	MakeOutput();
}


