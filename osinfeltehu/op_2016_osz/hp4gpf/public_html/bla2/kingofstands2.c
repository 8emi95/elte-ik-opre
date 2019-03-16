#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

//A második beadandó feladait a MasodikBead függvény végzi!
void readSignal(int);
int readyToSend = 0;

struct Jelentkezes {
	char nev[50];
	char mail[50];
	int id; //Rendezvény IDje
	int index; //Vendég Indexe 
	time_t ido;
};

void UjJelentkezes(int maxindex, int rendezvenyek[], int rendn){
	int noofstructs = NoOfStructs();
	FILE* f = fopen("jelentkezesek.data", "ab+");
	struct Jelentkezes j;
	printf("Név: ");
	fgets(j.nev, 50, stdin); //Enélkül üresen hagyja a nevet
	fgets(j.nev, 50, stdin);
	printf("E-mail: ");
	fgets(j.mail, 50, stdin);
	printf("Rendezvény azonosítója: ");
	int rid;
	scanf("%d", &rid);
	while(RendezvenyEllenorzes(rendezvenyek, rendn, rid) == 0){
		printf("Ez a rendezvény-azonosító nem létezik!\nRendezvény azonosítója: ");
		scanf("%d", &rid);
	}
	j.id = rid;
	j.index=maxindex+1;
	j.ido=time(0);
	fwrite(&j, sizeof(struct Jelentkezes), 1, f);
	printf("Jelentkezés sikeres! Te vagy a %d. vendég!\n", j.index);
	fclose(f);
}

void UjRendezveny(int rendezvenyek[], int rendn){//
	printf("Új Rendezvény azonosítója: ");
	int rid;
	scanf("%d", &rid);
	while(RendezvenyEllenorzes(rendezvenyek, rendn, rid) == 1){
		printf("Ez a rendezvény-azonosító már létezik!\nRendezvény azonosítója: ");
		scanf("%d", &rid);
	}
	int i;
	FILE * f = fopen("rendezvenyek.data", "w");
	for(i=0;i<rendn; i++){
		fwrite(&rendezvenyek[i], sizeof(int), 1,f);
	}	
	fwrite(&rid, sizeof(int), 1,f);
	fclose(f);
}

int RendezvenyEllenorzes(int rendezvenyek[], int n, int rendezveny){
	int i;
	for(i=0; i<n; i++){
		if(rendezvenyek[i] == rendezveny){
			return 1;
		}
	}
	return 0;
}

void RendezvenyListaz(int rendezvenyek[], int rendn){
	printf("Rendezvényazonosítók listája: ");
	int i;
	for(i=0; i<rendn; i++){
		printf("%d ",rendezvenyek[i]);
	}
	printf("\n");
}

void RendezvenyTorles(struct Jelentkezes osszes[], int n, int rendezvenyek[], int rendn){
	printf("Törlendő rendezvény azonosítója: ");
	int rid;
	scanf("%d", &rid);
	while(RendezvenyEllenorzes(rendezvenyek, rendn, rid) != 1){
		printf("Ez a rendezvény-azonosító nem létezik!\nRendezvény azonosítója: ");
		scanf("%d", &rid);
	}
	printf("A rendezvény törlésével az összes hozzátartozó jelentkezés is törlődik! Folytatja? I/N: ");
	getchar();
	char c=getchar();
	c=toupper(c);
	if(c == 'I'){
		int i;
		FILE * fr = fopen("rendezvenyek.data", "w");
		for(i=0;i<rendn; i++){
			if(rendezvenyek[i] != rid){
			fwrite(&rendezvenyek[i], sizeof(int), 1,fr);
			}
		}
		fclose(fr);
		
		FILE* f = fopen("jelentkezesek.data", "w");
		for(i=0; i<n; i++){
			if(osszes[i].id != rid){
			fwrite(&osszes[i], sizeof(struct Jelentkezes), 1, f);
			}
		}
		fclose(f);
	}
}

void JelListaz(struct Jelentkezes osszes[], int n){
	int i;
	for(i=0; i<n; i++){
		char* c_time = ctime(&osszes[i].ido);
		printf("%d %s %s %d %s", osszes[i].index, osszes[i].nev, osszes[i].mail, osszes[i].id, c_time);
	}
}

int VendegEllenorzes(struct Jelentkezes osszes[], int n, int vendeg){
	int i;
	for(i=0; i<n; i++){
		if(osszes[i].index == vendeg){
			return i;
		}
	}
	return -1;
}

int NoOfStructs(){ //Mivel a structoknak ugyanannyi helyet foglalunk, a fájlméretből megkaphatjuk a rekordok számát
	struct stat st;
	stat("jelentkezesek.data", &st);
	int filesize = st.st_size;
	int structsize = sizeof(struct Jelentkezes);
	if(filesize%structsize != 0){
		printf("HIBA: Fájlméret nem osztható a struct méretével!");
		printf("File size: %d\n", filesize);
		printf("Struct size: %d\n", structsize);
		exit(1);
	}
	return filesize/structsize;
}

