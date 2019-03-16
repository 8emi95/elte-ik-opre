//Írjuk ki, ha egy programnak legalább két paramétere van, és ezek mind meg egyeznek!

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc>=3){
		printf("legalabb 2 arg van es ezek ");
		int i = 2;
		while(i<argc){
			if(strcmp(argv[1], argv[i])!=0){
				printf("nem egyeznek meg\n");
				return 0;
			}
			//printf("%d \n",strcmp(argv[0], argv[i]));
			i++;
		}
		printf("ezek megegyeznek\n");
		return 0;
	}
	else{
		printf("kevesebb mint 2 arg van\n");
		return 0;
	}
}