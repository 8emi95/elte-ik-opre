#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/wait.h> 
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <sys/msg.h> 

#define MAX_SIZE 512

void menu();
void vlistazas();
void jelentkezes();
void vmodositas();
void vtorles();
void ujrend();
void rtorles();
void copy_file();
void rendezvenyek();

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char* mtext; 
}; 

int kuld( int uzenetsor, pid_t pid , char* mytext) 
{    

     const struct uzenet uz = { pid,  mytext }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, sizeof(uz.mtext), 0 ); 
     if ( status < 0 ) {
          perror("msgsnd"); 
		  printf("valami");
	 }
     return 0; 
} 
int fogad( int uzenetsor ) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
	
	
	 pid_t pid = getpid();
     status = msgrcv(uzenetsor, &uz, 1024, pid, 0 ); 
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "Az esemeny sikeressege:  %s\n",  uz.mtext ); 
     return 0; 
} 



void handler(int signumber)
{
	signal(SIGTERM,SIG_DFL);
	//pid_t my_id = getpid();
	//printf("Megerkeztem a rendezvenyre: (%i,%i).\n",my_id,signumber);
	
}

int main(int argc,char *argv[])
{   
      
     
      FILE* rendezveny;
	  FILE* vendeg;
	  FILE* p;
	  
	  rendezveny=fopen("rendezveny.txt","r");

	  
	  int rend=2;
	  
      char sor[MAX_SIZE];	  
	 
	  
	 
	  
	  
	  int pipefile;
	  pipefile= open("pipefile.txt",O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	  
	  
	  
	  p=fopen("pipefile.txt","a+");
	  vendeg = fopen("vendegek.txt","r");
	    
	   while(fgets(sor,sizeof(sor),vendeg)!=NULL)
       {
        char name[MAX_SIZE];
        char email[MAX_SIZE];

        int numb;
        int id;
        sscanf(sor,"%d %s %s %d",&id,name,email,&numb);
		if(numb==rend) {
			 fputs(sor,p);
		}	  
     }
	 
	 
	 fclose(p);
	 fclose(vendeg);
	  
	  mkfifo("pipefile.txt", S_IRUSR | S_IWUSR );
	  
	
	  int uzenetsor, status; 
 	  int pipefd[2]; 
      pid_t child;
      char sz[100];
	  int kapott;
	  key_t kulcs;
	  kulcs = ftok(argv[0],1); 
      uzenetsor = msgget( kulcs, 0600 | IPC_CREAT ); 
	  
	  
	  signal(SIGTERM,handler);
	 if (pipe(pipefd) == -1) 
	   {
               perror("Hiba a pipe nyitaskor!");
               exit(EXIT_FAILURE);
           }
	
	
	      child = fork();
		   
		   if(child>0)
		   {
			   
			   int status;
			   close(pipefd[0]); 
			   write(pipefd[1], &rend,sizeof(rend));
			   close(pipefd[1]);
			   pause();
			   
			   printf("Elküldtem a vendeglistat!\n");
			   kill(child,SIGTERM);
			   
			   wait(&status);
			   
			   
			   
			   
			   fogad(uzenetsor);
           
              
			    
			   
		   }
	       else
		   {   
			  close(pipefd[1]);
			  read(pipefd[0],&kapott,sizeof(kapott));
			  printf("A következo szamu rendezvenyt kell lebonyolitani: %d",kapott);
	          printf("\n");
			  printf("Helyszinre utazik!\n");
			  printf("Itt vagyok!\n");
			  kill(getppid(),SIGTERM);
			  pause();
			  
			   printf("A vendegek:\n");
			   p=fopen("pipefile.txt","r");
			   while(fgets(sor,sizeof(sor),p)!=NULL)
               {
			       printf("%s\n",sor);
				   
			   }
               fclose(p);
			   
			   
			    printf("Akik nem jelentek meg:\n");
			     p=fopen("pipefile.txt","r");
			     int a = 0;
			   while(fgets(sor,sizeof(sor),p)!=NULL)
               {
				   a++;
				   srand(time(NULL) + a);
			       int r2 = rand() % 10;
				   
			       if(r2 == 1)
			      {
				     printf("%s", sor);
			      }   
			   }
               fclose(p);
			   unlink("pipefile.txt");		
			  
			  
			  
			  char uzi[MAX_SIZE]="\0";
			  srand(time(NULL) + a);
			  int r = rand() % 100;
			  sprintf(uzi, "%d", r);
			  pid_t parent = getppid();
			  
			  kuld(uzenetsor,parent,"3");
			
			  	   
		   } 
    //menu();
    return 0;
}



























void menu()
{
    char menu[MAX_SIZE];
    while(1 < 2)
    {


        printf("\nMenu eleresehez nyomd meg az 'm'-t!!\t");
        scanf("%s",menu);
        if( strcmp(menu,"m") == 0 )
        {

            printf("Vendég Listázása:\tl\n");
            printf("Jelentkezes:\tj\n");
            printf("Vendeg Modosítas:\ts\n");
            printf("Vendég Törlés:\tt\n");
            printf("Uj Rendezveny:\tu\n");
            printf("Rendezveny Törlés:\td\n");
            printf("KILEPES:\tPress CTRL + C to exit\n");
        }
        if( strcmp(menu,"l") == 0 )
        {
            vlistazas();
        }
        else if( strcmp(menu,"j") == 0 )
        {
            jelentkezes();
        }
        else if( strcmp(menu,"s") == 0 )
        {
            vmodositas();
        }
        else if( strcmp(menu,"t") == 0 )
        {
            vtorles();
        }
        else if( strcmp(menu,"u") == 0 )
        {
            ujrend();
        }
        else if( strcmp(menu,"d") == 0 )
        {
            rtorles();
        }
    }
}

void vlistazas()
{

    FILE *vendegek;

    vendegek = fopen("vendegek.txt","r");
    char kiir[MAX_SIZE];
    printf("\nVendegek:\n");

    while(fgets(kiir,100,vendegek) != NULL)
    {
        printf("%s",kiir);
    };

    fclose(vendegek);


}
void jelentkezes()
{
    FILE *vendegek;

    vendegek = fopen("vendegek.txt","a+");


    char nev[MAX_SIZE];
    char mail[MAX_SIZE];
    int id=1;
    int newId;
    int num;
    time_t t;
    t=time(NULL);

    char sor[MAX_SIZE];

    printf("\nKerem a nevet:\t");
    scanf("%s",nev);

    printf("\nKerem az e-mail cimet:\t");
    scanf("%s",mail);

    printf("\nKerem a rendezveny szamat:\t");
    scanf("%d",&num);


    if(fgets(sor,sizeof(sor),vendegek)==NULL)
    {
        fprintf(vendegek,"%d %s %s  %d %s",id,nev,mail,num,ctime(&t));
    }
    else
    {
        while(fgets(sor,sizeof(sor),vendegek)!=NULL)
        {
            sscanf(sor,"%d", &id);
        }
        id++;
        fprintf(vendegek,"\n%d %s %s  %d %s",id,nev,mail,num,ctime(&t));

    }


    printf("On %d.-kent jelentkezett",id);
    fclose(vendegek);




}
void vmodositas()
{
    char nev[MAX_SIZE];
    printf("Kerem a modositando nevet:");
    scanf("%s",nev);
    time_t t;
    t=time(NULL);

    FILE *vendegek;
    FILE *plus;
    vendegek=fopen("vendegek.txt","r");
    plus=fopen("plus.txt","w");
    char sor[MAX_SIZE];
    while(fgets(sor,sizeof(sor),vendegek)!=NULL)
    {
        char name[MAX_SIZE];
        char email[MAX_SIZE];
        int numb;
        int id;
        sscanf(sor,"%d %s %s %d",&id,name,email,&numb);
        char nnev[MAX_SIZE];
        char nmail[MAX_SIZE];
        int nnum;

        if(strcmp(name,nev)==0)
        {
            char menu[MAX_SIZE];
            printf("Mit modosit?\n");
            printf("Nev (n)\n");
            printf("E-mail (e)\n");
            printf("Rendezveny (r)\n");
            scanf("%s",menu);

            if(strcmp(menu,"n")==0)
            {
                printf("KErem az uj nevet:");
                scanf("%s",nnev);
                fprintf(plus,"%d %s %s %d %s\n",id,nnev,email,numb,ctime(&t));


            }
            else if(strcmp(menu,"e")==0)
            {
                printf("Kerem az uj mailt:");
                scanf("%s",nnev);
                fprintf(plus,"%d %s %s %d %s\n",id,name,nmail,numb,ctime(&t));


            }
            else if(strcmp(menu,"r")==0)
            {
                printf("KErem az uj rendezvenyt:");
                scanf("%s",nnev);
                fprintf(plus,"%d %s %s %d %s\n",id,name,email,nnum,ctime(&t));


            }
            else
            {

                vmodositas();
            }


        }
        else
        {
            fputs(sor,plus);
        }

    }
    fclose(vendegek);
    fclose(plus);
    copy_file("plus.txt","vendegek.txt") ;
}

void vtorles()
{
    char nev[MAX_SIZE];
    printf("Kerem a torlendo szemely nevet:");
    scanf("%s",nev);

    FILE *vendegek;
    FILE *plus;

    vendegek=fopen("vendegek.txt","r");
    plus=fopen("plus.txt","w");

    char sor[MAX_SIZE];

    while(fgets(sor,sizeof(sor),vendegek)!=NULL)
    {
        int id;
        char name[MAX_SIZE];
        sscanf(sor,"%d %s ",&id,name);

        if(strcmp(name,nev) != 0)
        {
            fputs(sor,plus);
        }
    }
    fclose(vendegek);
    fclose(plus);

    copy_file("plus.txt","vendegek.txt");

}
void copy_file(const char *from, const char *to)
{
    FILE *fr;
    FILE *t;
    fr = fopen(from,"r");
    t = fopen(to,"w");

    char sor[MAX_SIZE];
    int rid;
    int id=1;
    char nev[MAX_SIZE];
    char mail[MAX_SIZE];
    int num;
    char nap[MAX_SIZE];
    char ho[MAX_SIZE];
    int date;
    char ido[MAX_SIZE];
    int ev;

    while(fgets(sor,sizeof(sor),fr) != NULL)
    {
        sscanf(sor,"%d %s %s %d %s %s %d %s %d",&rid,nev,mail,&num,nap,ho,&date,ido,&ev);
        fprintf(t,"%d %s %s %d %s %s %d %s %d\n",id,nev,mail,num,nap,ho,date,ido,ev);
        id++;
    }
    fclose(fr);
    fclose(t);

    remove("plus.txt");
}


void ujrend()
{
    FILE *rend;

    rend = fopen("rendezveny.txt","a+");

    int id;
    char nev[MAX_SIZE];

    int ok = 1;
    printf("\nKerem a rendezveny azonosítot:");
    scanf("%d",&id);
    char sor[MAX_SIZE];

    if(fgets(sor,sizeof(sor),rend) == NULL)
    {

        printf("\nRendezveny neve:");
        scanf("%s",nev);
        fprintf(rend,"%d %s",id,nev);

    }
    else
    {
        while(fgets(sor,sizeof(sor),rend) != NULL)
        {
            int regid;
            sscanf(sor,"%d",&regid);
            if(regid==id)
            {
                ok=0;
            }
        }
        if(ok==0)
        {
            printf("Ilyen azonosito mar van");
            ujrend();
        }
        else
        {
            printf("\nRendezveny neve:");
            scanf("%s",nev);
            fprintf(rend,"\n%d %s",id,nev);
        }
    }
    fclose(rend);
}
void rtorles()
{ 
    int id;
    printf("Kerem a torlendo rendezveny azonositot:");
    scanf("%d",&id);

    FILE *rend;
    FILE *plus;

    rend=fopen("rendezveny.txt","r");
    plus=fopen("plus.txt","w");

    char sor[MAX_SIZE];

    while(fgets(sor,sizeof(sor),rend)!=NULL)
    {
        int iid;
        sscanf(sor,"%d",&iid);

        if(iid==id)
        {
            fputs(sor,plus);
        }
    }
	fclose(rend);
	fclose(plus);
	
	rend=fopen("rendezveny.txt","w");
	plus = fopen("plus.txt","r");
	
	int rid;
	char nev[MAX_SIZE];
	
     while(fgets(sor,sizeof(sor),plus) != NULL){
		 sscanf(sor,"%d" "%s",&rid,nev);
		 fprintf(rend,"%d %s",rid,nev)	;	
	 }
	 fclose(plus);
	 fclose(rend);
	 remove("plus.txt");

}