int NoOfInts(){
	struct stat st;
	stat("rendezvenyek.data", &st);
	int filesize = st.st_size;
	int intsize = sizeof(int);
	if(filesize%intsize != 0){
		printf("HIBA: Fájlméret nem osztható a struct méretével!");
		printf("File size: %d\n", filesize);
		printf("Int size: %d\n", intsize);
		exit(1);
	}
	return filesize/intsize;
}

void JelentkezesModosit(struct Jelentkezes osszes[], int n, int rendezvenyek[], int rendn){
	printf("Jelentkezés ID-je: ");
	int id;
	scanf("%d", &id);
	int index = VendegEllenorzes(osszes, n, id);
	while(index == -1){
		printf("Vendég ID nem létezik! Próbálja újra: ");
		scanf("%d", &id);
		index = VendegEllenorzes(osszes, n, id);
	}
	
	char* c_time = ctime(&osszes[index].ido);
		printf("%d %s %s %d %s", osszes[index].index, osszes[index].nev, osszes[index].mail, osszes[index].id, c_time);
	
	printf("Mit szeretnél módosítani? Név[N] E-mail[E] RendezvényID[R] Mégsem[X]");
	getchar();
	char c=getchar();
	c=toupper(c);
	char s[50];
		switch(c){
			case 'N':
				printf("Új név: ");
				fgets(s, 50, stdin);				
				fgets(s, 50, stdin);
				strcpy(osszes[index].nev, s);
				break;
			case 'E':
				printf("Új e-mail: ");
				fgets(s, 50, stdin);
				fgets(s, 50, stdin);
				strcpy(osszes[index].mail, s);
				break;
			case 'R':
				printf("Új rendezvény-azonosító: ");
				int rid;
				scanf("%d", &rid);
				while(RendezvenyEllenorzes(rendezvenyek, rendn, rid) == 0){
					printf("Ez a rendezvény-azonosító nem létezik!\nRendezvény azonosítója: ");
					scanf("%d", &rid);
				}
				osszes[index].id = rid;
				break;	
			default:
				break;
		}
	FILE* f = fopen("jelentkezesek.data", "w");
	int i;
	for(i=0; i<n; i++){
		fwrite(&osszes[i], sizeof(struct Jelentkezes), 1, f);
	}
	fclose(f);
}

void JelentkezesTorles(struct Jelentkezes osszes[], int n){
	printf("Jelentkezés ID-je: ");
	int id;
	scanf("%d", &id);
	int index = VendegEllenorzes(osszes, n, id);
	while(index == -1){
		printf("Vendég ID nem létezik! Próbálja újra: ");
		scanf("%d", &id);
		index = VendegEllenorzes(osszes, n, id);
	}
	
	char* c_time = ctime(&osszes[index].ido);
		printf("%d %s %s %d %s", osszes[index].index, osszes[index].nev, osszes[index].mail, osszes[index].id, c_time);
	
	printf("Biztosan törölni szeretnéd? I/N\n");
	getchar();
	char c=getchar();
	c=toupper(c);
	if(c == 'I'){
		FILE* f = fopen("jelentkezesek.data", "w");
		int i;
		for(i=0; i<n; i++){
			if(i != index){
			fwrite(&osszes[i], sizeof(struct Jelentkezes), 1, f);
			}
		}
		fclose(f);
	}
}

void child_function(){
	int fe;
	char buf[50];
	pid_t parent = getppid();
	char * myfifo = "myfifo";
	fe= open(myfifo, O_RDONLY);
	read(fe, buf, 50);
	//close(fe);
	printf("Utazás: %s", buf);
	sleep(2);
	int eredmeny;
	printf("Várom a résztvevőket!\n");
	kill(getppid(),SIGUSR1);

	//fe= open(myfifo, O_RDONLY);
	read(fe, &eredmeny, sizeof(eredmeny));
	printf("%d jelentkezett résztvevő megkapva. Listázás:\n", eredmeny);
	int i;
	char resztvevk[eredmeny][50]; 
	read(fe, &resztvevk, sizeof(resztvevk));	
	close(fe);
	for(i=0; i<eredmeny; i++){
		printf("%s", resztvevk[i]);
	}
	printf("Rendezvény lebonyolítása...\n");
	sleep(2);
	time_t t;
	srand((unsigned) time(&t));
	int siker = rand() % 101;
	char jelentes[300];
	sprintf(jelentes, "A rendezvény %d százalénak tetszett. Nem jöttek el: \n", siker);
	int voltHianyzo = 0;
	for(i=0; i<eredmeny; i++){
		int n = rand() % 10;
		if(n == 0){
			char * cat = resztvevk[i];
			strcat(jelentes, cat);
			voltHianyzo = 1;
		}
	}
	if(voltHianyzo == 0){
		strcat(jelentes, "Mindenki eljött!");
	}
	//printf(jelentes);
	fe=open(myfifo, O_WRONLY);
	write(fe, &jelentes, sizeof(jelentes));
	close(fe);
	unlink(myfifo);
	exit(0);
}

