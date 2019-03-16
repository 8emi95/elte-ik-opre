#include <stdio.h>
#include <string.h>

int main (){
	
	FILE* file = fopen("test.txt","r+");
	
	char str[5];
	fpos_t pos;
	
	fgetpos (file,&pos);
	
	for(int i=0;i<3;++i){
		fgets(str,5,file);
	}
	
	fputs("\nmmmmmm\n",file);
	
	fclose(file);
  
	return 0;
}



	/*
			split(&Vendegek, sor);
			if( !strcmp(vnev, Vendegek.vnev) && !strcmp(knev, Vendegek.knev) ){
				printf("Vezeteknev: %s\n",Vendegek.vnev);
				printf("Keresztnev: %s\n",Vendegek.knev);
				printf("e-mail cim: %s\n",Vendegek.email);
				printf("Rendezveny azonosito: %s\n",Vendegek.rend);
				
				printf("--------------------------------\n");
				printf("Mit szeretne javitani?\n");
				printf("v - Vezeteknev\n");
				printf("k - Keresztnev\n");
				printf("e - e-mail cim\n");
				printf("r - Rendezveny azonosito\n");
				
				char ch[4];
				scanf("%s", ch);
				printf("--------------------------------\n");
			
				if( !strcmp(ch, "v") ){
					printf("Vezeteknev: ");
					scanf("%s", Vendegek.vnev);
				}
				else if( !strcmp(ch, "k") ){
					printf("Vendegek.knev: ");
					scanf("%s", Vendegek.knev);
				}
				else if( !strcmp(ch, "e") ){
					printf("Vendegek.email: ");
					scanf("%s", Vendegek.email);
				}
				else if( !strcmp(ch, "r") ){
					printf("Vendegek.rend: ");
					scanf("%s", Vendegek.rend);
				}
	*/