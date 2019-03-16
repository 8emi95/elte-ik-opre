#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct latogato {
	int azon;
	char knev[20];
	char csnev[20];
	char email[20];
	int rendazon;
	char ido[50];
};

struct rendezveny {
	int rendazon;
	char rendnev[30];
};

typedef struct latogato latogato;
typedef struct rendezveny rendezveny;

latogato jelentkezes(int);
void listaz(latogato*, int);
void modosit(latogato*, int, int);
int torol(latogato*, int, int);
rendezveny ujrendezveny(int);
void rlista(rendezveny*, int);
int rendezvenytorol(rendezveny*, int, int);
int betolto(FILE*, latogato*);
int rbetolto(FILE*, rendezveny*);
void kiir(FILE*, FILE*, rendezveny*, latogato*, int, int);

void string_copy(char*, char*);
char** str_split(char*, const char);


int main() {
	int i = 0;
	int rend = 0;
	FILE* f;
	FILE* r;
	int modazon;
	int rendezvenyazon;
	latogato visitor[1000];
	rendezveny rendezv[20];

    f = fopen("latogatok.txt", "a+");
    r = fopen("rendezvenyek.txt", "a+");
    i = betolto(f, visitor);
    rend = rbetolto(r, rendezv);

	while(1){
		int x;
		
		printf("\nKing of Stands\n");
		printf("1. Jelentkezes\n");
		printf("2. Jelentkezettek listazasa\n");
		printf("3. Vendeg adatainak modositasa\n");
		printf("4. Vendeg torlese\n");
		printf("5. Uj rendezveny inditasa\n");
		printf("6. Rendezveny torlese\n");
		printf("7. Kilepes\n");
		 
		scanf("%i", &x);

		switch(x) {
			case 1:
				visitor[i] = jelentkezes(i);
				i++;
				printf("Ön a(z) %i. latogato\n", i);
				break;
			case 2:
				listaz(visitor, i);
				break;
			case 3:
				printf("Hanyas szamu vedeg adatait szeretne modositani\n");
				scanf("%i", &modazon);
				modosit(visitor,i, modazon);
				break;
			case 4:
				printf("Hanyas szamu vedeget szeretne torolni\n");
				scanf("%i", &modazon);
				i = torol(visitor,i, modazon);
				break;
			case 5:
				rendezv[rend] = ujrendezveny(rend);
				rend++;
				printf("A(z) %i. rendezveny letrehozva\n", rend);
				break;
			case 6:
				rlista(rendezv, rend);
				printf("Adja meg a rendezveny azonositojat\n");
				scanf("%i", &rendezvenyazon);
				rend = rendezvenytorol(rendezv, rend, rendezvenyazon);
				break;
			case 7:
				kiir(f, r, rendezv, visitor, rend, i);
			 	fclose(f);
				exit(1);
				break;
			default:
				kiir(f, r, rendezv, visitor, rend, i);
				printf("nyaaaaah\n");
				fclose(f);
				exit(1);
				break;
		}
	}
}

latogato jelentkezes(int i) {
	int j;
	time_t sido;
	struct tm * timeinfo;
	time ( &sido );
  	timeinfo = localtime ( &sido );

  	char * timeString = asctime(timeinfo);
	latogato jozsi;

	jozsi.azon = i+1;

	printf("Csaladev?\n");
	scanf("%s", jozsi.csnev);

	printf("Keresztnev?\n");
	scanf("%s", jozsi.knev);

	printf("Email cim?\n");
	scanf("%s", jozsi.email);

	printf("Rendezveny azonosito?\n");
	scanf("%i", &jozsi.rendazon);

	string_copy(jozsi.ido, timeString);
	
	printf("Sikeres jelentkezes\n");
	return jozsi;
}

void listaz(latogato* jozsi, int size){
	int i;
	for(i=0; i<size; ++i) {
		printf("Azon: %i, Nev: %s %s, Email: %s, Rendazon: %i \n", jozsi[i].azon, jozsi[i].csnev, jozsi[i].knev, jozsi[i].email,jozsi[i].rendazon);
	}    
}

