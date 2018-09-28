extern int pro95(int *a,int num)
{
	int i=0,j=0,b,max,num95;
	num95=num*0.05;
	for(i=0;i<num-1;i++)
	{
		for(j=num-1;j>i;j--)
		{
			if(a[j]>a[j-1])
			{
				b=a[j-1];
				a[j-1]=a[j];
				a[j]=b;
			}
		}
	}
	max=0;
	for(i=num95;i<num;i++)
	{
		if(a[i]>max)
			max=a[i];
	}
	return max;
}
