//Definiáljunk két stringet, írjuk ki hogy megegyeznek-e!
//nem 0 ha egyenlõek, nulla ha nem egyenlõek

#include <stdio.h>
#include <string.h>

int equal(char *s1, char *s2){
	int id=0;
	while(&s1[id] != NULL && &s2[id] != NULL){
		if(&s1[id]!=&s2[id]){
			return 0;
		}
		id++;
	}
	if(strlen(s1)!=strlen(s2)){
		return 0;
	}
	return 1;
}

int main(){
	char* str="alma";
	char* str2="alma";
	char* str3="korte";
	printf("%d\n", equal(str,str2));
	printf("%d\n", equal(str2,str3));
	printf("%d\n", equal("alma","alma"));
	printf("%d\n", equal("gyumolcs","zoldseg"));
}