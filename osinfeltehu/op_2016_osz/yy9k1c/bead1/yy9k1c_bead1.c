#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <errno.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>


int size = 0;
int maxG = 100;
int maxE = 100;
struct guest {
    char name[100];
    char mail[100];
    int eventID;
    int date;
};

int main() {
    char * r = (char *)malloc(sizeof(char)*maxE);
	struct guest * g = (struct guest *)malloc(sizeof(struct guest)*maxG);
	int menuPoint;
    do{
        printf("uj vendeg(1), lista(2), adatmodositas-vendeg(3), torles-vendeg(4), uj rendezveny(5), torles-rendezveny(6), kilepes(0) \n");
        printf("kerem a valasztott szamot(0-8): ");
        scanf("%d",&menuPoint);
		if(menuPoint==1){
            struct guest newGuest;
			printf("vendeg adatok(szokozokkel): ");
			scanf("%s %s %i",&newGuest.name,&newGuest.mail,&newGuest.eventID);
			newGuest.date = time(NULL);
			if(newGuest.eventID < 0 || newGuest.eventID >= maxE || r[newGuest.eventID]==0) {
				printf("nem letezo rendezvenyazonosito!\n");
				//break;
				return 0;
			}
			if(maxG <= size) {
				g = (struct guest *)realloc(g,sizeof(struct guest)*maxG*2);
				maxG = maxG*2;
			}
			g[size] = newGuest;
			size++;
			printf("A %s vendeg %i.-nak/nek regisztralt.\n",newGuest.name,size);
                break;
            
		}
		else{
			if(menuPoint==2){
				int i;     
				for(i=0;i<size;i++) {
					printf("%i. %s mail: %s rendezveny: %i\n",(i+1),g[i].name,g[i].mail,g[i].eventID);
				}
                break;
            }
			else{
				if(menuPoint==3){
					int number;
					int type;
					printf("vendegsorszam: ");
					scanf("%i",&number);
					if(number < 1 || number > size) {
						printf("Nincs ilyen sorszamu vendeg!\n");
						break;
					}
					printf("nev(1), email(2), rendezveny azonosito(3)\n");
					printf("modositando adat szama(1-3): \n");
					scanf("%i",&type);
					switch (type) {
						
						case 1: {
							printf("uj nev: ");
							scanf("%s",&g[number-1].name);
							break;
						}
						case 2: {
							printf("uj email: ");
							scanf("%s",&g[number-1].mail);
							break;
						}
						case 3: {
							printf("uj rendezveny azonosito: ");
							int id;
							scanf("%i",&id);
							if(id < 0 || id >= maxE || r[id] == 0) {
								printf("Nincs ilyen rendezveny!");
								break;
							}
							g[number-1].eventID = id;
							break;
						}
						default: {
							printf("Nem megfelelo szam!\n");
							break;
						}
					}
					break;   
				}
				else{
					if(menuPoint==4){
						int number;
						printf("vendeg sorszama: ");
						scanf("%i",&number);
						if(number < 1 || number > size) {
							printf("nem letezo!\n");
							break;
						}
						struct guest * ng = (struct guest *)malloc(sizeof(struct guest)*maxG); 
						int i;
						int j = 0;
						for(i = 0;i < size; i++) {
							if(number-1 != i) {
								ng[j] = g[i];
								j++;
							}
						}
						free(g);
						g = ng;
						size--;
						break;
					}
					else{
						if(menuPoint==5){
							int id;
							printf("rendezveny azonosito: ");
							scanf("%i",&id);
							if(id < 0 ||id  >= maxE) {
								printf("helytelen azonosito! csak 0 es %i kozotti szam lehet\n",maxE-1); 
								break;    
							} 
							r[id] = 1;
							break; 
						}
						else{
							if(menuPoint==6){
								int id;
								printf("rendezveny azonosito: ");
								scanf("%i",&id);
								if(id < 0 ||id  >= maxE) {
									printf("helytelen azonosito! csak 0 es %i kozotti szam lehet\n",maxE-1); 
									break;    
								}
								if(r[id] == 0) {
									printf("meg nem volt letrehozva");
									break;
								}
								struct guest * ng = (struct guest *)malloc(sizeof(struct guest)*maxG); 
								int i;
								int j, h = 0;
								for(i = 0;i < size; i++) {
									if(g[i].eventID != id) {
										ng[j] = g[i];
										j++;
									} else {
										h++;
									}
								}
								free(g);
								g = ng;
								size-=h;
								break; 
							}
						}
					}
				}
			}
		}
    }while(menuPoint!=0);
    return 0;
}