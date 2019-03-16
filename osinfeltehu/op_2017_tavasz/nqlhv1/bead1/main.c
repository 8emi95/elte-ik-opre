#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#define OUTFILE "output.bin"

struct kerdes{
char szoveg[5][50];
time_t ido;
};


struct kerdes* kerdesek = 0;
int kerdes_db = 0;
int kerdes_max = 0;

void clear_line()
{
    char *buffer;
    size_t bufsize = 50;
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));

    characters = getline(&buffer,&bufsize,stdin);
	fflush(stdin);
}

void szambe(int* szam){
	char tmp[3];
	scanf("%s", &tmp);
    *szam = atoi(tmp);
	clear_line();
}

void get_line(char* out)
{
    char *buffer;
    size_t bufsize = 50;
    size_t characters;

    buffer = (char *)malloc(bufsize * sizeof(char));

    characters = getline(&buffer,&bufsize,stdin);
	stpcpy(out, buffer);
	fflush(stdin);
}

void beolvas(char* szoveg, char* bemenet)
{
	strcpy(bemenet,"\n");
	while(!strcmp(bemenet, "\n"))
	{
		printf("%s", szoveg);
		get_line(bemenet);
	}
}

void hozzaad_kerdes(struct kerdes* kerdes)
{
	if(kerdes_db == kerdes_max)
	{
		struct kerdes* tmp;
		tmp = (struct kerdes*)malloc(2*kerdes_max*sizeof(struct kerdes));
		int i;
		for(i = 0; i < kerdes_max;++i)
		{
			tmp[i] = kerdesek[i];
		}
		free(kerdesek);
		kerdesek = tmp;
		kerdes_max *= 2;
	}
	kerdesek[kerdes_db] = *kerdes;
	++kerdes_db;
}

void ujkerdes(){
	struct kerdes uj_kerdes;
	memset (&uj_kerdes, '\0', sizeof(struct kerdes));
	beolvas("Kerem a kerdest\n", (char*)&uj_kerdes.szoveg[0]);
	
	int db =0;
	while (db < 2 || db > 4){
	printf("Hany valaszt szeretne?\n");
	szambe(&db);
	}
	int i;
	for (i = 1; i<=db; ++i){
		beolvas("Kerem a valaszt\n", (char*)&uj_kerdes.szoveg[i]);
	}
	for (i = db + 1; i<=4; ++i){
		uj_kerdes.szoveg[i][0]='\0';
	}
	time(&uj_kerdes.ido);
	hozzaad_kerdes(&uj_kerdes);
}

