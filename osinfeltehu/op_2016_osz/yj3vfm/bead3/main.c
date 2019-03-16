#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 
#include <signal.h>
#include <string.h>
 
#define MAX_VISITOR 100
 
void listApplicants(){
    int number=visitorNumber();
	if(number==0){
		printf("\n\t Nincs vendeg\n");
		return;
		
	}
    FILE *fp = fopen("visitor.txt","r");
    char line[250];
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    char name[100];
    char email[100];
    char id[50];
    char date[100];
    int i = 1;
    while(fgets(line, sizeof(line), fp) != NULL){
        sscanf(line, "%[^\t]\t%s\t%s\t%[^\n]",name,email,id,date);
        printf("%d. %s\t%s\t%s\t%s\n", i,name,email,id,date);  
        i++;    
    }

    fclose(fp);
    return; 
}

void listEvents(){
    
    FILE *fp = fopen("events.txt","r");
    char line[250];
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    char id[50];
	char hely[200];
    int i = 1;
    while(fgets(line, sizeof(line), fp) != NULL){
        sscanf(line, "%s\t%s\n",id,hely);
        printf("%d. %s\t%s\n", i,id,hely);  
        i++;    
    }

    fclose(fp);
    return; 
}
 

int newApplication(){
	if(eventNumber()==0){
		printf("Nincs hozzadadoot rendezvény!\n");
		return 0;
		
	}
    FILE *f = fopen("visitor.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return 0;
    }
    
    char name[100];
    char email[100];
    char id[20];
    printf("\nNeve?\n");
    fgets (name, 100, stdin);
    strtok(name, "\n");
    
    printf("Email?\n");
    fgets (email, 100, stdin);
    strtok(email, "\n");
    
    
    int exist=0;
	while(exist!=1){
		printf("Adjon meg egy letezo rendezveny azonositot\n");
		fgets (id, 20, stdin);
		strtok(id, "\n");
		exist = eventExists(id);
		
	}
	
    
    time_t now = time(0);
    
    fprintf(f, "%s\t%s\t%s\t%s", name, email,id,ctime(&now));
    fclose(f);
	int visitors=visitorNumber();
	printf("On %d.-nak regisztralt",visitors);
    return visitors;

}

