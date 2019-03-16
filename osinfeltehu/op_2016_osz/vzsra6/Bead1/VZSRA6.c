#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_SIZE 512

void menu();
int sorszam();
void listazas();
void jelentkezes();
void modositas();
void torles();
void addRednezveny();
void listRednezveny();
void deleteRendezveny();
void copy_file();
void copy_file_rendezveny();

int main(int argc,char *argv[])
{
    menu();
    
	return 0;
}

void menu()
{
	char menu[MAX_SIZE];
        char rendezveny[MAX_SIZE];
	while(1 < 2)
	{
          
                printf("\nAdja meg a parancsot: (help: h)\t");
		scanf("%s",menu);
                
		if( strcmp(menu,"h") == 0 )
		{
                    	printf("Vendeg:\n");
                        printf("+ LISTAZAS:\tl\n");
			printf("+ JELENTKEZES:\tj\n");
			printf("+ MODOSITAS:\tm\n");
			printf("+ TORLES:\tt\n");
			printf("\n");
                        printf("RENDEZVENY:\n");
                        printf("+ LETREHOZAS:\ta\n");
                        printf("+ LISTAZAS:\tr\n");
                        printf("+ TORLES:\td\n");
			printf("KILEPES:\tPress CTRL + C to exit\n");
		}
		if( strcmp(menu,"l") == 0 )
		{
			listazas();
		}
		else if( strcmp(menu,"j") == 0 )
		{
			jelentkezes();
		}
		else if( strcmp(menu,"m") == 0 )
		{
			modositas();
		}
		else if( strcmp(menu,"t") == 0 )
		{
			torles();
		}
                else if( strcmp(menu,"a") == 0 )
		{
			addRednezveny();
		}else if( strcmp(menu,"r") == 0 )
		{
			listRednezveny();
		}else if( strcmp(menu,"d") == 0 )
		{
			deleteRendezveny();
		}
                
                
        }	
}

void listazas()
{
	FILE *vendeg;
	if(fopen("vendegek.txt","r")){
	vendeg = fopen("vendegek.txt","r");
	
	char kiir[MAX_SIZE];
	printf("\nVENDEGEK ADATAI:\n");
	while(fgets(kiir,100,vendeg) != NULL)
	{   
		printf("%s",kiir);
	};
	printf("\n");
	fclose(vendeg);
        }
}

int sorszam(){
    
    FILE *vendegek;
    int sorszam = 0;
   
    if(fopen("vendegek.txt","r")){
        vendegek = fopen("vendegek.txt","r");
        char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),vendegek) != NULL)
	{   
                int id;
		sscanf(line,"%d",&id);
               
                if( id > sorszam )
		{   
                   
                    sorszam = id;
		}
	}
        fclose(vendegek);
        
    }
   
    sorszam++;
    return sorszam;
}

void jelentkezes()
{
	FILE *vendegek;
	vendegek = fopen("vendegek.txt","a+");
	
        int id;
	char nev[MAX_SIZE];
	char email[MAX_SIZE];
	int rendezvenyId; 
	
        id = sorszam();
        
	printf("Add meg a vendeg adatait:");
	printf("\nNev:\t");
	scanf("%s",nev);
	printf("\nEmail cim:\t");
	scanf("%s",email);
	printf("\nRendezveny azonosito szama:\t");
	scanf("%d",&rendezvenyId);
	
        time_t timer;
        char buffer[26];
        struct tm* tm_info;

        time(&timer);
        tm_info = localtime(&timer);

        strftime(buffer, 26, "%Y-%m-%d_%H:%M", tm_info);
        
        fprintf(vendegek,"%d %s %s %d %s\n",id,nev,email,rendezvenyId,buffer);
        fclose(vendegek);
        
        printf("A vendeg regisztracios sorszama: %d",id);
}

