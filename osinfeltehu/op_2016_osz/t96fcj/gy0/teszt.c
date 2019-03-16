#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h> //opendir, readdir
#include <sys/types.h> //opendir
#include <sys/stat.h> //stat
#include <unistd.h>   //stat

char *removedot(char* mystr) {
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    return retstr;
}

int is_ExistEvent(const char * Event_Name)
{
	DIR* d;
	struct dirent * dp;
	int exist=0;
	d = opendir("events/");
	for(;dp=readdir(d);)
	{
		
		char *substring;
		substring = removedot(dp->d_name);
		
		
		
		if(strcmp(substring,Event_Name)==0)
		{
			
			exist=1;
			break;
		}
		
	}
	return exist;
	
	
	
}

/*void aloha() {
    

   
}*/



void Creat_Event()
{
	
    
	char *buffer;
    
    
    printf("Please Type the Event Name: ");
	buffer= malloc(32*sizeof(char));
	scanf("%s",buffer);
	
	
	while(is_ExistEvent(buffer))
	{
		
		printf("The Event is already exist, please Choose an other name: ");
		free(buffer);
		buffer= malloc(32*sizeof(char));
		scanf("%s",buffer);
	}
	
	
	char * file_name ;
	if((file_name = malloc(40)) != NULL){
    file_name[0] = '\0'; 
    strcat(file_name,"events/");
    strcat(file_name,buffer);
	strcat(file_name,".event");
	} else {
			printf("Something went wrong during creating the file name\n");
			
	}

	
	FILE *fp;
	fp=fopen(file_name,"w");
	fclose(fp);
	free(file_name);
	printf("The event has been created! \n \n");
	

	
	
}


void List_Events()
{
	DIR* d;
	struct dirent * dp; //pointer to dirent struct
	//struct stat st;
	d = opendir("events/");
	printf("The Current Events: \n\n");
	int i;
	
	for(i=1;dp=readdir(d);)
	{
		if (!strcmp (dp->d_name, "."))
            continue;
        if (!strcmp (dp->d_name, ".."))    
            continue;
		
			char * substring;
 			substring = removedot(dp->d_name);
			printf("%i. : \t%s \n",i,substring);
			i++;
		}
		closedir(d);
}

void Registration()
{
	printf("Kérem adja meg, hogy melyik eseményre kíván regisztrálni:");
	char *event_name=malloc(32*sizeof(char));
	scanf("%s",event_name);
	if(!(is_ExistEvent(event_name)))
	{
		printf("Nem létezik ilyen nevű esemény!");
		return;
	}
	
	char * file_name;
	
	file_name = malloc(40);
    file_name[0] = '\0'; 
    strcat(file_name,"events/");
    strcat(file_name,event_name);
	strcat(file_name,".event");
	
	FILE *fp;
	fp=fopen(file_name,"a");
	
	printf("Kérem adja meg a vezeték nevét: ");
	printf("Kérem adja meg a keresztnevét: ");
	printf("Kérem adja meg a az email címét: ");
	printf("Kérem adja meg, hogy melyik eseményre kíván jelentkezni");
	
	
	
	
	free(file_name);
	
	
	
	
	
	
	
	fclose(fp);
	
	
	
		
	
	
	
}

	
	
	



int main()
{
	printf("lol");
	List_Events();
	Registration();
	
   return(0);
}
	