void readSignal(int sig){
		if(sig == SIGUSR1){
			//printf("Signal caught\n");
			readyToSend = 1;
		}
}

void MasodikBead(struct Jelentkezes osszes[], int n, int rendezvenyek[], int rendn){
	printf("Elindítandó rendezvény azonosítója: ");
	int rid;
	scanf("%d", &rid);
	while(RendezvenyEllenorzes(rendezvenyek, rendn, rid) != 1){
		printf("Ez a rendezvény-azonosító nem létezik!\nRendezvény azonosítója: ");
		scanf("%d", &rid);
	}
	
	pid_t pID = fork();
	
	int fd;
	char * myfifo = "myfifo";
	mkfifo(myfifo, 0666);
	
   if (pID == 0)                // child
   {
	   child_function();
    }
	
    else if (pID < 0)            // failed to fork
    {
		printf("Failed to fork\n");
        exit(1);
    }
    //parent
		signal(SIGUSR1, readSignal);
		char helyszin[50];
		printf("Helyszín: ");
		fgets(helyszin, 50, stdin); //Enélkül üresen hagyja
		fgets(helyszin, 50, stdin);
		fd=open(myfifo, O_WRONLY);
		write(fd, helyszin, sizeof(helyszin));
		//close(fd);
		int i;	
		int eredmenyN = 0;
		for(i=0; i<n; i++){
			if(rid == osszes[i].id){
				eredmenyN++;
			}
		}
		//printf("Eredmeny: %d", eredmenyN);
		if(eredmenyN != 0){
			char resztvevok[eredmenyN][50];
			int j = 0;
			for(i=0; i<n; i++){
				if(rid == osszes[i].id){
					strcpy(resztvevok[j], osszes[i].nev);
					//printf("%s", osszes[i].nev);
					j++;
				}
			}	
			while(readyToSend == 0)
				;
			printf("Ready to send!\n");
			//fd=open(myfifo, O_WRONLY);
			write(fd, &eredmenyN, sizeof(eredmenyN));	
			write(fd, &resztvevok, sizeof(resztvevok));	
			//printf("written sg probably\n");
			close(fd);
		}
		else{
			printf("A rendezvénynek nincs résztvevője. Kilépés...");
			exit(0);
		}
		char par_jelentes[300];
		fd=open(myfifo, O_RDONLY);
		read(fd, par_jelentes, sizeof(par_jelentes));
		close(fd);
		printf(par_jelentes);
		unlink(myfifo);

}
	

int main(){
	int b = 0;
	while(b==0){
		FILE* f = fopen("jelentkezesek.data", "ab+"); //ab+ hogy új fájlt készítsen, ha nincs
		int i;
		int n = NoOfStructs();
		printf("Jelentkezések száma: %d\n", n);
		struct Jelentkezes osszes[n];
		for(i=0; i<n; i++){
			fread(&osszes[i], sizeof(struct Jelentkezes), 1, f);
		}
		fclose(f);
		int maxIndex  = osszes[n-1].index;
		FILE* fr = fopen("rendezvenyek.data", "ab+");//ab+ hogy új fájlt készítsen, ha nincs
		int rendn = NoOfInts();
		int rendezvenyek[rendn];
		printf("Rendezvények száma : %d\n", rendn);
		for(i=0;i<rendn; i++){
		fread(&rendezvenyek[i], sizeof(int), 1,fr);
		}
		fclose(fr);
		printf("Üdvözlet a King of Stands nyilvántartásában! Lehetőségek:\n[I] Rendezvény indítása\n[U] Új jelentkezés\n[J] Jelentkezések listázása\n[M] Jelentkezés Módosítása\n[T] Jelentkezés Törlése\n[F] Új Rendezvény Felvétele\n[R] Rendezvények listázása\n[D] Rendezvény Törlése\n[X] Kilépés\n");
		char c=getchar();
		c=toupper(c);
		switch(c){
			case 'I':
				MasodikBead(osszes, n, rendezvenyek, rendn);
				break;
			case 'U':
				UjJelentkezes(maxIndex, rendezvenyek, rendn);
				break;
			case 'J':
				JelListaz(osszes, n);
				break;
			case 'M':
				JelentkezesModosit(osszes, n, rendezvenyek, rendn);
				break;
			case 'T':		
				JelentkezesTorles(osszes, n);
				break;
			case 'F':
				UjRendezveny(rendezvenyek, rendn); 
				break;
			case 'R':
				RendezvenyListaz(rendezvenyek, rendn);
				break;
			case 'D':
				RendezvenyTorles(osszes, n, rendezvenyek, rendn);//TODO: Write it 
				break;
			case 'X':
				b = 1;
				break;
			default:
				b=1;
				break;
		}
		getchar();
	}
	return 0;
}