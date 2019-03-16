/*
mintafájl: data.txt
A mintafálj esetén az első sorban vana  kérdés, utána 4 soron leresztül az egy-egy válaszlehetőség. 
Ha a felhasználó nem akar az elején megadni egy válaszlehetőséget, akkor azt a sor üresen kell hagyni. 
Két kérdés között nem kell ssort kihagyni!

A prpogram minden fontos tudnivalót, instrukciót megleníta  konzolon. Ha ezeket követi a felhsználó nem lesz probléma.
A program nem támogatja az ékezetes betűket!

tesztelési- és fejlesztési környezet: Windows 10, Notepad++

*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <string.h>
#include <sys/wait.h> 

typedef enum { false, true } bool;

struct Answer {
    char first [1024];
    char second [1024];
    char three [1024];
    char four [1024];
};

struct Question {
    char quest [1024];
    struct Answer *ans;
    time_t questtime;
    struct Question *next;
	int nummber;
};

void menu();
// új kérdés hozzáadása esetén hívódik meg
void addQuest( struct Question *first, struct Question *new );
// kérdés-válaszok kiiratása/lekérdezése esetén hívódik meg
void questAnsw(struct Question *first);
// kérdés törlése esetén hívódik meg
void delQuest( struct Question *first);
// kérdés változtatása esetén hívódik meg
void changQuest( struct Question *first);
// a kérdőív véglegesítésekor hívódik meg
void printFile(struct Question *first, FILE *out);


struct Question* randomGenerator(struct Question *first)
{
	int osszes = 0;
	struct Question *q = first->next;
	if(q == NULL)
	{ 
		osszes = 0;
	}
	while( q != NULL)
	{
		osszes += 1;
		q = q->next;
	}

	srand(time(NULL));
	int random1 = (rand() % osszes)+1;
	int random2 = (rand() % osszes)+1;
	if(random1 == random2)
	{
		random2 = (rand() % osszes)+1;
	}
	int random3 = (rand() % osszes)+1;
	if(random1 == random2 || random2 == random3 || random1 == random3)
	{
		random3 = (rand() % osszes)+1;
	}
	struct Question *kival = malloc(sizeof(struct Question));
	struct Question *p = first->next;
	while(p != NULL &&  p->nummber != random1)
	{
		p = p->next;
	}
	strcpy(kival->quest, p->quest);
	kival->ans = p->ans;
	kival->nummber = p->nummber;
	p = first->next;

	while(p != NULL &&  p->nummber != random2)
	{
		p = p->next;
	}
	kival->next = malloc(sizeof(struct Question));
	strcpy(kival->next->quest, p->quest);
	kival->next->ans = p->ans;
	kival->next->nummber = p->nummber;
	p = first->next;

	while(p != NULL &&  p->nummber != random3)
	{
		p = p->next;
	}
	kival->next->next = malloc(sizeof(struct Question));
	strcpy(kival->next->next->quest,p->quest);
	kival->next->next->ans = p->ans;
	kival->next->next->nummber = p->nummber;
	kival->next->next->next = NULL;
	return kival;
}


void handler(int signal_number)
{
	printf("Kesz vagyok");
}

void beszelFork(struct Question *first) 
{
	struct Question *adat1 = malloc(sizeof(struct Question));
	struct Question *adat2 = malloc(sizeof(struct Question));

	signal(SIGUSR1, handler);	

	int darabszam = 3;
	int pipefd[2];
	pid_t child;
	adat2 = randomGenerator(first);
	if (pipe(pipefd) == -1) 
	{
       perror("Hiba a pipe nyitaskor!");
       exit(EXIT_FAILURE);
    }
    child = fork();
    if (child == -1) 
	{
	    perror("Fork hiba");
	    exit(EXIT_FAILURE);
	}

	if(child == 0)
	{
		printf("[GYEREK] Gyerek var az adatokra: \n");

		kill(getppid(), SIGUSR1);		

		read(pipefd[0], &adat1, darabszam*sizeof(adat1));
		//read(pipefd[0], &adat1, 3*sizeof(adat1));
		//close(pipefd[0]);
		printf("[GYEREK] Kiolvasta az adatokat a csobol!\n");
		//printf("Ellenorzo:\n");
		struct Question *pa = adat1;
		/*while( pa != NULL)
		{
			printf("[GYEREK] %s\n", pa->quest);
			printf("[GYEREK] %i\n", pa->nummber);
			printf("[GYEREK] %s\n", pa->ans->first);
			printf("[GYEREK] %s\n", pa->ans->second);
			printf("[GYEREK] %s\n", pa->ans->three);
			printf("[GYEREK] %s\n", pa->ans->four);
			pa = pa->next;
		}*/
		srand(time(NULL));
		int randomitt = (rand() % 10 )+ 10;
		write(pipefd[1], &randomitt, sizeof(randomitt));
		printf("[GYEREK] Randomitt: %d\n", randomitt);
		int eredmeny[3*randomitt];
		int i;
		for(i = 0; i < randomitt; ++i)
		{
			pa = adat1;
			int j = 0;
			while( pa != NULL)
			{
				int sum = 0;
				if(strcmp(pa->ans->first,"") != 0) sum += 1;
				if(strcmp(pa->ans->second,"") != 0) sum += 1;
				if(strcmp(pa->ans->three,"") != 0) sum += 1;
				if(strcmp(pa->ans->four,"") != 0) sum += 1;
				
				srand(time(NULL));
				//printf("[GYEREK] Sum: %d", sum);
				int r = rand() % sum + 1;
				//printf("[GYEREK] Felhasznalo: %d Hanyadik kerdes: %d Valasz: %d\n", i,j,r);
				eredmeny[i*3+j] = r;
				j += 1;
				pa = pa->next;
			}
		}
		write(pipefd[1], &eredmeny, sizeof(eredmeny));
		close(pipefd[1]);
		printf("[GYEREK] Gyerek teljesitette a kuldetest.\n"); 
		printf("[GYEREK] Pause\n");
		kill(getpid(), SIGKILL);
	}
	else
	{
		//szülő
		printf("[SZULO] Szulo vagyok... \n");
		pause;
		printf("[SZULO] adat2 kuldese: \n");
		write(pipefd[1], &adat2, darabszam * sizeof(adat2));
		printf("[SZULO] adat2 ellenorzese: \n");
		struct Question *qa = adat2;
		/*while( qa != NULL)
		{
			printf("[SZULO] %s\n", qa->quest);	
			printf("[SZULO] %i\n", qa->nummber);
			qa = qa->next;
		}*/
		close(pipefd[1]);
		int status;
		printf("[SZULO] Waiting child\n");
		waitpid(child,&status,0);
		int duplarand;
		read(pipefd[0], &duplarand, sizeof(duplarand));
		printf("[SZULO] Duplarand: %i\n", duplarand);	
		int eredmeny2[3*duplarand];
		read(pipefd[0], &eredmeny2, sizeof(eredmeny2));

		int i;
		int userNumber = 1;
		for(i = 0; i < 3*duplarand; i = i + 3)
		{
			qa = adat2;
			int j = 0;
			while( qa != NULL)
			{
				printf("[SZULO] Felhasznalo: %d Hanyadik kerdes: %d Valasz: %d\n", userNumber,j+1,eredmeny2[i+j]);
				qa = qa->next;
				j = j + 1;
			}
			userNumber += 1;
		}
		printf("[SZULO] close\n");
	}

}

