/*
 * main.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
int test1[100];
extern void initial();
extern int pro95(int *a,int num);
int main()
{
	int max,num=0,i;
	initial();//���ò�������
	for(i=0;test1[i]!=0;i++)
	{
		num++;
	}
	max=pro95(test1,num);//����95%���ʴ�ֵ��test1Ϊ��������numΪ����Ԫ�ظ���
	while(1){};
	//return 0;
}
