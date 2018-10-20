#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stdint.h>
#include <stdbool.h>

extern float active_power_2s , reactive_power_2s , apparent_power_2s , power_factor_2s ;
extern char XDY[96];
extern char XDL[96];
extern char BPHD[128];
extern char DYPC[128];
extern char PL[64];
extern float Uphase,Iphase;
extern float fre;
extern void screen21();
extern void screen22();
extern void screen23();
extern void screen11(void);
extern void screen12(void);
extern void screen13(void);
extern void screen14(void);
extern void test(void);
extern void prifloat(unsigned char ucLine, unsigned char ucRow,float value, int n);
extern float Round(float value, int n);
extern void screen_harmonic(void);




#endif