int main(int argc,char** argv) 
{
    struct stat st;
    stat(argv[0],&st);
    struct Question *first = malloc(sizeof(struct Question));	//fejelemes
    first->next = NULL;
	first->nummber = 0;
    int ansNum;
    int i = 0;
	
	printf("Add meg a bemeneti fajl nevet, kitejesztessel egyutt:\t");
	char dataIn[1024];
	scanf("%s", &dataIn);
    FILE *data = fopen(dataIn,"r");
	
	struct stat info;
    int ret = stat(dataIn, &info);

//Beolvassa a fájlban tárolt adatokat.	
			
	while(!feof(data) && info.st_size > 0) {
		struct Question *new = malloc(sizeof(struct Question));
		new->ans = malloc(sizeof(struct Answer));
		
		fgets(new->quest, 1024, (FILE*)data);
		if(strcmp(new->quest,"\n") == 0)
		{
			strcpy(new->quest, "");
		}
		strtok(new->quest, "\r\n");
		
		fgets(new->ans->first, 1024, (FILE*)data);
		if(strcmp(new->ans->first,"\n") == 0)
		{
			strcpy(new->ans->first, "");
		}
		strtok(new->ans->first, "\r\n");
	
		fgets(new->ans->second, 1024, (FILE*)data);
		if(strcmp(new->ans->second,"\n") == 0)
		{
			strcpy(new->ans->second, "");
		}
		strtok(new->ans->second, "-\r\n");
	
		fgets(new->ans->three, 1024, (FILE*)data);
		if(strcmp(new->ans->three,"\n") == 0)
		{
			strcpy(new->ans->three, "");
		}
		strtok(new->ans->three, "-\r\n");
		
		fgets(new->ans->four, 1024, (FILE*)data);
		if(strcmp(new->ans->four,"\n") == 0)
		{
			strcpy(new->ans->four, "");
		}
		strtok(new->ans->four, "-\r\n");
	
		if(strlen(new->quest) != 0) 
		{
			addQuest(first,new);
		}
	}
	
	fclose(data);
    do 
    {
        menu();
        scanf("%d", &ansNum);
		char c;
		while((c = getchar()) != '\n' && c != EOF);
        switch(ansNum) 
        {
            case 1: 
                printf("Add meg az uj kerdest:\t");
                time_t t = st.st_mtime;
                struct Question *new = malloc(sizeof(struct Question));
                new->ans = malloc(sizeof(struct Answer));
                new->questtime = t;
                strcpy(new->ans->first,"");
				char newQ [1024];
				scanf("%[^\n]",&newQ);
				while((c = getchar()) != '\n' && c != EOF);
                strcpy(new->quest, newQ);
                strtok(new->quest, "\r\n");
				int inA;
                do {   
                    printf("Hany uj valaszt akarsz?\t"); 
                    scanf("%i", &inA);
                } while(inA > 4);
				while((c = getchar()) != '\n' && c != EOF);
                int inC = 0;
                printf("Add meg az uj valaszokat:\n");
                char newA [1024] ;
 			
				do
                {
                    scanf("%[^\n]", &newA);
					while((c = getchar()) != '\n' && c != EOF);
					
					if (strlen(new->ans->first) == 0)
                    {
						strtok(new->ans->first, "\r\n");
                        strcpy(new->ans->first, newA);
			        }
                    else if(strlen(new->ans->second) == 0)
                    {
						strtok(new->ans->second, "\r\n");
						strcpy(new->ans->second, newA);
                    }
                    else if(strlen(new->ans->three) == 0)
                    {						
						strtok(new->ans->three, "\r\n");
                        strcpy(new->ans->three, newA);
                    }
                    else if(strlen(new->ans->four) == 0)
                    {
						strtok(new->ans->four, "\r\n");						
                        strcpy(new->ans->four, newA);
                    }
                    inC++;

				}while( inC != inA);
 
				addQuest(first, new);
                break; 
            case 2:
				delQuest(first);
                break;
            case 3:
				changQuest(first);
                break;
            case 4:
                questAnsw(first);
                break;
            case 5:
				printf("Add meg a kimeneti fajl nevet, kitejesztessel egyutt:\t");
				char outF[1024];
				scanf("%s", &outF);
				FILE *out= fopen(outF,"w");
				printFile(first,out);
				fclose(out);
                break;
            case 6: 
            	//randomGenerator(first);
            	beszelFork(first);
            	break;
        }
    } while (ansNum != 9 );    
    

    return 0;
}

