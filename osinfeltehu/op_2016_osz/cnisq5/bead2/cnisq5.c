/*Egy kiállításokat, rendezvényeket szervező társaság a "King of Stands", elektronikus látogatói nyilvántartást készített. A látogatók a nyilvántartásba jelentkezéskor megadják nevüket, mail címüket, a rendezvény azonosító számát! Az elektronikus rendszer a jelentkezés nyugtázásaként visszaadja, hogy hányadik vendégként történt meg a regisztráció! Az alkalmazás adjon lehetőséget vendég jelentkezéshez, a jelentkezettek listázásához, egy vendég adatainak módosításához, törléséhez. Legyen lehetőségünk egy új rendezvény indításához, egy rendezvény összes adatának törléséhez! A jelentkezőknél a korábbi adatok mellett rögzítsük automatikusan a jelentkezési időt(rendszeridő) is(ezt nem kell bekérni)!*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h> //ctime
#include <sys/types.h>

#define MAX_SIZE 512

struct vendeg {
    int id;              
    char nev[100];
	char e_mail[200];
	int rid;
	time_t time;
};

struct rendezveny {
    int id;
	char helyszin[200];
	int resztvevokszama;
};

struct helper {
	int id;
	int count;
};

struct visszajelzes {
	int siker;
	struct vendeg resztvett[100];
	int emberek;
};

//void menu();
void ujvendeg();
void ujrendezveny();
void listazas();
void modositas();
void vendegtorles();
void rendezvenytorles();
void lebonyolitas();

int main(int argc,char *argv[])
{	
	char menu[MAX_SIZE];	
	int exit = 0;
	do	
	{
		printf("\nAz alabbi menupontok kozul valaszthat\n");
		printf("1. Bejelentkezes\n");
		printf("2. Vendegek listazasa\n");
		printf("3. Vendeg adatainak modositasa\n");
		printf("4. Vendeg torlese\n");
		printf("5. Uj rendezveny\n");
		printf("6. Rendezveny torlese\n");
		printf("7. Rendezvenyek lebonyolitasa\n");
		printf("8. Kilepes\n");
		printf("\nAdja meg a menupont kodjat\t");
		scanf("%s",menu);
		if( strcmp(menu,"1") == 0 )
		{
			ujvendeg();
		}
		else if( strcmp(menu,"2") == 0 )
		{
			printf("2. Vendegek listazasa\n");
			listazas();
		}
		else if( strcmp(menu,"3") == 0 )
		{
			printf("3. Vendeg adatainak modositasa\n");
			modositas();
		}
		else if( strcmp(menu,"4") == 0 )
		{
			printf("4. Vendeg torlese\n");
			vendegtorles();
		}
		else if( strcmp(menu,"5") == 0 )
		{
			printf("5. Uj rendezveny\n");
			ujrendezveny();
		}
		else if( strcmp(menu,"6") == 0 )
		{
			printf("6. Rendezveny torlese\n");
			rendezvenytorles();
		}
		else if( strcmp(menu,"7") == 0 )
		{
			printf("7. Rendezvenyek lebonyolitasa\n");
			lebonyolitas();
		}
		else if( strcmp(menu,"8") == 0 )
		{
			exit = 1;
		}
	}while (exit == 0);
	
	return 0;
}

void menu()
{
	
}

void ujvendeg()		//BESZÚRÁS
{
	
	FILE *rendezvenyek;
	rendezvenyek = fopen("rendezveny.bin", "rb+");
	
	struct vendeg v;
	struct rendezveny r;
	
	printf("Add meg a vendeg adatait \n");
	printf("Vendeg neve:\t");
	scanf("%s",v.nev);
	printf("Vendeg e-mail cime:\t");
	scanf("%s",v.e_mail);
	printf("Rendezveny azonositoja:\t");
	scanf("%d", &v.rid);
	
	int bool;
	bool = 0;
	if (!rendezvenyek)
	{
		bool = -1;
	}
	else
	{
		int count = 0;
		while(bool != 1 && fread(&r,sizeof(struct rendezveny),1,rendezvenyek) != 0)
		{
			if( v.rid == r.id )	
			{
				bool = 1;
				r.resztvevokszama++;
				fseek(rendezvenyek,sizeof(struct rendezveny)*count,SEEK_SET);
				fwrite(&r,sizeof(struct rendezveny),1,rendezvenyek);
			}
			count++;
		};
		fclose(rendezvenyek);
	}
	
	if (bool != 1)
	{
		printf("Nem letezik rendezveny a megadott azonositoval!");
	}
	else
	{
		FILE *vendegek;
		FILE *helping;
		vendegek = fopen("vendeg.bin","ab+");
		helping = fopen ("help.bin", "ab+");
		struct helper h;
	
		if (fread(&h, sizeof(struct helper), 1, helping) != 0)
		{
			h.id++;
			h.count++;
		}
		else 
		{
			h.id = 1;
			h.count = 1;
		}
		
		fclose(helping);
		
		helping = fopen ("help.bin", "wb+");
		fwrite(&h,sizeof(struct helper),1,helping);
		
		fclose(helping);
		
		v.id = h.id;
		v.time = time(NULL);
		fwrite(&v,sizeof(struct vendeg),1,vendegek);
		
		
		fclose(vendegek);
		
		printf("\nA vendeg %d", h.count);
		printf(". regisztralt vendeg, es a rendezvenyen pedig a %d", r.resztvevokszama );
		printf(". vendeg\n");
	}
	
	//menu();
}

void ujrendezveny()		//BESZÚRÁS
{
	
	FILE *rendezvenyek;
	rendezvenyek = fopen("rendezveny.bin", "ab+");
	
	struct rendezveny r1,r2;
	
	printf("Add meg a rendezveny azonositojat:\t");
	scanf("%d",&r1.id);
	printf("Add meg a rendezveny helyszinet:\t");
	scanf("%s",r1.helyszin);
	
	int bool;
	bool = 0;
	while(bool != 1 && fread(&r2,sizeof(struct rendezveny),1, rendezvenyek) != 0)
	{
		if( r1.id == r2.id )	
		{
			bool = 1;
		}
	};
		
	
	if (bool == 1)
	{
		printf("Mar letezik rendezveny a megadott azonositoval!");
	}
	else
	{
		r2.id = r1.id;
		strcpy(r2.helyszin, r1.helyszin);
		r2.resztvevokszama = 0;
		fwrite(&r2,sizeof(struct rendezveny),1, rendezvenyek);
	}
	fclose(rendezvenyek);
	
	//menu();
}
void listazas()		//LISTÁZÁS
{	
	FILE *vendegek;
	struct helper help;
	struct vendeg v;
	vendegek = fopen("vendeg.bin","rb");

	printf("\nVENDEGEK ADATAI:\n");
	if (vendegek)
	{		
		while(fread(&v,sizeof(struct vendeg), 1, vendegek) != 0)
		{
			printf("%d\n",v.id);
			printf("%s\n", v.nev);
			printf("%s\n", v.e_mail);
			printf("%d\n",v.rid);
			printf(ctime(&v.time));
			printf("\n");
		};
		fclose(vendegek);
	}
	
	
	FILE *rendezvenyek;
	struct rendezveny r;
	
	rendezvenyek = fopen("rendezveny.bin","rb");
	
	printf("\nRENDEZVENYEK ADATAI:\n");
	if (rendezvenyek)
	{		
		while(fread(&r,sizeof(struct rendezveny), 1, rendezvenyek) != 0)
		{
			printf("%d\n",r.id);
			printf("%s\n", r.helyszin);
			printf("%d\n",r.resztvevokszama);
			printf("\n");
		};
		fclose(rendezvenyek);
	}
	//menu();
	
}

void modositas()		//MÓDOSÍTÁS
{
	char menu[MAX_SIZE];
	struct vendeg v;
	struct rendezveny r;
	
	FILE *vendegek;
	vendegek = fopen("vendeg.bin", "rb+");
	FILE *rendezvenyek;
	rendezvenyek = fopen("rendezveny.bin", "rb+");
	
	if (vendegek)
	{
		printf("\nMi alapjan szeretne megadni a modositando vendeget:\t");
		printf("\n1. Azonosito alapjan");
		printf("\n2. Nev alapjan");
		printf("\n3. E-mail cim alapjan");
		printf("\nKerem valasszon menupontot:\t");
		scanf("%s",menu);
		
		int bool = 0;
		int count = 0;
		
		if( strcmp(menu,"1") == 0 )
		{
			int id;
			printf("Kerem adja meg az azonositot: \t");
			scanf("%d",&id);
			
			while(bool != 1 && fread(&v,sizeof(struct vendeg),1,vendegek) != 0)
			{
				if( v.id == id )	
				{
					bool = 1;
				}
				count++;
			};
			
		} else if (strcmp(menu,"2") == 0 )
		{
			char nev[100];
			printf("Kerem adja meg a nevet: \t");
			scanf("%s", nev);
			
			while(bool != 1 && fread(&v,sizeof(struct vendeg),1,vendegek) != 0)
			{
				if( strcmp(v.nev,nev) == 0  )	
				{
					bool = 1;
				}
				count++;
			};
		
		} else if (strcmp(menu,"3") == 0 )
		{
			char e_mail[200];
			printf("Kerem adja meg az e-mail cimet: \t");
			scanf("%s", e_mail);
			
			while(bool != 1 && fread(&v,sizeof(struct vendeg),1,vendegek) != 0)
			{
				if( strcmp(v.e_mail,e_mail) == 0  )	
				{
					bool = 1;
				}
				count++;
			};
		}
		if (bool !=0)
		{
			int rid;
			
			printf("A modositani kivant vendeg: \t");
			printf("%d\n",v.id);
			printf("%s\n", v.nev);
			printf("%s\n", v.e_mail);
			printf("%d\n\n",v.rid);
			printf("Kerem adja meg az uj nevet: \t");
			scanf("%s", v.nev);
			printf("Kerem adja meg az uj e-mail cimet: \t");
			scanf("%s", v.e_mail);
			printf("Kerem adja meg az uj rendezveny azonositot: \t");
			scanf("%d", &rid);
			
			if (rid != v.rid)
			{
				int bool2 = 0;
				int count2 = 0;
				while(bool2 != 1 && fread(&r,sizeof(struct rendezveny),1,rendezvenyek) != 0)
				{
					if( rid == r.id )	
					{
						bool2 = 1;
						r.resztvevokszama++;
						/*fprintf(rendezvenyek,"%d %d", r.id, r.resztvevokszama);*/
						fseek(rendezvenyek,sizeof(struct rendezveny)*count2,SEEK_SET);
						fwrite(&r,sizeof(struct rendezveny),1,rendezvenyek);
					}
					count2++;
				};
				fclose(rendezvenyek);
				
				if (bool2 !=0)
				{
					rendezvenyek = fopen("rendezveny.bin", "rb+");
					int bool3 = 0;
					int count3 = 0;
					while(bool3 != 1 && fread(&r,sizeof(struct rendezveny),1,rendezvenyek) != 0)
					{
						if( v.rid == r.id )	
						{
							bool3 = 1;
							r.resztvevokszama--;
							/*fprintf(rendezvenyek,"%d %d", r.id, r.resztvevokszama);*/
							fseek(rendezvenyek,sizeof(struct rendezveny)*count3,SEEK_SET);
							fwrite(&r,sizeof(struct rendezveny),1,rendezvenyek);
						}
						count3++;
					};
					fclose(rendezvenyek);
					v.id = rid;
					fseek(vendegek,sizeof(struct vendeg)*(count-1),SEEK_SET);
					fwrite(&v,sizeof(struct vendeg),1,vendegek);
				}
				else
				{
					printf("A megadott rendezveny azonositoval nem letezik rendezveny");
				}
					
			}
			else
			{
				fclose(rendezvenyek);
				fseek(vendegek,sizeof(struct vendeg)*(count-1),SEEK_SET);
				fwrite(&v,sizeof(struct vendeg),1,vendegek);
				printf("Modosul.");
			}		
			
		}
		else
		{
			printf("Nincs ilyen vendeg a nyilvantartasban.");
		}	
		fclose(vendegek);
	}	
	else
	{
		printf("Egyetlen vendeg sincs a nyilvantartasban.");
	}
	//menu();
}

