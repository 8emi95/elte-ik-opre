#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <time.h>
#include <string.h>
#include <unistd.h>  //fork
#include <signal.h>


void handler(int signumber){}

struct Kerdes{
  char kerd[128];
  char va[128];
  char v2[128];
  char v3[128];
  char v4[128];
  time_t ido;
};


void listaz(struct Kerdes kerdesek[], int db){
  int i=0;
  for(i=0;i<db;i++){
    printf("\n");
    printf("%i. kerdes:\n",i+1);
    printf("kerdes letrehozva: %s", asctime(localtime(&kerdesek[i].ido)));
    printf("\n");
    printf("kerdes: ");
    printf(kerdesek[i].kerd);
    
    printf("1.valasz: ");
    printf(kerdesek[i].va);
    printf("\n");
    printf("2.valasz: ");
    printf(kerdesek[i].v2);
    printf("\n");
    printf("3.valasz: ");
    printf(kerdesek[i].v3);
    printf("\n");
    printf("4.valasz: ");
    printf(kerdesek[i].v4);
    printf("\n");
    }
}

void torol(struct Kerdes *kerdesek, int db, int n){
	int i;
	for(i=n;i<db-1;i++){
		strcpy(kerdesek[i].kerd, kerdesek[i+1].kerd);
		strcpy(kerdesek[i].va, kerdesek[i+1].va);
		strcpy(kerdesek[i].v2, kerdesek[i+1].v2);
		strcpy(kerdesek[i].v3, kerdesek[i+1].v3);
		strcpy(kerdesek[i].v4, kerdesek[i+1].v4);
		kerdesek[i].ido= kerdesek[i+1].ido;
		
	}
	strcpy(kerdesek[i].kerd, kerdesek[255].kerd);
	strcpy(kerdesek[i].va, kerdesek[255].va);
	strcpy(kerdesek[i].v2, kerdesek[255].v2);
	strcpy(kerdesek[i].v3, kerdesek[255].v3);
	strcpy(kerdesek[i].v4, kerdesek[255].v4);
	kerdesek[i].ido= kerdesek[255].ido;

}

void modositas(struct Kerdes *mkerd){
	char mker[128];
	char m2ker[128];
	printf("\nkerdes: \n");
	scanf(" ", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->kerd, mker);
	printf("1. valasz: \n");
	//scanf(" %s", &mker);
	scanf(" ", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->va, mker); 
	printf("2. valasz: \n");
	//scanf(" %s", &m2ker);
	scanf(" ", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->v2, mker);
	printf("3. valasz: \n");
	//scanf(" %s", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->v3, mker); 
	printf("4. valasz: \n");
	//scanf(" %s", &mker);
	fgets(mker, sizeof(mker),stdin);
	strcpy(mkerd->v4, mker);
	mkerd->ido=time(0);


}

void menu(){
  printf("1:Kerdes felvetel\n");
  printf("2:Kerdes modositasa\n");
  printf("3:Kerdes torlese\n");
  printf("4:Kerdesek listazasa\n");
  printf("5:Kedoiv veglegesitese\n");
  printf("6:Veglegesitett kerdoiv megtekintese\n");
  printf("7:Kilepes es mentes\n");
}

int main(int argc, char *argv[]){
	struct Kerdes kerdesek[128];
	int db=0;
	struct Kerdes vkerdesek[128];
	srand(time(NULL));
	signal(SIGTERM,handler);
	int fd;
	int pipefd;
	int pipefd1;
	int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR );
	int fid1=mkfifo("fifi.ftc", S_IRUSR|S_IWUSR );
	if (fid==-1)
    {
	printf("Error number: %i",errno);
	exit(EXIT_FAILURE);
    }