void menu() 
{
    printf("Neptun kod: HFYCXJ\n");
    printf("1\tKerdes felvetele\n");
    printf("2\tKerdes torlese\n");
    printf("3\tKerdes modositasa\n");
    printf("4\tKerdes-Valasz listazasa\n");
    printf("5\tKerdesek veglegesitese\n");
    printf("6\tEredmeny osszesites eredmenye\n");
    printf("9\tKilepes\n");
}

void addQuest( struct Question *first, struct Question *new )
{
    struct Question *q = first;
    while( q->next != NULL)
    {
        q = q->next;
    }
	new->nummber = q->nummber + 1;
    q->next = new;
    q->next->next = NULL;
}

void questAnsw(struct Question *first)
{
    struct Question *q = first->next;
    if(q == NULL)
    { 
        printf("Nincs kerdes.\n");
        return;
    }
    while( q != NULL)
    {
		printf("Sorszam: %i\n", q->nummber);
		printf("%s\n", q->quest);
		printf("%s\n", q->ans->first);
		printf("%s\n", q->ans->second);
		printf("%s\n", q->ans->three);
		printf("%s\n\n", q->ans->four);;
		
		q = q->next;
    }
}

void delQuest( struct Question *first)
{
	struct Question *q = first->next;
	struct Question *qF =q;
    if(q == NULL)
    { 
        printf("Nincs kerdes.\n");
        return;
    }
	printf("Add meg a torolni kivant kerdes sorszamat:\t");
	int delQ;
	scanf("%i", &delQ);
	while(q != NULL  && q->nummber != delQ )
	{
		printf("%i\n", delQ);
		qF = q;
		q = q->next;
	}

	printf("%s\n", q->quest);
	printf("%s\n", q->ans->first);
	printf("%s\n", q->ans->second);
	printf("%s\n", q->ans->three);
	printf("%s\n\n", q->ans->four);
	if(q == NULL)
    { 
        printf("Nincs ilyen kerdes.\n");
        return;
    }
   
	int delNum;
	do 
    {
		printf("1\tKerdes torlese! Ebben az esetben a valaszlehetosegek is torlodni fognak.\n");
		printf("2\tElso valaszlehetoseg torlese\n");
		printf("3\tMasodik valaszlehetoseg torlese\n");
		printf("4\tHarmadik valaszlehetoseg torlese\n");
		printf("5\tNegyedik valaszlehetoseg torlese\n");
		printf("9\tKilepes\n");
		printf("Valassz mit szeretnel belole torolni:\t");
        scanf("%d", &delNum);
        switch(delNum) 
        {
            case 1: 
				if(delQ == 1) {
					first->next = q->next;
				} else {
					qF->next = q->next;
				}
				free(q->ans);
				free(q);
				return;
                break; 
            case 2:
				strcpy(q->ans->first,"");
				return;
                break;
            case 3:
				strcpy(q->ans->second,"");
				return;
				break;
            case 4:
               	strcpy(q->ans->three,"");
				return;
                break;
            case 5:
				strcpy(q->ans->four,"");
				return;
                break;
        }
    } while (delNum != 9 );    
}

