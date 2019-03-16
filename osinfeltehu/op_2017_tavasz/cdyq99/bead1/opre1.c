#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Question_list{
	char* question;
	int answernumb;
	char* answers[4];
	time_t Time;
	struct Question_list *next;
};

typedef struct Question_list Question_list;

void clear (void)
{    
	while ( getchar() != '\n' );
}

char *inputString(FILE* fp, size_t size){
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

void Menu(){
	printf("\n1. Kerdessor szerkesztese\n");
	printf("2. Kerdessor listazasa\n");
	printf("3. Kerdessor veglegesitese\n");
	printf("4. Kilepes\n");
}

void Menuedit(){
	printf("\n1. Kerdes hozzadasa\n");
	printf("2. Kerdes szerkesztese\n");
	printf("3. Kerdes torlese\n");
	printf("4. Visszalepes a menube\n");
}

void Menuansweredit(){
	printf("\n1. Valasz hozzaadasa\n");
	printf("2. Valasz szerkesztese\n");
}

void Create_question(Question_list *survey){
	int n, i;
	Question_list* q = survey;
	Question_list* p = q->next;
	while(p != NULL){
		q = p;
		p = q->next;
	}
	q->next = (Question_list*)malloc(sizeof(Question_list));
	clear();
	printf("Adja meg a kerdest!: ");
	q->next->question = inputString(stdin, 1);
	printf("Adja meg, hogy hany valaszt akar megadni (2-4): ");
	scanf("%d", &n);
	clear();
	if(n >= 2 && n <= 4){
		q->next->answernumb = n;
		for(i = 0; i<n ; i++) {
			printf("Adja meg a(z) %d. valaszt: ", i+1);
			q->next->answers[i] = inputString(stdin, 1);
		}
		time(&q->next->Time);
	}else{
		free(q->next);
		q->next = NULL;
		printf("Legalabb 2, legfeljebb 4 valasz lehet!!\n");
	}
}

void Modify(Question_list *survey){
	Question_list* p = survey;
	Question_list* q = (Question_list*)malloc(sizeof(Question_list));
	int n;
	int i = 0;
	char c;
	char* tmp;
	printf("Hanyadik kerdest akarja megvaltoztatni?: ");
	scanf("%d", &n);
	clear();
	while(i<n-1){
		i++;
		p = p->next;
	}
	printf("Mit szeretne valtoztatni? (K/V): ");
	scanf("%c", &c);
	clear();
	switch(c){
		case 'K':
			q->answernumb = p->next->answernumb;
			for(i = 0; i<q->answernumb; i++){
				q->answers[i] = p->next->answers[i];
			}
			q->next = p->next->next;
			free(p->next);
			p->next = q;
			printf("Adja meg az uj kerdest: ");
			q->question = inputString(stdin, 1);
			time(&q->Time);
			break;
		case 'V':
			Menuansweredit();
			scanf("%d", &n);
			switch(n){
				case 1:
					if(p->answernumb == 4){
						printf("Mar maximalis szamu valaszlehetoseg van megadva!!!\n");
					}else{
						printf("Adja meg az uj valaszt: ");
						p->answers[p->answernumb] == inputString(stdin, 1);
						p->answernumb++;
					}
					break;
				case 2:
					printf("Hanyadik valaszt szeretne modositani?: ");
					scanf("%d", &n);
					clear();
					q->answernumb = p->next->answernumb;
					for(i = 0; i<q->answernumb; i++){
						if(i != n-1){
							q->answers[i] = p->next->answers[i];
						}else{
							printf("Adja meg az uj valaszt: ");
							q->answers[i] = inputString(stdin, 1);
						}
					}
					q->next = p->next->next;
					q->question = p->next->question;
					time(&q->Time);
					free(p->next);
					p->next = q;
					break;
				default: printf("Hibas menusorszam!");
			}
			break;
		default: printf("K: kerdes, V: valasz, mas nem elfogadhato!!!\n");
	}
}

void Delete(Question_list *survey){
	if(survey->next == NULL){
		printf("Meg nem adott meg kerdest!!\n");
	}else{
		char c;
		int i = 0;
		int n;
		Question_list* p = survey;
		Question_list* q = p->next;
		printf("Adja meg, hogy kerdest, vagy valaszt akar torolni (K/V): ");
		scanf("%c", &c);
		clear();
		if(c == 'K' || c == 'k'){
			printf("Adja meg, hogy hanyadik kerdest akarja torolni: ");
			scanf("%d", &n);
			clear();
			while(i<n){
				q = p;
				p = p->next;
				i++;
			}
			q->next = p->next;
			free(p);
		}else if(c == 'V' || c == 'v'){
			printf("Adja meg, hogy hanyadik kerdesnel akar torolni: ");
			scanf("%d", &n);
			clear();
			if(p->answernumb <= 2){
				printf("Legalabb 2 valaszlehetoseg kell minden kerdesnel!!!");
			}else{
				while(i<n){
					p = p->next;
					i++;
				}
				printf("Adja meg, hogy hanyadik valaszt szeretne torolni: ");
				scanf("%d", &n);
				clear();
				for(i = 0; i < p->answernumb-1; i++){
					if(i >= n-1){
						p->answers[i] = p->answers[i+1];
					}
				}
				p->answernumb--;
			}
		}else{
			printf("K: kerdes, V: valasz, mas nem elfogadhato!!!\n");
		}
	}
}

void Listing(Question_list *survey){
	Question_list* p = survey->next;
	int i;
	int j = 1;
	if(survey->next == NULL) {
		printf("Meg nem adott meg kerdest!\n");
	} else {
		while(p != NULL){
			printf("\n%d. ", j);
			printf(p->question);
			printf("\n");
			for(i = 0; i < p->answernumb; i++){
				printf("\t%d) ", i+1);
				printf(p->answers[i]);
				printf("\n");
			}
			printf("letrehozas datuma : ");
			struct tm *tmp;
			tmp = gmtime(&p->Time);
			printf("%d-%02d-%02d %02d:%02d:%02d\n",tmp->tm_year+1900,tmp->tm_mon,tmp->tm_mday, tmp->tm_hour+2, tmp->tm_min, tmp->tm_sec);
			p = p->next;
			j++;
		}
	}
}

void File_out(Question_list *survey){
	
	FILE *f = fopen("survey.txt", "w+");
	if (f == NULL)
	{
		printf("Hiba a megnyitas soran!\n");
		exit(1);
	}
	
	Question_list* p = survey->next;
	int i;
	int j = 1;
	int k = 0;
	char c;
	if(survey->next == NULL) {
		printf("Meg nem adott meg kerdest!\n");
	} else {
		while(p != NULL){
			printf("\n%d. ", j);
			printf(p->question);
			printf("\n");
			for(i = 0; i < p->answernumb; i++){
				printf("\t%d) ", i+1);
				printf(p->answers[i]);
				printf("\n");
			}
			printf("Szeretne menteni ezt a kerdest? (Y/N): ");
			scanf("%c", &c);
			clear();
			if(c == 'Y' || c == 'y'){
				k++;
				fprintf(f, "%d. ", k);
				fprintf(f, "%s\n", p->question);
				for(i = 0; i < p->answernumb; i++){
					fprintf(f, "\t%d) ", i+1);
					fprintf(f, "%s\n", p->answers[i]);
				}
				fprintf(f, "letrehozas datuma : ");
				struct tm *tmp;
				tmp = gmtime(&p->Time);
				fprintf(f, "%d-%02d-%02d %02d:%02d:%02d\n",tmp->tm_year+1900,tmp->tm_mon,tmp->tm_mday, tmp->tm_hour+2, tmp->tm_min, tmp->tm_sec);
				fprintf(f, "\n");
			}
			p = p->next;
			j++;
		}
	}
	
	fclose(f);
}


int main(void){
	char c;
	int m;
	Question_list survey;
	survey.next = NULL;
	while(1){
		Menu();
		printf("\nAdja meg a menu sorszamat: ");
		scanf("%d", &m);
		clear();
		switch(m){
			case 1: 
				Menuedit();
				printf("\nAdja meg a menu sorszamat: ");
				scanf("%d", &m);
				switch(m){
					case 1: 
						do{
							Create_question(&survey);
							printf("Akar meg kerdest hozzaadni? (Y/N): ");
							scanf("%c", &c);
						}while(c == 'Y' || c == 'y');
						break;
					case 2: Modify(&survey); break;
					case 3: clear(); Delete(&survey); break;
					case 4: break;
					default: printf("\n1 es 4 kozotti szamot adjon meg!!!!\n\n"); break;
				}
				break;
			case 2: Listing(&survey); break;
			case 3: File_out(&survey); break;
			case 4: return 0;
			default: printf("\n1 es 4 kozotti szamot adjon meg!!!!\n\n"); break;
		}
	}
}