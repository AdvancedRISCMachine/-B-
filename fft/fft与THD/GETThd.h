#ifndef __GETTHD_H__
#define __GETTHD_H__

float sum_fft[256],ave_fft[11];
extern float GetTHDU(float *VH,float V1);
extern float GetTHDI(float *IH,float I1);
extern float GetHR_Uh(float VH,float V1);
extern float GetHR_Ih(float IH,float I1);


#endif
