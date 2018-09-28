#include<stdlib.h>
extern int test1[100];
extern void initial()
{
int i;
for(i=0;i<100;i++)
{
test1[i]=rand()/100;
}
test1[i]=0;
}