void vendegtorles()		
{
	char menu[MAX_SIZE];
	struct vendeg v;
	struct rendezveny r;
	struct helper h;
	
	FILE *vendegek;
	vendegek = fopen("vendeg.bin", "rb+");
	FILE *tempo;
	tempo = fopen("temp.bin", "wb+");
	FILE *rendezvenyek;
	rendezvenyek = fopen("rendezveny.bin", "rb+");
	FILE *helping;
	helping = fopen("help.bin", "rb+");
	
	if (vendegek)
	{
		printf("\nMi alapjan szeretne megadni a torlendo vendeget:\t");
		printf("\n1. Azonosito alapjan");
		printf("\n2. Nev alapjan");
		printf("\n3. E-mail cim alapjan");
		printf("\nKerem valasszon menupontot:\t");
		scanf("%s",menu);
		
		int bool = 0;
		int count = 0;
		
		if( strcmp(menu,"1") == 0 )
		{
			int id;
			printf("Kerem adja meg az azonositot: \t");
			scanf("%d",&id);
			
			while(bool != 1 && fread(&v,sizeof(struct vendeg),1,vendegek) != 0)
			{
				if( v.id == id )	
				{
					bool = 1;
				}
				count++;
			};
			
		} else if (strcmp(menu,"2") == 0 )
		{
			char nev[100];
			printf("Kerem adja meg a nevet: \t");
			scanf("%s", nev);
			
			while(bool != 1 && fread(&v,sizeof(struct vendeg),1,vendegek) != 0)
			{
				if( strcmp(v.nev,nev) == 0  )	
				{
					bool = 1;
				}
				count++;
			};
		
		} else if (strcmp(menu,"3") == 0 )
		{
			char e_mail[200];
			printf("Kerem adja meg az e-mail cimet: \t");
			scanf("%s", e_mail);
			
			while(bool != 1 && fread(&v,sizeof(struct vendeg),1,vendegek) != 0)
			{
				if( strcmp(v.e_mail,e_mail) == 0  )	
				{
					bool = 1;
				}
				count++;
			};
		}
		if (bool !=0)
		{
			fclose(vendegek);
			vendegek = fopen("vendeg.bin", "rb+");
			
			int rid;
			printf("A torolni kivant vendeg: \t");
			printf("%d\n",v.id);
			printf("%s\n", v.nev);
			printf("%s\n", v.e_mail);
			printf("%d\n\n",v.rid);
		
			int bool2 = 0;
			int count2 = 0;
			while(bool2 != 1 && fread(&r,sizeof(struct rendezveny),1,rendezvenyek) != 0)
			{
				if( v.rid == r.id )	
				{
					bool2 = 1;
					r.resztvevokszama--;
					fseek(rendezvenyek,sizeof(struct rendezveny)*count2,SEEK_SET);
					fwrite(&r,sizeof(struct rendezveny),1,rendezvenyek);
					}
					count2++;
			};
			fclose(rendezvenyek);
			
			int id = v.id;
			while(fread(&v,sizeof(struct vendeg),1,vendegek) != 0)
			{
				if( v.id != id )	
				{
					fwrite(&v, sizeof(struct vendeg),1,tempo);
				}
			};
			fclose(tempo);
			fclose(vendegek);
			
			if(fread(&h,sizeof(struct helper),1,helping) != 0)
			{
				h.count--;
				fseek(helping,0, SEEK_SET);
				fwrite(&h,sizeof(struct helper),1,helping);
			};
			
			fclose(helping);
			remove("vendeg.bin");
			rename("temp.bin", "vendeg.bin");
		}
		else
		{
			printf("Nincs ilyen vendeg a nyilvantartasban.");
			fclose(vendegek);
		}	
		
	}	
	else
	{
		printf("Egyetlen vendeg sincs a nyilvantartasban.");
	}
	//menu();
}