void torles()
{
	int sorszam;
	printf("Add meg a torlendo vendeg sorszamat:\t");
	scanf("%d",&sorszam);
	
	FILE *vendeg;
	FILE *helpfile;
        if(fopen("vendegek.txt","r")){
	vendeg = fopen("vendegek.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),vendeg) != NULL)
	{
		int id;
		sscanf(line,"%d",&id);
               
                if( id != sorszam )
		{   
                    fputs(line,helpfile);
		}
	}
	
	fclose(vendeg);
	fclose(helpfile);
	
	copy_file("helpfile.txt","vendegek.txt");
        }else{
            printf("A fajl nem letezik!");
        }
}



void modositas()		//M”DOSÕT¡S
{
	int sorszam;
	printf("Add meg a modositando vendeg sorszamat:\t");
	scanf("%d",&sorszam);
	
	FILE *vendegek;
	FILE *helpfile;
	vendegek = fopen("vendegek.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),vendegek) != NULL)
	{
		int id;
                char name[MAX_SIZE];
                char email[MAX_SIZE];
                int rid;
                char date[MAX_SIZE];
                sscanf(line, "%d %s %s %d %s", &id, name, email, &rid, date);
		
                //BekÈrt uj adatok
		char new_name[MAX_SIZE];
		char new_email[MAX_SIZE];
		int new_rid;
		
		if( sorszam == id)
		{
			printf("Add meg a vendeg uj adatait:");
                        printf("\nUj Nev:\t");
                        scanf("%s",new_name);
                        printf("\nUj Email cim:\t");
                        scanf("%s",new_email);
                        printf("\nUj Rendezveny azonosito szama:\t");
                        scanf("%d",&new_rid);
                        fprintf(helpfile,"%d %s %s %d %s\n",id,new_name,new_email,new_rid,date);
		}
		else
		{
			fputs(line,helpfile);
		}
	}
	
	fclose(vendegek);
	fclose(helpfile);
	
	copy_file("helpfile.txt","vendegek.txt");	
}
void addRednezveny(){

    FILE *rendezveny;
    rendezveny = fopen("rendezveny.txt","a+");
	
        int id;
	char nev[MAX_SIZE];
	
	
        
        
	printf("Add meg a rendezveny adatait:");
	printf("\nNev:\t");
	scanf("%s",nev);
	printf("\nRendezveny azonosito szama:\t");
	scanf("%d",&id);
	
        fprintf(rendezveny,"%s %d\n",nev,id);
        
        fclose(rendezveny);
        
        printf("Sikeresen letrehoztuk a rendezvenyt");
    
}
void listRednezveny()
{
	FILE *rendezveny;
	 if(fopen("rendezveny.txt","r")){
	rendezveny = fopen("rendezveny.txt","r");
	
	char kiir[MAX_SIZE];
	printf("\nRENDEZVENYEK ADATAI:\n");
	while(fgets(kiir,100,rendezveny) != NULL)
	{   
		printf("%s",kiir);
	};
	printf("\n");
	fclose(rendezveny);
         }else{
             printf("Nem talalhato rendezveny");
        }
}
void deleteRendezveny()
{
	int sorszam;
	printf("Add meg a torlendo rendezveny sorszamat:\t");
	scanf("%d",&sorszam);
	
	FILE *rendezveny;
	FILE *helpfile;
        if(fopen("rendezveny.txt","r")){
	rendezveny = fopen("rendezveny.txt","r");
	helpfile = fopen("helpfile.txt","w");
	char line[MAX_SIZE];
	
	while(fgets(line,sizeof(line),rendezveny) != NULL)
	{
		int id;
		char name[MAX_SIZE];
		sscanf(line,"%s %d",name, &id);
                
                
                
                if( id != sorszam )
		{   
                    printf("%s\n",line);
                    printf("%s\n",name);
                    printf("%d\n",id);
                    printf("%d\n",sorszam);
                    fprintf(helpfile,"%s %d\n",name,id);
		}
	}
	
	fclose(rendezveny);
	fclose(helpfile);
	
	copy_file_rendezveny("helpfile.txt","rendezveny.txt");
        
        FILE *vendeg;
        FILE *helpfile;
        if (fopen("vendegek.txt", "r")) {
            vendeg = fopen("vendegek.txt", "r");
            helpfile = fopen("helpfile.txt", "w");
            char line[MAX_SIZE];

            while (fgets(line, sizeof (line), vendeg) != NULL) {
                int vid;
                char vname[MAX_SIZE];
                char vemail[MAX_SIZE];
                int vrid;
                char vdate[MAX_SIZE];
                sscanf(line, "%d %s %s %d, %s", &vid, vname, vemail, &vrid, vdate);

                if (vrid != sorszam) {
                    fputs(line, helpfile);
                }
            }

            fclose(vendeg);
            fclose(helpfile);

            copy_file("helpfile.txt", "vendegek.txt");
        } else {
            printf("A fajl nem letezik!");
        }
        
        }else{
            printf("A fajl nem letezik!");
        }
}
void copy_file_rendezveny(const char *from,const char *to)
{
	FILE *fr;
	FILE *t;
	fr = fopen(from,"r");
	t = fopen(to,"w");
	
	char line[MAX_SIZE];
	char name[MAX_SIZE];
	int rid;
	while(fgets(line,sizeof(line),fr) != NULL)
	{
		sscanf(line,"%s %d",name,&rid);
		fprintf(t,"%s %d\n",name,rid);
	}
	
	fclose(fr);
	fclose(t);
	
	remove("helpfile.txt");
}

void copy_file(const char *from,const char *to)
{
	FILE *fr;
	FILE *t;
	fr = fopen(from,"r");
	t = fopen(to,"w");
	
	char line[MAX_SIZE];
	int vid;
	char name[MAX_SIZE];
	char email[MAX_SIZE];
	int rid;
        char date[MAX_SIZE];
        
	while(fgets(line,sizeof(line),fr) != NULL)
	{
		sscanf(line,"%d %s %s %d %s",&vid,name,email,&rid,date);
		fprintf(t,"%d %s %s %d %s\n",vid,name,email,rid,date);
	}
	
	fclose(fr);
	fclose(t);
	
	remove("helpfile.txt");
}