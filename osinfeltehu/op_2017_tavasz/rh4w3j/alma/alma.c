#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

struct data
{
    char question[145];
    char ans1[144];
    char ans2[144];
    char ans3[144];
    char ans4[144];
    char ido[21];
};
const char* strPipe1Name = "StadionPipe1";
const char* strPipe2Name = "StadionPipe2";
const char* strPipe3Name = "StadionPipe3";
FILE *be_file;
FILE *ki_file;

int valasz_e(char valasz[])
{
    if (strlen(valasz)<144)
    return 1;
    return 0;
}


int kerdes_e(char kerdes[])
{
        int i;
       for( i=0; i<145; i++)
       {
            if(kerdes[i]=='\n')
            {
				
                if(kerdes[i-2]=='?')
                {
					
                       return 1;
                }

            }

       }
        return 0;
}

int kerdes_1(char kerdes[], struct data akt)//hanyadik valasz ures 
{
    int ans;
        if(strlen(akt.ans1)==0)
        {
            ans=1; return ans;
        }
        else
        if(strlen(akt.ans2)==0)
        {
            ans=2; return ans;
        }
        else
        if(strlen(akt.ans3)==0)
        {
            ans=3; return ans;
        }
        else
        if(strlen(akt.ans4)==0)
        {
            ans=4; return ans;
        }
        return -1;

}


int dataload(struct data dat[], int tombszamolo )
{
	    be_file=fopen("vegso.txt","a+");
    char line[145];
     while (fgets(line, sizeof(line), be_file)) {
            if(kerdes_e(line)==1)
            {
                tombszamolo=tombszamolo+1;
               strcpy(dat[tombszamolo].question,line);
            }
            else
            {
                if(kerdes_1(line,dat[tombszamolo])==1)
                {
                    strcpy(dat[tombszamolo].ans1,line);
                }
                else if(kerdes_1(line,dat[tombszamolo])==2)
                {
                    strcpy(dat[tombszamolo].ans2,line);
                }
                else if(kerdes_1(line,dat[tombszamolo])==3)
                {
                    strcpy(dat[tombszamolo].ans3,line);
                }
                else if(kerdes_1(line,dat[tombszamolo])==4)
                {
                    strcpy(dat[tombszamolo].ans4,line);
                }else
                {
                        time_t ido=time(NULL);
                        char buffer[21];
                        strftime(buffer,21, "%Y-%m-%d %H:%M:%S\n", localtime(&ido));
                        strcpy(dat[tombszamolo].ido,buffer);
                }

            }
     printf("%s", line);
    memset(line, 0, 145);

    }
     printf("Fin!  %i \n", tombszamolo);
     fclose(be_file);
	 return tombszamolo;
};

void InitPipes(){
	int result;
	result = mkfifo(strPipe1Name, S_IRUSR|S_IWUSR ); // creating named pipe file
	if (result == -1) {
		printf("Error number: %i\n",errno);
		exit(EXIT_FAILURE);
	}
	
	result = mkfifo(strPipe2Name, S_IRUSR|S_IWUSR ); // creating named pipe file
	if (result == -1) {
		printf("Error number: %i\n",errno);
		exit(EXIT_FAILURE);
	}
	result = mkfifo(strPipe3Name, S_IRUSR|S_IWUSR ); // creating named pipe file
	if (result == -1) {
		printf("Error number: %i\n",errno);
		exit(EXIT_FAILURE);
	}
};

void DestroyPipes(){
	unlink(strPipe1Name);
	unlink(strPipe2Name);
	unlink(strPipe3Name);
}

void buffervivion(struct data dat2[], int tombszamolo)
{
    int i;
    printf("Az Eddigi kerdesbazis: \n\n");
    for(i=1; i<=tombszamolo; i++)
    {
        printf("------------------------------------------\n");
        printf("Ez a(z) %i-edik kerdes: \n", i);
        printf("%s",dat2[i].question);
        printf("%s",dat2[i].ans1);
        printf("%s",dat2[i].ans2);
        printf("%s",dat2[i].ans3);
        printf("%s",dat2[i].ans4);
        printf("%s",dat2[i].ido);
        printf("\n");
        printf("------------------------------------------\n");
    }
     printf("\nAz Eddigi kerdesbazis vege! \n");
      printf("\n");
};
struct valasz 
{
	char question[145];
	int ans1;
	int ans2;
	int ans3;
	int ans4;
	
};
void nullazo(struct valasz a[])
{
	int i;
	for(i=0;i<3; i++)
	{
		strcpy(a[i].question,"");
		a[i].ans1=0;
		a[i].ans2=0;
		a[i].ans3=0;
		a[i].ans4=0;
	}
	
};

void kiir(struct valasz a)
{
	//	int i;
	//for(i=0;i<3; i++)
	//{
		printf("%s\n",a.question);
		printf("%d\n",a.ans1);
		printf("%d\n",a.ans2);
		printf("%d\n",a.ans3);
		printf("%d\n",a.ans4);
	//}
	
};


