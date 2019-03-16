#include<stdio.h>
#include<stdlib.h>
#include <time.h>
  
void listVisitors(){
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
    int i = 1;
	int visitors = visitorNumber();
    while(fgets(line, sizeof(line), fp) != NULL){
        sscanf(line, "%s",id);
        printf("%d. %s\tNumber of visitors: %d\n", i,id,visitors);  
        i++;    
    }

    fclose(fp);
    return; 
}

int newVisitor(){
    FILE *f = fopen("visitor.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return 0;
    }
    
    char name[100];
    char email[100];
    char id[20];
	
    printf("\nName?\n");
    fgets (name, 100, stdin);
    strtok(name, "\n");
    
    printf("Email?\n");
    fgets (email, 100, stdin);
    strtok(email, "\n");
    
	printf("Exist codes with number of visitors:\n");
    listEvents();
	
    int exist=0;
	while(exist!=1){
		printf("Write an exist event code\n");
		fgets (id, 20, stdin);
		strtok(id, "\n");
		exist = eventExists(id);
		
	}
	
    
    time_t now = time(0);
    
    fprintf(f, "%s\t%s\t%s\t%s", name, email,id,ctime(&now));
    fclose(f);
	int visitors=visitorNumber();
	printf("You are %d. visitor.",visitors);
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
    printf("\nCode of event?\n");
	
	fgets (id, 20, stdin);
    strtok(id, "\n");
    int exist=eventExists(id);
	while(exist==1){
		printf("This code is exist, plead enter an other one!\n");
		fgets (id, 20, stdin);
		strtok(id, "\n");
		exist = eventExists(id);
		
	}
	
    
    
    fprintf(f, "%s\n",id);
    fclose(f);
    
    return 0;

}

void modifyVisitor(){
    int number=visitorNumber();
	if(number==0){
		printf("\n\t Nincs vendeg\n");
		return;
		
	}
	int order =1;
    listVisitors();
    printf("\n\t Melyik sorszamu vendeget szeretned modositani?\n");
    scanf(" %d", &order);
	while(order<0 || order>number){
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

void removeVisitor(){
	int number=visitorNumber();
	if(number==0){
		printf("\n\t Nincs vendeg\n");
		return;
		
	}
    int order =1;
    listVisitors();
    printf("\n\t Melyik sorszamu vendeget szeretned torolni?\n");
    scanf(" %d", &order);
	while(order<0 || order>number){
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
	while(order<0 || order>number){
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
    char date[100];
    int i =1;
    
	while (fgets(line, sizeof(line), input2))
    {
        
        if (i==order)
        {
            sscanf(line, "%s",eventId); 
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
	/*read line until end of file*/
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

int main ()
{   
    int choice=0;
    while(choice!='8')
    {
        printf("\n\n\t 1. New event");
        
		//list event menu is available if we have event
		if (eventNumber() != 0) {
			printf("\n\t 2. New visitors");
			printf("\n\t 3. List events");
			
			//list visitor menu is available if we have visitor and event
			if (visitorNumber()!=0) {
				printf("\n\t 4. List visitors");
			}
		}
		
		if (visitorNumber()!=0) {
				printf("\n\t 5. Modify visitor");
				printf("\n\t 6. Remove visitor");
		}
        
		if (eventNumber() != 0) {
			printf("\n\t 7. Remove event");
		}
		
        printf("\n\t 8. Exit");
        printf("\n\n Enter your choice: ");
        scanf(" %c%*c", &choice);
			switch(choice)
			{
			case '1':
				newEvent();
				break;
			case '2':
				newVisitor();
				break;
			case '3':
				listEvents();
				break;
			case '4':
				listVisitors();
				break;			
			case '5':
				modifyVisitor();
				break;
			case '6':
				removeVisitor();
				break;
			case '7':
				deleteEvent();
				break; 
			case '8':
				break;			
			default:
				printf("\n\nPlease try again\n");
			}
    }
  return 0;
}





