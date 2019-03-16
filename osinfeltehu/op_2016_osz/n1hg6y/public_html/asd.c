#include <stdio.h>

int IsSame(char[] a,char[] b)
{
	while(a!=0 && b!=0)
	{
		if(*a!=*b)return 0;
		a++;
		b++;
	}
	if (!(*a==0 && *b==0)) return 0;
	return 1;
}


int main()
{
	char str1[]="asdasd";
	char str2[]="hehheheeh";
	char str3[]="asdasd";
	
	printf(IsSame(str1,str2));
	printf("\n");
	printf(IsSame(str1,str3));

	return 0;
}
