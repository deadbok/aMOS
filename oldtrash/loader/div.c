#include <stdlib.h>
#include <stdio.h>

unsigned long	a, b, c;

void puti(long i)
{
	char	str[255];
	char	ch;
	int		j=0;
	int		end=0;

	do
	{
		str[end++]=i%10+'0';
		i/=10;
	}
	while(i);
	str[end]='\0';
	
	for(j=0; j<(end>>1); j++)
	{
		ch=str[j];
		str[j]=str[end-(j+1)];
		str[end-(j+1)]=ch;
	}
		
	printf("%s",str);
}


int main()
{
	a=2;
	b=10;
	
	c=b/a;
	c=b+a;
	printf("%d", c);

	c=b%a;
	printf("%d", c);

	if (b>a) 
		puti(b);

	return(0);
}