void Children1Main(){
	int fd;
	pid_t szuloID;
	szuloID=getppid();
	printf("KIE EZ : %d\n", szuloID);
	printf(">>>child1 start<<<\n");
	int start = open(strPipe1Name,O_WRONLY);
	char rdy[10]="I'm ready";
	write(start, &rdy, 10);
	close(start);
	printf("Mehetunk anyu\n");
	///////////////////////////////////////////////
	kill(szuloID,SIGHUP);
	sleep(5);
	////////////////////////// ////////////////
	fd = open(strPipe2Name,O_RDONLY);
	// olvasás 
	int polgarok= (rand() % 11)+10;
	struct valasz kitoltottkerdoiv[3];
	nullazo(kitoltottkerdoiv);
	int valaszdb=0;
	int i;
	for(i=0; i<3;i++)
	{
	struct data temp;
	read(fd, &temp, sizeof(temp));
	//printf("temp: \n");
	//printf("%s %s %s %s %s \n", temp.question, temp.ans1, temp.ans2, temp.ans3, temp.ans4);
	strcpy( kitoltottkerdoiv[i].question, temp.question);
	//printf("temp: \n");
	//printf("%s\n",temp.question );
	//printf("%s\n", kitoltottkerdoiv[i]);
	
		int j;
		for(j=0; j<polgarok; j++)
		{
			int poz;
			poz=kerdes_1("",temp);
			if(poz==-1)
			{
				poz=4;
			}
			//printf("anyu: %d\n",poz);
			int randomvalasz= (rand()% poz )+1;
			//printf("creat random: %d\n",randomvalasz);
			if(randomvalasz==1)
			{
				
				kitoltottkerdoiv[i].ans1++;
			}
			else if(randomvalasz==2)
			{
				
				kitoltottkerdoiv[i].ans2++;
			}
			else if(randomvalasz==3)
			{
				
				kitoltottkerdoiv[i].ans3++;
			}
			else
			{
				
				kitoltottkerdoiv[i].ans4++;
			}
		}
	}
	
	close(fd);
	//eddig volt az polgárok kérdezgetése ------------------------------------------
	//printf("random hurka :%d\n",polgarok);
	//printf("%d %d %d %d\n", kitoltottkerdoiv[0].ans1,kitoltottkerdoiv[0].ans2,kitoltottkerdoiv[0].ans3,kitoltottkerdoiv[0].ans4);
	
	//adatok visszaküldése
	int last;
	last=open(strPipe3Name,O_WRONLY);
	write(last, &kitoltottkerdoiv[0], sizeof(kitoltottkerdoiv[0]));
	write(last, &kitoltottkerdoiv[1], sizeof(kitoltottkerdoiv[1]));
	write(last, &kitoltottkerdoiv[2], sizeof(kitoltottkerdoiv[2]));
	//printf("Elkuldtem anyunak az eredmenyt \n");
	close(last);
	
	kill(szuloID,SIGUSR1);
	sleep(7);////////////////////////////////////////////////////////////////////////
	
	
	
	
};

void Dummy(int a){
	// This line was intentionally left blank
	printf("Dummy signal: a=%d\n", a);
}


int main()
{
	
	signal(SIGUSR1, Dummy);
	signal(SIGHUP, Dummy);
	signal(SIGTERM, Dummy);
	signal(SIGKILL, Dummy);
	struct data adat[40];
	int darab=0;
	darab=dataload(adat, darab);
	//buffervivion(adat,darab);
	InitPipes();
	printf("SzuloID :%d\n", getpid());
	
	//random 
	srand(time(NULL));
	int r1 = (rand() % darab)+1; 
	int r2 = (rand() % darab)+1; 
	int r3 = (rand() % darab)+1; 
	//printf("%d\n%d\n%d\n", r1, r2, r3);
	// 3 random kerdes
	//int polgarok= (rand() % 11)+10;
	//printf("%d\n",polgarok);
	
	
	pid_t p;


	p = fork();		
	//osztódás
	if (p==0) //gyermek
	{
		
		Children1Main();
		//
		printf("kilepek\n");
		
		printf("kilepek\n");
		
	}
	else
	{
	//szulo
	sleep(2);
	printf("Indul a szülő\n");
	int start = open(strPipe1Name,O_RDONLY);
	
	char rdy1[10];
	int rdy=read(start,&rdy1, 10);
	if(rdy<0)
	{
		printf("Connect error\n");
	}
	if(rdy>=0)
	{
		printf("%s\n",rdy1);
	}
	close(start);
	
	printf("Kuldom a csomagot a csovon SZULO\n");
	int fd = open(strPipe2Name,O_WRONLY);	
	int i; 
	write(fd, &adat[r1], sizeof(adat[0]));
	write(fd, &adat[r2], sizeof(adat[0]));
	write(fd, &adat[r3], sizeof(adat[0]));
	close(fd); 
	//printf("%d\n",p);
	kill(p,SIGUSR1);
	sleep(5);///////////////////////////////////////////////////////////////////////////////////////////
	
	printf("Eddig vartam SZULO \n");
	
	int last;
	last=open(strPipe3Name,O_RDONLY);
	struct valasz valaszok[3];
	nullazo(valaszok);
	
	printf("A konzultácio eredménye: \n\n");
	sleep(1);
	int k;
	for(k=0; k<3; k++)
	{
	read(last, &valaszok[i], sizeof(valaszok[i]));
	//kiir(valaszok[i]);
	printf("%sA valasztasok szama:%d\nB valasztasok szama:%d\nC valasztasok szama:%d\nD valasztasok szama:%d\n",valaszok[i].question, valaszok[i].ans1,valaszok[i].ans2,valaszok[i].ans3,valaszok[i].ans4);
	
	}
	//printf("%s\n%s\n%s\n", valaszok[0].question,valaszok[2].question,valaszok[1].question);
	
	close(last);
	kill(p,SIGKILL);
	
	}// else vége
	
	
	DestroyPipes();
    return 0;
}