void modosit(latogato* latogatok, int size, int azon) {
	int i;
	char c;
	bool van = false;
	for(i=0; i<size; ++i) {	
		if(latogatok[i].azon == azon) {
			van = true;
			printf("Azon: %i, Nev: %s %s, Email: %s, Rendazon: %i \n", latogatok[i].azon, latogatok[i].csnev, latogatok[i].knev, latogatok[i].email,latogatok[i].rendazon);
			printf("Modositja (i/n)?\n");
			scanf(" %c", &c);
			if(c=='i') {
				printf("Csaladev?\n");
				scanf("%s", latogatok[i].csnev);

				printf("Keresztnev?\n");
				scanf("%s", latogatok[i].knev);

				printf("Email cim?\n");
				scanf("%s", latogatok[i].email);

				printf("Rendezveny azonosito?\n");
				scanf("%i", &latogatok[i].rendazon);
				continue;
			} else {
				continue;
			}
		}
	}
		if(!van) {
		printf("nincs ilyen azonositoszamu vendeg\n");
	}
}

int torol(latogato* latogatok, int size, int azon) {
	int i;
	char c;
	bool van = false;
	for(i=0; i<size; ++i) {	
		if(latogatok[i].azon == azon) {
			van = true;
			printf("Azon: %i, Nev: %s %s, Email: %s, Rendazon: %i \n", latogatok[i].azon, latogatok[i].csnev, latogatok[i].knev, latogatok[i].email,latogatok[i].rendazon);
			printf("Torli? (i/n)?\n");
			scanf(" %c", &c);
			if(c=='i') {
				size--;
				for(i; i<size; ++i) {
					latogatok[i].azon = latogatok[i+1].azon;
					string_copy(latogatok[i].csnev, latogatok[i+1].csnev);
					string_copy(latogatok[i].knev, latogatok[i+1].knev);
					string_copy(latogatok[i].email, latogatok[i+1].email);
					latogatok[i].rendazon = latogatok[i+1].rendazon;
					string_copy(latogatok[i].ido, latogatok[i+1].ido);
				}
			} else {
				continue;
			}	
		}
		if(!van) {
			printf("nincs ilyen azonositoszamu vendeg\n");
		}
	}
	return size;
}

rendezveny ujrendezveny(int r) {
	rendezveny rendezv;
	printf("rendezveny neve\n");
	scanf("%s", rendezv.rendnev);
	rendezv.rendazon = r+1; 
	return rendezv;
}

void rlista(rendezveny* rendezveny, int size) {
	int i;
	for(i=0; i<size; ++i) {
		printf("Azon: %i, Nev: %s\n", rendezveny[i].rendazon, rendezveny[i].rendnev);
	}    
}

int rendezvenytorol(rendezveny* rendezveny, int size, int azon) {
	int i;
	char c;
	bool van = false;
	for(i=0; i<size; ++i) {	
		if(rendezveny[i].rendazon == azon) {
			van = true;
			printf("Azon: %i, Nev: %s \n", rendezveny[i].rendazon, rendezveny[i].rendnev);
			printf("Torli? (i/n)?\n");
			scanf(" %c", &c);
			if(c=='i') {
				size--;
				for(i; i<size; ++i) {
					rendezveny[i].rendazon = rendezveny[i+1].rendazon;
					string_copy(rendezveny[i].rendnev, rendezveny[i+1].rendnev);
				}
			} else {
				continue;
			}	
		}
		if(!van) {
			printf("nincs ilyen azonositoszamu rendezveny\n");
		}
	}
	return size;
}