void rendezvenytorles()
{
	char menu[MAX_SIZE];
	struct vendeg v;
	struct rendezveny r;
	struct helper h;
	
	FILE *vendegek;
	vendegek = fopen("vendeg.bin", "rb+");
	FILE *tempo;
	tempo = fopen("temp.bin", "wb+");
	FILE *rendezvenyek;
	rendezvenyek = fopen("rendezveny.bin", "rb+");
	FILE *helping;
	helping = fopen("help.bin", "rb+");
	
	int rid;
	
	if (rendezvenyek)
	{
		printf("\nAdja meg a torolni kivant rendezveny azonositojat:\t");
		scanf("%d",&rid);
		
		int bool = 0;
		
		while(fread(&r,sizeof(struct rendezveny),1,rendezvenyek) != 0)
		{
			if( r.id == rid )	
			{
				bool = 1;
			}
			else
			{
				fwrite(&r, sizeof(struct rendezveny), 1, tempo);
			}
		};
		fclose(rendezvenyek);
		fclose(tempo);

		remove("rendezveny.bin");
		rename("temp.bin", "rendezveny.bin");
		
		
		if (bool !=1)
		{
			printf("Nem letezik ilyen azonositoju rendezveny.");
		}
		else
		{
			if (vendegek)
			{
				tempo = fopen("temp.bin", "wb+");
				
				int count = 0;
				while(fread(&v,sizeof(struct vendeg),1, vendegek) != 0)
				{
					if( v.rid == rid )	
					{
						count++;
					}
					else
					{
						fwrite(&v, sizeof(struct vendeg), 1, tempo);
					}
				};
				fclose(vendegek);
				fclose(tempo);

				remove("vendeg.bin");
				rename("temp.bin", "vendeg.bin");
				
				if(fread(&h,sizeof(struct helper),1,helping) != 0)
				{
					h.count = h.count - count;
					fseek(helping,0, SEEK_SET);
					fwrite(&h,sizeof(struct helper),1,helping);
				};
				
				fclose(helping);
				
			}
			
		}
		
	}	
	else
	{
		fclose(tempo);
		printf("Egyetlen rendezveny sincs a nyilvantartasban.");
	}
	//menu();
}