int newEvent(){
    FILE *f = fopen("events.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }
    
    char id[20];
    printf("\nRendezveny azonositoja?\n");
	
	fgets (id, 20, stdin);
    strtok(id, "\n");
    int exist=eventExists(id);
	while(exist==1){
		printf("Van mar ilyen azonositoju rendezveny, adjon meg egy masik azonositot\n");
		fgets (id, 20, stdin);
		strtok(id, "\n");
		exist = eventExists(id);
		
	}
	char hely[200];
	printf("\nRendezveny helyszine?\n");
	
	fgets (hely, 200, stdin);
    strtok(hely, "\n");
    
    
    fprintf(f, "%s\t%s\n",id,hely);
    fclose(f);
    
    return 0;

}

void editApplication(){
    int number=visitorNumber();
	if(number==0){
		printf("\n\t Nincs vendeg\n");
		return;
		
	}
	int order =1;
    listApplicants();
    printf("\n\t Melyik sorszamu vendeget szeretned modositani?\n");
    scanf(" %d", &order);
	while(order<=0 || order>number){
		printf("\n\t Adj meg egy ervenyes sorszamot\n");
		scanf(" %d", &order);
		
	}
    FILE *input  = fopen("visitor.txt", "r");
    if (input == NULL)
    {
        printf("No applications\n");
        return;
    }
    
    FILE *output = fopen("temp.txt", "w");

    char line[350];
    char name[100];
    char email[100];
    char id[50];
    char date[100];
    int i =1;
	int exist=0;
    
    while (fgets(line, sizeof(line), input))
    {
        
        if (i==order)
        {
            sscanf(line, "%[^\t]\t%s\t%s\t%[^\n]",name,email,id,date);
            int choice=0;
            while(choice!='4')
            {
                printf("\n\n\t Mit szeretne modositani?");
                printf("\n\n\t 1. Nev");
                printf("\n\t 2. Email");
                printf("\n\t 3. Rendezveny azonosito");
                printf("\n\t 4. Kesz\n");
                scanf(" %c%*c", &choice);
                switch(choice)
                {
                case '1':
                    printf("\n\n\t Uj nev:");
                    fgets (name, 100, stdin);
                    strtok(name, "\n");
                    break;
                case '2':
                    printf("\n\n\t Uj email:");
                    fgets (email, 100, stdin);
                    strtok(email, "\n");
                    break;
                case '3':               
					while(exist!=1){
						printf("Adjon meg egy letezo rendezveny azonositot\n");
						fgets (id, 20, stdin);
						strtok(id, "\n");
						exist = eventExists(id);
						
					}
                    break;
                case '4':
                  
                    break;
                default:
                    printf("\n\nINVALID SELECTION...Please try again\n");
                }
            }
            
            fprintf(output, "%s\t%s\t%s\t%s\n", name, email,id,date);
            
        }
        else{
            fputs(line, output);   
        }
        i++;    
    }

    fclose(output);
    fclose(input);
    
    remove("visitor.txt");
    rename("temp.txt", "visitor.txt");
    
    
}

void deleteApplication(){
	int number=visitorNumber();
	if(number==0){
		printf("\n\t Nincs vendeg\n");
		return;
		
	}
    int order =1;
    listApplicants();
    printf("\n\t Melyik sorszamu vendeget szeretned torolni?\n");
    scanf(" %d", &order);
	while(order<=0 || order>number){
		printf("\n\t Adj meg egy ervenyes sorszamot\n");
		scanf(" %d", &order);
		
	}
    FILE *input  = fopen("visitor.txt", "r");
    if (input == NULL)
    {
        printf("No applications\n");
        return;
    }
    
    FILE *output = fopen("temp.txt", "w");

    char line[350];
    char name[100];
    char email[100];
    char id[50];
    char date[100];
    int i =1;
    
    while (fgets(line, sizeof(line), input))
    {
        
        if (i==order)
        {
   
        }
        else{
            fputs(line, output);   
        }
        i++;    
    }

    fclose(output);
    fclose(input);
    
    remove("visitor.txt");
    rename("temp.txt", "visitor.txt");
    
    
}


void deleteEvent(){
    int number=eventNumber();
	if(number==0){
		printf("\n\t Nincs hozzadaott esemeny\n");
		return;
		
	}
	listEvents();
	int order;
    printf("\n\t Melyik sorszamu esemenyt szeretned torolni?\n");
    scanf(" %d", &order);
	while(order<=0 || order>number){
		printf("\n\t Adj meg egy ervenyes sorszamot\n");
		scanf(" %d", &order);
		
	}
    FILE *input2  = fopen("events.txt", "r");
	FILE *input  = fopen("visitor.txt", "r");
    if (input == NULL)
    {
        printf("No events\n");
        return;
    }
    
    FILE *output = fopen("temp.txt", "w");
	FILE *output2 = fopen("temp2.txt", "w");

    char line[350];
    char name[100];
    char email[100];
    char id[50];
	char eventId[50];
	char hely[200];
    char date[100];
    int i =1;
    
	while (fgets(line, sizeof(line), input2))
    {
        
        if (i==order)
        {
            sscanf(line, "%s\t%s\n",eventId,hely); 
        }else{
            fputs(line, output2);   
        }
        i++;    
    }
		
    while (fgets(line, sizeof(line), input))
    {
        
        sscanf(line, "%[^\t]\t%s\t%s\t%[^\n]",name,email,id,date);
		if (strcmp(id,eventId) == 0)
        {
            
        }else{
            fputs(line, output);   
        } 
    }

    fclose(output);
    fclose(input);
	fclose(output2);
    fclose(input2);
    
    remove("visitor.txt");
	remove("events.txt");
    rename("temp.txt", "visitor.txt");
	rename("temp2.txt", "events.txt");
	
	
}

int visitorNumber(){
	FILE *fp = fopen("visitor.txt","r");
    int ch=0;    
    int visitors=0;
    if (fp != NULL)
    {
        while(!feof(fp))
        {
            ch = fgetc(fp);
            if(ch == '\n')
            {
                visitors++;
            }
        } 
        fclose(fp);        
    }
	
	return visitors;
	
}

int eventNumber(){
	FILE *fp = fopen("events.txt","r");
    int ch=0;    
    int event=0;
    if (fp != NULL)
    {
        while(!feof(fp))
        {
            ch = fgetc(fp);
            if(ch == '\n')
            {
                event++;
            }
        } 
        fclose(fp);        
    }
	
	return event;
	
}

int eventExists(char *event){
	char eventId[50];
	char line[350];
	FILE *input2  = fopen("events.txt", "r");
	while (fgets(line, sizeof(line), input2))
    {  
            sscanf(line, "%s",eventId); 
			if (strcmp(event,eventId) == 0) return 1;
    }
	return 0;
	
}

void handler(int signumber){
  
}

int beolvas(char tomb[][100],char* idEvent){
    FILE *fp = fopen("visitor.txt","r");
    char line[250];
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    char name[100];
    char email[100];
    char id[50];
    char date[100];
    int i = 1;
    int j =0;
  
    while(fgets(line, sizeof(line), fp) != NULL){
        sscanf(line, "%[^\t]\t%s\t%s\t%[^\n]",name,email,id,date);
        if(strcmp(id,idEvent)==0){
            strcpy(tomb[j],name);
            j++; 
        }
               
        i++;    
    }

    fclose(fp);
    return j; 
    
}

void levezenyel(){
	
	FILE *fp = fopen("events.txt","r");
    char line[250];
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    char id[50];
	char hely[200];
    int i = 1;
    char tomb[MAX_VISITOR][100];   
    int k;
    int result;
    char no_tomb[MAX_VISITOR][100];
    int r;
    int no_number;
    int choice2;
	
    
    signal(SIGUSR1,handler);
	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;
	char sz1[200];  // char array for reading from pipe
	int number;
    
	if (pipe(pipefd) == -1) 
		{
		   perror("Hiba a pipe nyitaskor!");
		   exit(EXIT_FAILURE);
		}
		
	while(fgets(line, sizeof(line), fp) != NULL){
        srand ( time(NULL) );
		printf("Vezenyeljuk le a kovetkezo esemenyt?(igen-y,nem-n):\n");
        scanf(" %c%*c", &choice2);
        if(choice2=='n') break;
	    pid = fork();	// creating parent-child processes
	    if (pid == -1) 
	    {
		   perror("Fork hiba");
		   exit(EXIT_FAILURE);
	    }

	    if (pid == 0) 
			{
			pause();
			//close(pipefd[1]);  //Usually we close the unused write end
			read(pipefd[0],sz1,sizeof(sz1)); // reading max 100 chars
			printf("Helyszint megkaptam:%s\n",sz1);
			//close(pipefd[0]); // finally we close the used read end
			printf("Ottvagyok,varom a resztvevoket.\n");
			kill(getppid(),SIGUSR1);
            pause();
            read(pipefd[0],&number,sizeof(number));
             // reading max 100 chars
            printf("Resztvevoket megkaptam:\n");
            no_number=0;
            for(k=0; k<number; k++){
               read(pipefd[0],tomb[k],sizeof(tomb[k]));
               printf("%s\t",tomb[k]);
               r = rand()%100;
               if(r<=10){
                  stpcpy(no_tomb[no_number],tomb[k]);
                  no_number++;      
               }
               
            }
            printf("\n");
            r = rand()%100+1;
            write(pipefd[1], &r,sizeof(r));
            write(pipefd[1], &no_number,sizeof(no_number));
            for(k=0; k<no_number; k++){
            write(pipefd[1], no_tomb[k],sizeof(no_tomb[k]));
            }
            kill(getppid(),SIGUSR1);
			exit(0);
	    } 
	    else 
	    {   

			sleep(1);		
			sscanf(line, "%s\t%s\n",id,hely);                 
			//close(pipefd[0]); //Usually we close unused read end
			write(pipefd[1], hely,sizeof(hely));
			//close(pipefd[1]); // Closing write descriptor 
			printf("Esemeny helyszine elkuldve!\n");
			fflush(NULL); 	// flushes all write buffers (not necessary)		
			kill(pid,SIGUSR1);
			pause();
            sleep(1);
            number = beolvas(tomb,id);
            write(pipefd[1], &number,sizeof(number));
            for(k=0; k<number; k++){
                write(pipefd[1], tomb[k],sizeof(tomb[k]));
            }
			
            fflush(NULL);
            printf("Resztvevok elkuldve.\n");
            kill(pid,SIGUSR1);
            pause();
            read(pipefd[0], &r,sizeof(r));
            read(pipefd[0], &no_number,sizeof(no_number));
            
            printf("Eredmenyt megkaptam:%d.\n",r);
            printf("Nemerkeztek meg:\n");
            for(k=0; k<no_number; k++){
               read(pipefd[0], no_tomb[k],sizeof(no_tomb[k]));
               printf("%s\t",no_tomb[k]);
               
            }  
            printf("\n");            
	    }
        
	}  

    fclose(fp);
}



int main ()
{   
    
    srand ( time(NULL) );
    int choice=0;
    while(choice!='9')
    {
        printf("\n\n\t 1. Uj jelentkezes");
        printf("\n\t 2. Vendegek listazasa");
        printf("\n\t 3. Uj rendezveny");
        printf("\n\t 4. Rendezvények listazasa");
        printf("\n\t 5. Vendeg adatainak modositasa");
		printf("\n\t 6. Vendeg törlése");
		printf("\n\t 7. Rendezvény törlése");
		printf("\n\t 8. Rendezvények levezenylese");
        printf("\n\t 9. Kilep");
        printf("\n\n Enter Your Choice: ");
        scanf(" %c%*c", &choice);
        switch(choice)
        {
        case '1':
            newApplication();
            break;
        case '2':
            listApplicants();
            break;
        case '3':
            newEvent();
            break;
        case '4':
            listEvents();
            break;			
        case '5':
           editApplication();
            break;
        case '6':
			deleteApplication();
			break;
        case '7':
			deleteEvent();
            break; 
		case '8':
			levezenyel();
            break; 			
		case '9':
            break;			
        default:
            printf("\n\nINVALID SELECTION...Please try again\n");
        }
    }
  return 0;
}





