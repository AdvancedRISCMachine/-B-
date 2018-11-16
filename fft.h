/*
 * fft.h
 *
 *  Created on: 2018Äê10ÔÂ7ÈÕ
 *      Author: lenovo
 */

#ifndef FFT_H_
#define FFT_H_
typedef float          FFT_TYPE;
#ifndef PI
#define PI             (3.14159265f)
#endif
typedef struct complex_st
{
	FFT_TYPE real;
	FFT_TYPE img;
} complex;
complex Mul(complex a,complex b);
complex Plus(complex a,complex b);
complex Sub(complex a,complex b);
int fft(complex *x, int N);
int ifft(complex *x, int N);
void zx_fft(void);
#endif/* FFT_H_ */