int rbetolto(FILE* r, rendezveny* rendezven) {
	int re = 0;
    /*const size_t line_size = 300;
	char* line = malloc(line_size);
	rendezveny rendez;

	while (fgets(line, line_size, r) != NULL)  {
		char** tokens;
		tokens = str_split(line, ',');

		if (tokens){
	        int j;
	        

	        for (j = 0; *(tokens + j); j++) {
	            switch(j) {
	            	case 0: rendez.rendazon = atoi(*(tokens + j));
	            		break;

	            	case 1: string_copy(rendez.rendnev, *(tokens + j));
	            		break;
	            }
	            free(*(tokens + j));
	        }
	        rendezven[re] = rendez;
	        free(tokens);
		}
		re++;
	}
	free(line);*/
	while(!feof(r)){
		rendezveny rendez;
		//int succ = fscanf(r,"%i, %s\n", &rendez.rendazon, &rendez.rendnev);
		int succ = fread(&rendez, sizeof(rendez), 1, r);
		if (succ > 0) {
			rendezven[re] = rendez;
			re++;
		}
	}
	
	printf("\nBetoltott sorok szama: %d \n", re);
	return re;
}

int betolto(FILE* f, latogato* latogatok) {
	int i = 0;
	
	/*while(!feof(f)){
		latogato l;
		int succ = fscanf(f,"%i,%s,%s,%s,%i,%s\n", &l.azon, &l.csnev, &l.knev, &l.email, &l.rendazon, &l.ido);
		printf("%i,%s,%s,%s,%i,%s\n", l.azon, l.csnev, l.knev, l.email, l.rendazon, l.ido);
		if (succ != EOF){
			//latogatok[i] = l;
			i++;
		}
	}*/
	
	while(!feof(f)){
		latogato l;
		int succ = fread(&l, sizeof(l), 1, f);
		//printf("%i,%s,%s,%s,%i,%s\n", l.azon, l.csnev, l.knev, l.email, l.rendazon, l.ido);
		if (succ > 0){
			latogatok[i] = l;
			i++;
		}
	}
	
    /*const size_t line_size = 300;
	char* line = malloc(line_size);

	while (fgets(line, line_size, f) != NULL)  {
		char** tokens;
		tokens = str_split(line, ',');

		if (tokens){
	        int j;
	        latogato l;

	        for (j = 0; *(tokens + j); j++) {
	            switch(j) {
	            	case 0: l.azon = atoi(*(tokens + j));
	            		break;

	            	case 1: string_copy(l.csnev, *(tokens + j));
	            		break;

	            	case 2: string_copy(l.knev, *(tokens + j));
	            		break;

	            	case 3: string_copy(l.email, *(tokens + j));
	            		break;

	            	case 4: l.rendazon = atoi(*(tokens + j));
	            		break;

	            	case 5: string_copy(l.ido, *(tokens + j));
	            		break;
            	}
	        	free(*(tokens + j));
	    	}
	    	latogatok[i] = l;

	    	free(tokens);
	    }
		i++;
	}
	free(line);*/

    printf("\nBetoltott sorok szama: %d \n", i);
    return i;
}


void kiir(FILE* f, FILE* r,rendezveny* rendezv,latogato* visitor, int rend, int i) {
	int j;
	f = freopen("latogatok.txt","w",f);
	r = freopen("rendezvenyek.txt","w",r);
	for(j=0; j<i; ++j) {
		//fprintf(f,"%i,%s,%s,%s,%i,%s\n", visitor[j].azon, visitor[j].csnev, visitor[j].knev, visitor[j].email, visitor[j].rendazon, visitor[j].ido);
		fwrite(&visitor[j], sizeof(struct latogato), 1, f);
	}   
	for(j=0; j<rend; ++j) {
		//fprintf(r,"%i, %s\n", rendezv[j].rendazon, rendezv[j].rendnev);
		fwrite(&rendezv[j], sizeof(struct rendezveny), 1, r);
		
	} 
}



void string_copy(char *ide, char *innen) {
   int i;
 
   for (i = 0; innen[i] != '\0'; ++i) /* ugyanolyan indexűeket */
      ide[i] = innen[i];
   ide[i] = '\0';                     /* és még a lezáró nulla! */
}

char** str_split(char* a_str, const char a_delim) {
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result) {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}
