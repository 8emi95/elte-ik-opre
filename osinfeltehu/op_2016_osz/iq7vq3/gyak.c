#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int same(char * str1, char * str2){
	while(*str1 !=0 || *str2 !=0){
		if (*str1 != *str2){
			return 0;
		}else{
			str1++;
			str2++;
		}
	}
	return 1;
}

int length(char * str){
	int l=0;
	while(*str !=0){
		l++;
		str++;
	}
return l;
}
int countChar(char c,char * str){
	int db=0;
	while(*str !=0){
		if(*str==c)
			db++;
		str++;
	}
return db;
}

int countWord(char * w,char * str){
	int c = 0;
	char* p;
	char* w2;
	int x;
	while(*str !=0){
		if(*w==*str)
		{
			x=1;
			w2=w;
			p=str;
			while ((*w2 != 0 && *p != 0) && x==1)
			{
				if(*p==*w2)
				{
				p++;
				w2++;
				}else{
					x=0;
				}
			}
			if(x)
				c++;
		}
		str++;		
	}
	return c;
}

int main(){
char st1[] = "almáspaprika paprikás almáspörkölt almáspitével\0"; 
char st2[] = "TAZ0S\0";

printf("Hello\n");
if(same(st1,st2)){
	printf("Same\n");
}else{
	printf("Not same\n");
}
printf("%i\n",length(st1));
printf("%i\n",countChar('a',st1));
printf("%i\n",countWord("a\0",st1));
return 0;

}
