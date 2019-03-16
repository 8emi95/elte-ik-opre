#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>
#include <time.h> //ctime
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int signalFlag = 0;

void handler(int signumber)
{
	if(signumber==2){
		printf("Interrupted by signal!\n");
		exit(-1);
	}else{
   signalFlag = signumber;
	}
}


int getLines(char fileName[],pid_t pid){
	int status;
	FILE* myfile = fopen(fileName, "r");
	if (myfile == NULL)
	{
		printf("Error opening file!\n");
		kill(pid,SIGINT);
		wait(&status);
		exit(-1);
	}
	int ch, lines = 0;

	do 
	{
		ch = fgetc(myfile);
		if(ch == '\n')
			lines++;
	} while (ch != EOF);

	if(ch != '\n' && lines != 0) 
		lines++;

	fclose(myfile);

	return lines;
}

int main(int argc,char** argv){
	/*srand(time(NULL)); //the starting value of random number generation
	int r=rand()%100; //number between 0-99
	printf("Random number %i\n",r);
	return 0;
	
	struct student
	{
	  char name[80];
	  int year;
	};
	struct student who={"Who am i?",2014}; // define a data structure to send
	
	printf("Waits 3 seconds, then send a SIGUSR %i signal (it is not waited for by suspend) - so the suspend continues waiting\n", SIGUSR1);
    sleep(3);
    kill(getppid(),SIGUSR1);*/
	
	int status;
	//int notacommonvalue=1;
	//printf("The value is %i before forking \n",notacommonvalue);
	srand(time(NULL));
	
	signal(SIGTERM, handler);
	signal(SIGUSR1, handler);
	signal(SIGINT, handler);

	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;

	char sz[100];  // char array for reading from pipe

	if (pipe(pipefd) == -1) 
	{
	perror("Hiba a pipe nyitaskor!");
	exit(EXIT_FAILURE);
	}
	

	pid =fork(); //forks make a copy of variables
	if (pid<0){perror("The fork calling was not succesful\n"); exit(1);} 
	if (pid>0) //the parent process, it can see the returning value of fork - the child variable!
	{
		char varos[20];
		if(argv[1]==NULL){
		  printf("Nincs megadott argumentum!\n");
		  kill(pid,SIGINT);
		  wait(&status);
		  exit(-1);
		  //pid=waitpid(getpid(), NULL, WNOHANG);
		 // do{
			 // printf("pina");
			 // pid=waitpid(-1,&status,0);
			//pid=wait(&status);
		 // }while(pid!=-1);
		  //waitpid(pid,&status,0);
		  
		}
		strcpy(varos,argv[1]);
		
		
		printf("Szulo indul!\n");
		//close(pipefd[0]); //Usually we close unused read end
		//pause();
		//kill(pid,SIGUSR1);
		write(pipefd[1], varos,20);
		
		int i=0;
		int kerdesSzama[3]={0,0,0};
		while(i!=3){
			int r=(rand()%(getLines("vegKerdesek",pid)/7))+1;
			if (r==kerdesSzama[0] || r==kerdesSzama[1]){
				continue;
			}
			if (r<kerdesSzama[1]){
				kerdesSzama[2]=r;
			}else if(r<kerdesSzama[0]){
				kerdesSzama[2]=kerdesSzama[1];
				kerdesSzama[1]=r;
			}else{
				kerdesSzama[2]=kerdesSzama[1];
				kerdesSzama[1]=kerdesSzama[0];
				kerdesSzama[0]=r;
			}
			i++;
		}
		
		                    // kerdesSzama[2]=1;
		
		//printf("\nKerdesekSzama: %d %d %d \n",kerdesSzama[0],kerdesSzama[1],kerdesSzama[2]);
		
		FILE *f = fopen("vegKerdesek","r");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			kill(pid,SIGINT);
		    wait(&status);
		    exit(-1);
			
		}
		char kerdesek[15][120];
		char tmpCh;
		char dump[120];
		int sorSzam=1;
		
		while((sorSzam/7)+1!=kerdesSzama[2]){
			tmpCh=getc(f);
			while(tmpCh!='\n'){
				tmpCh=getc(f);
			}
			sorSzam++;
		}
		
		//printf("\nSorszam:%d\n",sorSzam);
		
		if(kerdesSzama[2]!=1)fgets(dump,120,f);
		//tmpCh=getc(f);
		fgets(kerdesek[0],120,f);
		//kerdesek[0][120]='\0';
		fgets(kerdesek[1],120,f);
		fgets(kerdesek[2],120,f);
		fgets(kerdesek[3],120,f);
		fgets(kerdesek[4],120,f);
		if(kerdesSzama[2]!=1){sorSzam+=6;}else{sorSzam+=5;}
		while((sorSzam/7)+1!=kerdesSzama[1]){
			tmpCh=getc(f);
			while(tmpCh!='\n'){
				tmpCh=getc(f);
			}
			sorSzam++;
		}
		
		//printf("\nSorszam:%d\n",sorSzam);
		fgets(dump,120,f);
		//tmpCh=getc(f);
		fgets(kerdesek[5],120,f);
		fgets(kerdesek[6],120,f);
		fgets(kerdesek[7],120,f);
		fgets(kerdesek[8],120,f);
		fgets(kerdesek[9],120,f);
		if(kerdesSzama[2]!=1){sorSzam+=6;}else{sorSzam+=6;}
		while((sorSzam/7)+1!=kerdesSzama[0]){
			tmpCh=getc(f);
			while(tmpCh!='\n'){
				tmpCh=getc(f);
			}
			sorSzam++;
		}
		
		//printf("\nSorszam:%d\n",sorSzam);
		fgets(dump,120,f);
		fgets(kerdesek[10],120,f);
		fgets(kerdesek[11],120,f);
		fgets(kerdesek[12],120,f);
		fgets(kerdesek[13],120,f);
		fgets(kerdesek[14],120,f);
		fclose(f);
		
		//printf("\nKerdes: %s \nkerdes: %s \nkerdes: %s\n %s\n %s\n %s\n %s\n %s\n %s\n %s\n %s\n %s\n %s\n %s\n %s\n",kerdesek[0],kerdesek[1],kerdesek[2],kerdesek[3],kerdesek[4],kerdesek[5],kerdesek[6],kerdesek[7],kerdesek[8],kerdesek[9],kerdesek[10]
		// ,kerdesek[11],kerdesek[12],kerdesek[13],kerdesek[14]);
		
		pause();
		////////////////
		//printf("Szulosignalutan\n");
		
		write(pipefd[1], kerdesek[0], 120);
		write(pipefd[1], kerdesek[1], 120);
		write(pipefd[1], kerdesek[2], 120);
		write(pipefd[1], kerdesek[3], 120);
		write(pipefd[1], kerdesek[4], 120);
		write(pipefd[1], kerdesek[5], 120);
		write(pipefd[1], kerdesek[6], 120);
		write(pipefd[1], kerdesek[7], 120);
		write(pipefd[1], kerdesek[8], 120);
		write(pipefd[1], kerdesek[9], 120);
		write(pipefd[1], kerdesek[10], 120);
		write(pipefd[1], kerdesek[11], 120);
		write(pipefd[1], kerdesek[12], 120);
		write(pipefd[1], kerdesek[13], 120);
		write(pipefd[1], kerdesek[14], 120);
		
		
		
		//close(pipefd[1]); // Closing write descriptor 
		
		printf("Szulo beirta az adatokat a csobe!\n");
		char hanytmp[10];
		int hanyember;
		char szoveg[120];
		/*fwrite(kerdesek[0],1,120,stdout);
		fwrite(kerdesek[5],1,120,stdout);
		fwrite(kerdesek[10],1,120,stdout);*/
		printf("\nA feltett kerdesek a kovetkezok:\n%s\n%s\n%s\n",kerdesek[0],kerdesek[5],kerdesek[10]);
		//fflush(stdout);
		pause();
		read(pipefd[0], hanytmp, 10);
		hanyember=atoi(hanytmp);
		
		for(i=0;i<hanyember;i++){
			
			read(pipefd[0], szoveg, 60);
			printf("%s\t",szoveg);
			read(pipefd[0], szoveg, 120);
			printf("%s\t",szoveg);
			read(pipefd[0], szoveg, 120);
			printf("%s\t",szoveg);
			read(pipefd[0], szoveg, 120);
			printf("%s\n",szoveg);
			
		}
		
		
		
		close(pipefd[0]);
		close(pipefd[1]);
		fflush(NULL); 	// flushes all write buffers (not necessary)
		wait(NULL);		// waiting for child process (not necessary)
				// try it without wait()
		printf("Szulo befejezte!\n");	

		
		
		//waitpid(pid,&status,0); 
		//waits the end of child process PID number=child, the returning value will be in status
		//0 means, it really waits for the end of child process - the same as wait(&status)
		//printf("The value is %i in parent process (remain the original) \n",notacommonvalue);

	}
	else //child process
	{
		
		
		//sleep(3);	// sleeping a few seconds, not necessary
		//kill(getppid(), SIGTERM);
		//sleep(3);
		//kill(getppid(), SIGUSR1);
		//close(pipefd[1]);  //Usually we close the unused write end
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		char kapottVaros[20];
		//pause();
		read(pipefd[0],kapottVaros,20); // reading max 100 chars
		printf("Gyerek olvasta uzenet: %s\n",kapottVaros);
		FILE *f = fopen(kapottVaros,"r");
		if (f == NULL)
		{
			printf("Error opening file!\n");
			kill(pid,SIGINT);
		    wait(&status);
		    exit(-1);
			return;
		}
		char kapottKerdesek[15][120];
		sleep(2);
		kill(getppid(), SIGUSR1);
		
		read(pipefd[0], kapottKerdesek[0], 120);
		//printf("Signalutan\n");
		read(pipefd[0], kapottKerdesek[1], 120);
		read(pipefd[0], kapottKerdesek[2], 120);
		read(pipefd[0], kapottKerdesek[3], 120);
		read(pipefd[0], kapottKerdesek[4], 120);
		read(pipefd[0], kapottKerdesek[5], 120);
		read(pipefd[0], kapottKerdesek[6], 120);
		read(pipefd[0], kapottKerdesek[7], 120);
		read(pipefd[0], kapottKerdesek[8], 120);
		read(pipefd[0], kapottKerdesek[9], 120);
		read(pipefd[0], kapottKerdesek[10], 120);
		read(pipefd[0], kapottKerdesek[11], 120);
		read(pipefd[0], kapottKerdesek[12], 120);
		read(pipefd[0], kapottKerdesek[13], 120);
		read(pipefd[0], kapottKerdesek[14], 120);
		//printf("Masodik uzenet: %s\n", kapottKerdesek[2]);
		//printf("\n");
		
		/*char tmp[120];
		char kerdesekAtalakitott[15][120];
		int i=0;
		for(;i<15;i++){
			memcpy(kerdesekAtalakitott[i], &kapottKerdesek[i/5][(i%5)*120],120);
			kerdesekAtalakitott[i][119]='\0';
		}*/
		
		int i;
		
		int valaszokSzama[3]={0,0,0};
		for(i=0;i<15;i++){
			if(i!=0 && i!=5 && i!=10){
				if(kapottKerdesek[i][0]!='\n' && kapottKerdesek[i][1]!='\n' && kapottKerdesek[i][2]!='\n'){
					valaszokSzama[i/5] += 1; 
				}
			}
		}
		
		//printf("valaszokszama: %d %d %d\n",valaszokSzama[0],valaszokSzama[1],valaszokSzama[2]);
		
		char nevek[20][60];
		for(i=0;i<20;i++){
			fgets(nevek[i],60,f);
		}
		fclose(f);
		
		int emberSzama[20]={21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21};
		int hanyember=(rand()%11)+10;
		
		         //hanyember=20;
		
		printf("\n%d ember valaszol a kerdesekre:\n",hanyember);
		int voltEMar;
		int j;
		for(i=0;i<hanyember;){
			voltEMar=0;
			int random=rand()%20;
			//printf("%d\n",random);
			for(j=0;j<20;j++){
				if(emberSzama[j]==random){
					voltEMar=1;
				}
			}
			if(voltEMar!=1){
				emberSzama[i]=random;
				i++;
			}
			
		}
		
		//printf("Szamok: %d %d %d %d %d %d % d %d %d %d %d \n",emberSzama[0],emberSzama[1],emberSzama[2],emberSzama[3],emberSzama[4]
		//,emberSzama[5],emberSzama[6],emberSzama[7],emberSzama[8],emberSzama[9],emberSzama[10]);
		
		int hanyasValasz;
		char hanytmp[10];
		sprintf(hanytmp, "%d", hanyember);
		sleep(2);
		kill(getppid(), SIGUSR1);
		write(pipefd[1], hanytmp, 10);
		
		for(i=0;i<hanyember;i++){
			//hanyasValasz=rand()%valaszokSzama[0];
			write(pipefd[1],nevek[emberSzama[i]],60);
			write(pipefd[1],kapottKerdesek[(rand()%valaszokSzama[0])+1],120);
			write(pipefd[1],kapottKerdesek[(rand()%valaszokSzama[1])+6],120);
			write(pipefd[1],kapottKerdesek[(rand()%valaszokSzama[2])+11],120);
			
		}
		
		
		close(pipefd[0]);
		close(pipefd[1]);		// finally we close the used read end
		fflush(NULL);
		printf("Gyerek befejezte!\n");
		/*char subbuff[5];
		memcpy( subbuff, &buff[10], 4 );
		subbuff[4] = '\0';*/
		
		//notacommonvalue=5; //it changes the value of the copy of the variable
		//printf("The value is %i in child process \n",notacommonvalue);
	}
	return 0;
	
	
}