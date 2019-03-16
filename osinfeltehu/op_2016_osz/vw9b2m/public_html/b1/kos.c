typedef int bool;
#define true 1
#define false 0

#include <stdio.h>
#include <stdlib.h>



int main(int argc,char ** argv) 
{
 bool notDone = true;
 char str1[20];
 char strExit[] = "kilep";
/* while(notDone){
		printf("ide meg eljutok.\n");
		scanf("%s", str1);
		if(strcmp(str1,strExit) != false){
			printf("nem lepett ki az illeto \n");
			printf("amit megjegyeztem : %s \n", str1);

		}
		else{
			notDone = false;
			printf("kilepett az illeto \n");
		}
	}
*/

	while(notDone){
		printf(" Valassza ki mit szeretne csinalni.:\n 1 : Rendezvenyek megnyitasa\n 2 : Uj Rendezveny\n 3 : Rendezveny torlese\n 4 : Uj vendeg felvetel egy esemenyre\n 5 : Kilepes\n");
		scanf("%s", str1);
		int num = atoi(str1);
		switch(num){
			case 1: printf("congrats, megnyitottad a rendezvenyeket.\n"); break;
			case 2: printf("congrats, uj rendezvenyt akarsz.\n"); break;
			case 3: printf("congrats, rendezvenyt akarsz torolni\n"); break;
			case 4: printf("congrats, megnyitottad a uj felhasznalo felvetelet.\n"); break;
			case 5: printf("congrats, kileptel.\n"); notDone = false; break;
			
		}
	}
	return 0;
}