if (fid1==-1)
    {
	printf("Error number: %i",errno);
	exit(EXIT_FAILURE);
    }
		

	
	int handle = open("./data.dat", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	int vhandle = open("./vegleges.dat", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if (handle < 0)
 	{
       		printf("Fajl hiba.");
   	}
	
	if (vhandle < 0)
 	{
       		printf("Fajl hiba.");
   	}

	read(handle, &kerdesek, sizeof(kerdesek));
	read(vhandle, &vkerdesek, sizeof(vkerdesek));
	for(;;){
		if(strlen(kerdesek[db].kerd)==0)
			break;
		else
			db++;
	}



	int s=0;
	for(;;){
		if(strlen(vkerdesek[s].kerd)==0)
			break;
		else
			s++;
	}

	char c;
	while(1==1){
	menu();
	scanf(" %c", &c);
	switch(c){
		case '1' :
			modositas(&kerdesek[db++]);
			break;
		case '2':
			listaz(kerdesek, db);
			scanf(" %c", &c);
			if(c<db||c>0){
				modositas(&kerdesek[c-'0'-1]);
			}else{
				printf("%i db kerdes van\n",db);
			}
			break;
		case '3' :
			listaz(kerdesek, db);
			printf("Torlendo kerdes szama: \n");
			scanf(" %c", &c);
			if(c<db||c>0){
				torol(&kerdesek,db,c-'0'-1);
			}else{
				printf("%i db kerdes van\n",db);
			}
			db--;
			break;
		case '4' :
			listaz(kerdesek, db);
			break;
		case '5' :
			s=0;
			while(1 == 1){
				listaz(kerdesek, db);
				printf("Hanyas szamu legyen az %i.kerdes(x a lista befejezesehez): \n",s+1);
				scanf(" %c", &c);

				if(c<db||c>0||c=='x'){
					if(c=='x')
						break;
					else{
						
						strcpy(vkerdesek[s].kerd, kerdesek[c-'0'-1].kerd);
						strcpy(vkerdesek[s].va, kerdesek[c-'0'-1].va);
						strcpy(vkerdesek[s].v2, kerdesek[c-'0'-1].v2);
						strcpy(vkerdesek[s].v3, kerdesek[c-'0'-1].v3);
						strcpy(vkerdesek[s].v4, kerdesek[c-'0'-1].v4);
						vkerdesek[s].ido= kerdesek[c-'0'].ido;
						s++;
						
					}
				}else{
					printf("%i db kerdes van\n",db);
				}
			}	
			pid_t child1=fork();

				if(child1> 0) {
					printf("szulo\n");
					pause();
					printf("pauseutan\n");
					int v=0;
					char c[128];
					pipefd=open("fifo.ftc",O_RDONLY);
					pipefd1=open("fifi.ftc",O_WRONLY);

					if(s<3){
						int i=0;
 						for(i=0 ;i<s; i++){
							printf("elsociklus szulo\n");
							write(pipefd1 ,&vkerdesek[i],sizeof(vkerdesek[i]));//kuldes 
							printf("kuldve: %s",vkerdesek[i].kerd);
							sleep(1);
							kill(child1,SIGTERM);

							pause();
							
							printf("Eredmenyek:\n");
							int i=0;
							for(i=0;i<4;i++){
								pause();
								read(pipefd ,c,sizeof(c));
								printf("pread: %s",c);
							sleep(1);
								
								kill(child1,SIGTERM);

								
								printf("%i. valaszra %s db szavazat\n",i+1,c);
							}
							
							
							/*
							for(;i<3;i++){
								write(pipefd1 ,"",sizeof(""));
								write(pipefd1 ,"",sizeof(""));
								write(pipefd1 ,"",sizeof(""));
								write(pipefd1 ,"",sizeof(""));
								write(pipefd1 ,"",sizeof(""));
								for(i=0;i<4;i++){
									read(pipefd ,c,sizeof(c));
								}
	
							}*/
						}
						write(pipefd1,"",sizeof(""));
							sleep(1);
							kill(child1,SIGTERM);
					}else{
						printf("random kuldeshez");
						int e=rand()%s;
						printf("e random:%i|\n",e);
						int m;
						int h;
						
						do{
							m=rand()%s;
						}while(e==m);
						printf("m rand:%i|\n",m);
						do{
							h=rand()%s;
						}while(h==m || e==h);
						printf("h rand:%i|\n",h);
						//kuldes emh l
							write(pipefd1 ,&vkerdesek[e],sizeof(vkerdesek[e]));//kuldes 
							printf("elkulve: %s\n",vkerdesek[e].kerd);
							sleep(1);
							kill(child1,SIGTERM);
							pause();

							printf("Eredmenyek:\n");
							int i=0;
							for(i=0;i<4;i++){
								pause();
								read(pipefd ,c,sizeof(c));
								printf("pread: %s",c);
							sleep(1);
								
								kill(child1,SIGTERM);

								
								printf("%i. valaszra %s db szavazat\n",i+1,c);
							}
 							
							write(pipefd1 ,&vkerdesek[m],sizeof(vkerdesek[m]));//kuldes 

							sleep(1);
							kill(child1,SIGTERM);

							pause();
							
							printf("Eredmenyek:\n");
							i=0;
							for(i=0;i<4;i++){
								pause();
								read(pipefd ,c,sizeof(c));
								printf("pread: %s",c);
							sleep(1);
								
								kill(child1,SIGTERM);

								
								printf("%i. valaszra %s db szavazat\n",i+1,c);
							}



							
							write(pipefd1 ,&vkerdesek[h],sizeof(vkerdesek[h]));//kuldes
							sleep(1);
							kill(child1,SIGTERM);

							pause(); 
							
							
							printf("Eredmenyek:\n");
							i=0;
							for(i=0;i<4;i++){
								pause();
								read(pipefd ,c,sizeof(c));
								printf("pread: %s",c);
							sleep(1);
								
								kill(child1,SIGTERM);

								
								printf("%i. valaszra %s db szavazat\n",i+1,c);
							}

					}
					close(pipefd);
					close(pipefd1);
					
					
					
						
 				 }
  				else
  				{
     					printf("Child 1\n");
					printf("A kerdezeobitos megekerkezet\n");
							sleep(1);
					kill(getppid(),SIGTERM);
					printf("signal\n");
					struct Kerdes mkerd[3];
					int sz=rand()%10+10;
					printf("random: %i\n",sz);
					pipefd=open("fifo.ftc",O_WRONLY);
					pipefd1=open("fifi.ftc",O_RDONLY);
					printf("fifokesz\n");
					char s[128];
					int j=0;
					int tsz=sz;
					int r=tsz;
					for(j=0; j<3; j++){
						printf("csofogadas\n");

						pause();
						read(pipefd1 , &mkerd[j],sizeof(mkerd[j]));
						printf("kerdes: %s hossza:%i\n",mkerd[j].kerd,strlen(mkerd[j].kerd));
							sleep(1);
						kill(getppid(),SIGTERM);
						printf("killppid\n");
						if(strlen(mkerd[j].kerd)==0){
							printf("exit\n");
							exit(1);
							printf("megseexit\n");
						}
	
					 	
										 

					
					tsz=sz;
					r=rand()%tsz;
					tsz-=r;
					char str[15];
					sprintf(str, "%d", r);
					printf("childpause\n");
					write(pipefd ,str,sizeof(str));
					printf("kr: %s\n",str);
							sleep(1);
					kill(getppid(),SIGTERM);
					pause();
					

										
					
						if(strcmp(mkerd[j].v3,"")==0){
							sprintf(str, "%d", tsz);
							printf("kuldott rand:%s\n",str);
							write(pipefd ,str,sizeof(str));//visszakuldeni tsz
							sleep(1);
							kill(getppid(),SIGTERM);
							pause();
						}else{
							r=rand()%tsz;
							tsz-=r;
							sprintf(str, "%d", r);
							printf("xxxxxx\n");
							write(pipefd ,"x",sizeof("x"));//r visszakuldeni
							sleep(1);
							kill(getppid(),SIGTERM);
							pause();
						}
					if(strcmp(mkerd[j].v4,"")==0){
							sprintf(str, "%d", tsz);
							write(pipefd ,str,sizeof(str));//visszakuldeni tsz
							sleep(1);
							kill(getppid(),SIGTERM);
							pause();
						}else{
							r=rand()%tsz;
							tsz-=r;
							sprintf(str, "%d", r);
							write(pipefd ,str,sizeof(str));//r visszakuldeni

							sleep(1);
							kill(getppid(),SIGTERM);

							pause();
							sprintf(str, "%d", tsz);
							write(pipefd ,str,sizeof(str));
							sleep(1);
							kill(getppid(),SIGTERM);
							pause();
						}
					
					}
					close(pipefd);
					close(pipefd1);
					
					exit(0);
					
  				








			}
			break;
		case '6' :
			listaz(vkerdesek, s);
			break;
			
		case '7':
			
			remove ("./fifo.ftc");
			remove ("./fifi.ftc");
			remove("./data.dat");
			remove("./vegleges.dat");

			int handle = open("./data.dat", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
 			if (handle < 0)
 			{
       				printf("Fajl hiba.");
   			}
   			write(handle, &kerdesek, sizeof(kerdesek));



			int vhandle = open("./vegleges.dat", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
   			if (vhandle < 0)
   			{
       				printf("Fajl hiba.");
   			}
   			write(vhandle, &vkerdesek, sizeof(vkerdesek));
			exit(1);
		default :
			printf("1 es 7 kozotti szamot var a program(%c)",c);

	}
	}
	

}