void handler(int signumber){
  if (signumber == SIGUSR1)
  {
	  
  };
}

void lebonyolitas() 
{
	signal(SIGUSR1,handler);
	
	srand(time(0));
	
	int pipemain[2];
	int pipechild[2];
	pid_t pid;
	
	FILE *rendezvenyek;
	struct rendezveny r;
	
	struct visszajelzes vissza;
	
	rendezvenyek = fopen("rendezveny.bin","rb");
	if (rendezvenyek)
	{		
		while(fread(&r,sizeof(struct rendezveny), 1, rendezvenyek) != 0)
		{
			FILE *vendegek;
			struct vendeg v;
			vendegek = fopen("vendeg.bin","rb");
			struct vendeg resztvevok[100];
			int i =0;
			int j =0;
		
			if (vendegek)
			{		
				while(fread(&v,sizeof(struct vendeg), 1, vendegek) != 0)
				{
					if (v.rid == r.id)
					{
						resztvevok[i] = v;				
						i++;
					}
				};
				fclose(vendegek);
			}
		
		
		
			if (pipe(pipemain) == -1) 
			{
				perror("Hiba a pipe nyitaskor!");
				exit(EXIT_FAILURE);
			}
			if (pipe(pipechild) == -1) 
			{
				perror("Hiba a pipe nyitaskor!");
				exit(EXIT_FAILURE);
			}
		
			pid = fork();	// creating parent-child processes
			if (pid == -1) 
			{
				perror("Fork hiba");
				exit(EXIT_FAILURE);
			}

			if (pid == 0) // child process
			{		    	
				
				close(pipemain[1]);  //Usually we close the unused write end
			
			
				printf("Partner megkapta a helyszint!\n");
				read(pipemain[0],r.helyszin,sizeof(r.helyszin)); // reading max 100 chars
				printf("A partner utazik ide: %s",r.helyszin);
				printf("\n");
			
				
				kill(getppid(),SIGUSR1);
			
			
				printf("A resztvevok:\n");
				read(pipemain[0],resztvevok,sizeof(resztvevok)); // reading max 100 chars
				i = 0;
				j = 0 ;
				int eljott;
				while (i < r.resztvevokszama)
				{
					printf("%s\n", resztvevok[i].nev);
					eljott = rand() % 10;
					if (eljott != 5)
					{
						vissza.resztvett[j] = resztvevok[i];
						j++;
					}
					i++;
				}
				vissza.emberek = j;
				close(pipemain[0]); // finally we close the used read end
			
				vissza.siker = rand();
			
				printf("Megkezdodik a rendezveny!\n");
				printf("Veget ert a rendezveny!\n");
			
				close(pipechild[0]); //Usually we close unused read end
						
				write(pipechild[1], &vissza, sizeof(struct visszajelzes));
			
			
				close(pipechild[1]); // Closing write descriptor 
				sleep(2);
				
            
			} 
			else
			{
				close(pipemain[0]); //Usually we close unused read end
				
			
				printf("Helyszin elkuldese partnernek!\n");
				write(pipemain[1], r.helyszin, sizeof(r.helyszin));
				pause();		// waiting for child process (not necessary)
				printf("A partner megerkezett a helyszinre\n");
			
				//wait();
			
				printf("Resztvevok elindultak!\n");			
         
				write(pipemain[1], &resztvevok, sizeof(resztvevok));     
				//wait();
				close(pipemain[1]); //Usually we close unused read end
			
				close(pipechild[1]);  //Usually we close the unused write end	
				read(pipechild[0], &vissza,sizeof( struct visszajelzes)); // reading max 100 chars
				printf("A rendezveny visszajelzese:\n");
				printf("A rendezveny sikere: %d\n", vissza.siker);
				printf("A rendezvenyen resztvettek:\n");
				i = 0;
				while (i < vissza.emberek)
				{
					printf("%s\n", vissza.resztvett[i].nev);
					i++;
				}
			
				close(pipechild[0]); // finally we close the used read end
				printf("A rendezveny befejezodott!");
				//int status;
				//waitpid(pid,&status,0); //wait for the end of child process
				//wait();
					
			}
			wait();
		}
		
		fclose(rendezvenyek);
		
	}
	else
	{
		printf("Nincsenek lebonyolitando rendezvenyek!\n");
	}
	//menu();
	
}