void changQuest( struct Question *first)
{
	struct Question *q = first->next;
    if(q == NULL)
    { 
        printf("Nincs kerdes.\n");
        return;
    }
	printf("Add meg a modositani kivant kerdes sorszamat:\t");
	int chanQ;
	scanf("%i", &chanQ);
	while(q != NULL &&  q->nummber != chanQ)
	{
		q = q->next;
	}
	printf("%s\n", q->quest);
	printf("%s\n", q->ans->first);
	printf("%s\n", q->ans->second);
	printf("%s\n", q->ans->three);
	printf("%s\n\n", q->ans->four);
	if(q == NULL)
    { 
        printf("Nincs ilyen kerdes.\n");
        return;
    }
   
	int chanNum;
	char newChan[1024];
	do 
    {
		printf("1\tKerdes modositasa!\n");
		printf("2\tElso valaszlehetoseg modositasa\n");
		printf("3\tMasodik valaszlehetoseg modositasa\n");
		printf("4\tHarmadik valaszlehetoseg modositasa\n");
		printf("5\tNegyedik valaszlehetoseg modositasa\n");
		printf("9\tKilepes\n");
		printf("Valassz mit szeretnel belole modositani:\t");
        scanf("%i", &chanNum);
		char c;
        switch(chanNum) 
        {
            case 1: 
				while((c = getchar()) != '\n' && c != EOF);
                printf("Add meg az uj kerdest!\t");
				scanf("%[^\n]",&newChan);
				strtok(newChan, "\r\n");
                strcpy(q->quest, newChan);
				break;
            case 2:						
				while((c = getchar()) != '\n' && c != EOF);
				printf("Add meg az uj valaszlehetoseget!\t");
				scanf("%[^\n]",&newChan);
				strtok(newChan, "\r\n");
				strcpy(q->ans->first, newChan);
                break;
            case 3:						
				while((c = getchar()) != '\n' && c != EOF);
				printf("Add meg az uj valaszlehetoseget!\t");
				scanf("%[^\n]",&newChan);
				strtok(newChan, "\r\n");
				strcpy(q->ans->second, newChan);
				break;
            case 4:						
				while((c = getchar()) != '\n' && c != EOF);
               	printf("Add meg az uj valaszlehetoseget!\t");
				scanf("%[^\n]",&newChan);
				strtok(newChan, "\r\n");
				strcpy(q->ans->three, newChan);
                break;
            case 5:
				while((c = getchar()) != '\n' && c != EOF);
				printf("Add meg az uj valaszlehetoseget!\t");
				scanf("%[^\n]",&newChan);	
				strtok(newChan, "\r\n");
				strcpy(q->ans->four, newChan);
                break;
        }
    } while (chanNum != 9 );
}

void printFile(struct Question *first, FILE *out)
{
	struct Question *q = first->next;
	if(q == NULL)
	{ 
		fprintf(out,"");
		return;
	}
	while( q != NULL)
	{
		if(first->next == q)
		{
			fprintf(out,"%s\n", q->quest);
			fprintf(out,"%s\n", q->ans->first);
			fprintf(out,"%s\n", q->ans->second);
			fprintf(out,"%s\n", q->ans->three);
			fprintf(out,"%s", q->ans->four);
		}
		else
		{
			fprintf(out,"\n%s\n", q->quest);
			fprintf(out,"%s\n", q->ans->first);
			fprintf(out,"%s\n", q->ans->second);
			fprintf(out,"%s\n", q->ans->three);
			fprintf(out,"%s", q->ans->four);
		}

		q = q->next;
	}
}