void kiir(struct kerdes* kerdes){
	printf("%s", kerdes->szoveg[0]);
	int i = 1;
	while (strcmp(kerdes->szoveg[i],"")){
		printf("%d %s", i, kerdes->szoveg[i]);
		++i;
	}
	printf("letrehozas datuma : ");
	struct tm *tmp;
	tmp=gmtime(&kerdes->ido);
    printf("%d-%02d-%02d %02d:%02d:%02d\n",tmp->tm_year+1900,tmp->tm_mon,tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
}

void szerkesztes(){
	char tmp[1];
	int szam = 0;
	while(szam <=0 || szam > kerdes_db){
	printf("Hanyadik kerdest szeretne megvaltoztatni?\n");
	 szambe(&szam);
	 //szam = atoi(tmp);
	}
	--szam;
	kiir(&kerdesek[szam]);
	char c;
	printf("Megvaltoztatja a kerdest?(i/n)\n");
	scanf(" %c",&c);
	clear_line();
	if (c == 'i'){
		beolvas("Kerem az uj kerdest\n", (char*)&kerdesek[szam].szoveg[0]);
	}
	printf("Megvaltoztatja az elso valaszt?(i/n)\n");
	scanf(" %c",&c);
	clear_line();
	if (c == 'i'){
		beolvas("Kerem az uj valaszt\n", (char*)&kerdesek[szam].szoveg[1]);
	}
	printf("Megvaltoztatja a masodik valaszt?(i/n)\n");
	scanf(" %c",&c);
	clear_line();
	if (c == 'i'){
		beolvas("Kerem az uj valaszt\n", (char*)&kerdesek[szam].szoveg[2]);
	}
	if(!strcmp(kerdesek[szam].szoveg[3],"")){
		printf("Hozzaad harmadik valaszt?(i/n)\n");
		scanf(" %c",&c);
		clear_line();
		if (c == 'i'){
			beolvas("Kerem az uj valaszt\n", (char*)&kerdesek[szam].szoveg[3]);
			printf("Hozzaad negyedik valaszt?(i/n)\n");
			scanf(" %c",&c);
			clear_line();
			if (c == 'i'){
			beolvas("Kerem az uj valaszt\n", (char*)&kerdesek[szam].szoveg[4]);
			}
		}
	}
	else{
		printf("Megvaltoztatja a harmadik valaszt?(i/n)\n");
			scanf(" %c",&c);
			clear_line();
		if (c == 'i'){
			beolvas("Kerem az uj valaszt\n", (char*)&kerdesek[szam].szoveg[3]);
		}
		if(!strcmp(kerdesek[szam].szoveg[4],"")){
			printf("Hozzaad negyedik valaszt?(i/n)\n");
			scanf(" %c",&c);
			clear_line();
			if (c == 'i'){
				beolvas("Kerem az uj valaszt\n", (char*)&kerdesek[szam].szoveg[4]);
			}
		}
		else{	
			printf("Megvaltoztatja a negyedik valaszt?(i/n)\n");
			scanf(" %c",&c);
			clear_line();
			if (c == 'i'){
				beolvas("Kerem az uj valaszt\n", (char*)&kerdesek[szam].szoveg[4]);
			}
		}
	}

}
void torles(){
	int szam = 0;
	char c;
	while(szam <=0 || szam > kerdes_db){
	printf("Hanyadik kerdest szeretne torolni?\n");
	szambe(&szam);	
	}
	--szam;
	kiir(&kerdesek[szam]);
	int i = 0;
	while (strcmp(kerdesek[szam].szoveg[i+1],""))
	{i++;}
	if (i>2){
			printf("Csak valszt torol?(i/n)\n");
			scanf(" %c",&c);
			clear_line();
		if (c == 'i'){
			printf("Hanyadik valaszt torli?(i/n)\n");
			int szam2;
			szambe(&szam2);
			if(szam2>0 && szam2<=i){
				strcpy(kerdesek[szam].szoveg[szam2],kerdesek[szam].szoveg[i]);
				strcpy(kerdesek[szam].szoveg[i],"");
			}
			else{
			printf("Ilyen kerdes nincs\n");
			}
		}
		else{
			printf("Biztosan torli az egesz kerdest?(i/n)\n");
		scanf(" %c",&c);
		clear_line();
		if (c == 'i'){
			kerdesek[szam] = kerdesek[kerdes_db];
			memset (&kerdesek[kerdes_db], '\0', sizeof(struct kerdes));
			--kerdes_db;
		}
		}
		}
	else{
		printf("Biztosan torli?(i/n)\n");
		scanf(" %c",&c);
		clear_line();
		if (c == 'i'){
			kerdesek[szam] = kerdesek[kerdes_db];
			memset (&kerdesek[kerdes_db], '\0', sizeof(struct kerdes));
			--kerdes_db;
		}
	}
}


void listazas(){
	int i;
	for (i = 0; i<kerdes_db; ++i){
		printf("%d. ", i+1);
		kiir(&kerdesek[i]);
	}	
}

void veglegesit(){
	int i;
	char c;
	int file;
	file = open(OUTFILE,O_WRONLY|O_CREAT);
	for (i = 0; i<kerdes_db; ++i){
		printf("%d. ", i+1);
		kiir(&kerdesek[i]);
		printf("Veglegesiti a kerdest? (i/n)\n");
		scanf(" %c", &c);
		clear_line();
		if (c == 'i')
		{
			write(file,&kerdesek[i],sizeof(struct kerdes));
		}
	}
	close(file);
}


void menu(){
    int menu=0;
    while(menu!=6)
    {
        
        printf("1. Uj kerdes\n");
        printf("2. Kerdes szerkesztese\n");
        printf("3. Kerdes torlese\n");
        printf("4. Listazas\n");
        printf("5. Veglegesites\n");
        printf("6. Kilepes\n");
        szambe(&menu);
        switch (menu)
        {
            case 1:
                ujkerdes();
                break;
            case 2:
                szerkesztes();
                break;
            case 3:
                torles();
                break;
            case 4:
                listazas();
                break;
            case 5:
                veglegesit();
                break;
		}
    }
}

int main(){
	kerdes_max = 1;
	kerdesek=(struct kerdes*)malloc(kerdes_max*sizeof(struct kerdes));
	menu();
	return 0;
}