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



struct vendeg {
    char nev[100];
    char email[100];
    int rendezvenyAzon;
    int reg_date;
};

int main() {
    int answer;
    int va_size = 0;
    int va_max_size = 100;
    int ra_max_size = 100;
    char * ra_ptr = (char *)malloc(sizeof(char)*ra_max_size);
    struct vendeg * va_ptr = (struct vendeg *)malloc(sizeof(struct vendeg)*va_max_size);
    char * ra_jump_ptr;
    for(ra_jump_ptr = ra_ptr; ra_jump_ptr-ra_ptr < ra_max_size; ra_jump_ptr+=sizeof(char)) {
        *ra_jump_ptr = 0;
    }
    do {
        printf("1. Uj latogato felvetele\n");
        printf("2. Vendegek listazasa\n");
        printf("3. Vendeg adatainak modositasa\n");
        printf("4. Vendeg torlese\n");
        printf("5. Uj rendezveny felvetele\n");
        printf("6. Rendezveny adatainak torlese\n");
        printf("7. Mentes fajlba\n");
        printf("8. Betoltes fajlbol\n");
        printf("0. Kilepes\n");
        printf("Valasszon menupontot: ");
        scanf("%d",&answer);
        switch(answer) {
            case 1: {
                struct vendeg ujvendeg;
                printf("Irja be a latogato adatait szokozzel elvalasztva (a nev es email max 100-100 karatker): ");
                scanf("%s %s %i",&ujvendeg.nev,&ujvendeg.email,&ujvendeg.rendezvenyAzon);
                ujvendeg.reg_date = time(NULL);
                if(ujvendeg.rendezvenyAzon < 0 || ujvendeg.rendezvenyAzon >= ra_max_size || ra_ptr[ujvendeg.rendezvenyAzon]==0) {
                    printf("Nincs ilyen rendezveny jelenleg!\n");
                    break;
                }
                if(va_max_size <= va_size) {
                    va_ptr = (struct vendeg *)realloc(va_ptr,sizeof(struct vendeg)*va_max_size*2);
                    va_max_size = va_max_size*2;
                }
                va_ptr[va_size] = ujvendeg;
                va_size++;
                printf("A %s vendeg %i.-nak/nek regisztralt.\n",ujvendeg.nev,va_size);
                break;
            }
            case 2: {
                printf("\n---------------------------------------\n\n");
                int i;
                for(i=0;i<va_size;i++) {
                    printf("%i. %s email: %s rendezveny azon: %i\n",(i+1),va_ptr[i].nev,va_ptr[i].email,va_ptr[i].rendezvenyAzon);
                }
                printf("\n---------------------------------------\n\n");
                break;
            }
            case 3: {
                int sorszam;
                int adatipus;
                printf("Kerem a vendeg sorszamat: ");
                scanf("%i",&sorszam);
                if(sorszam < 1 || sorszam > va_size) {
                    printf("Nincs ilyen sorszamu vendeg!\n");
                    break;
                }
                printf("1. nev\n2. email\n3. rendezveny azonosito\nMelyik adatot szeretne modositani? ");
                scanf("%i",&adatipus);
                switch (adatipus) {
                    case 1: {
                        printf("Kerem a vendeg uj nevet (max 100 karakter): ");
                        scanf("%s",&va_ptr[sorszam-1].nev);
                        break;
                    }
                    case 2: {
                        printf("Kerem a vendeg uj emailcimet (max 100 karakter): ");
                        scanf("%s",&va_ptr[sorszam-1].email);
                        break;
                    }
                    case 3: {
                        printf("Kerem a vendeg uj rendezvenyazonositojat: ");
                        int azon;
                        scanf("%i",&azon);
                        if(azon < 0 || azon >= ra_max_size || ra_ptr[azon] == 0) {
                            printf("Nincs ilyen rendezveny!");
                            break;
                        }
                        va_ptr[sorszam-1].rendezvenyAzon = azon;
                        break;
                    }
                    default: {
                        printf("Nem megfelelo szam!\n");
                        break;
                    }
                }
                break;   
            }
            case 4: {
                int sorszam;
                printf("Kerem a vendeg sorszamat: ");
                scanf("%i",&sorszam);
                if(sorszam < 1 || sorszam > va_size) {
                    printf("Nincs ilyen sorszamu vendeg!\n");
                    break;
                }
                struct vendeg * va_ptr_new = (struct vendeg *)malloc(sizeof(struct vendeg)*va_max_size); 
                int i;
                int j = 0;
                for(i = 0;i < va_size; i++) {
                    if(sorszam -1 != i) {
                        va_ptr_new[j] = va_ptr[i];
                        j++;
                    }
                }
                free(va_ptr);
                va_ptr = va_ptr_new;
                va_size--;
                break;
            }
            case 5: {
               int azon;
               printf("Kerem a rendezveny azonositojat: ");
               scanf("%i",&azon);
               if(azon < 0 ||azon  >= ra_max_size) {
                    printf("A rendezveny azonositoja 0 es %i kozotti lehet!\n",ra_max_size-1); 
                    break;    
               } 
               ra_ptr[azon] = 1;
               printf("A %i azonositoju rendezveny sikeresen letrehozva!\n",azon);
               break; 
            }
            case 6: {
               int azon;
               printf("Kerem a rendezveny azonositojat: ");
               scanf("%i",&azon);
               if(azon < 0 ||azon  >= ra_max_size) {
                    printf("A rendezveny azonositoja 0 es %i kozotti lehet!\n",ra_max_size-1); 
                    break;    
               }
               if(ra_ptr[azon] == 0) {
                   printf("Csak korabban letrehozott rendezveny minden adatat lehet torolni!");
                   break;
               }
               struct vendeg * va_ptr_new = (struct vendeg *)malloc(sizeof(struct vendeg)*va_max_size); 
                int i;
                int j = 0;
                int s = 0;
                for(i = 0;i < va_size; i++) {
                    if(va_ptr[i].rendezvenyAzon != azon) {
                        va_ptr_new[j] = va_ptr[i];
                        j++;
                    } else {
                        s++;
                    }
                }
                free(va_ptr);
                va_ptr = va_ptr_new;
                va_size-=s;
               break; 
            }
            case 7: {
                char fajnev[100]; 
                printf("Kerem a fajl nevet (Max 100 karakter): ");
                scanf("%s",&fajnev);
                int fd = open(fajnev,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                if(fd < 0) {
                    printf("Hiba a megnyitaskor!\n");
                }
                else if(write(fd,&va_size,sizeof(int)) < 0 ||
                    write(fd,&va_max_size,sizeof(int)) < 0 ||
                    write(fd,va_ptr,sizeof(struct vendeg)*va_size) < 0 ||
                    write(fd,&ra_max_size,sizeof(int)) < 0 ||
                    write(fd,ra_ptr,sizeof(char)*ra_max_size) < 0) {
                    printf("Hiba a iraskor!\n");
                }
                else {
                    printf("Sikeres mentes!\n");
                }
                close(fd);
                break;
            }
            case 8: {
                int stat;
                char fajnev[100]; 
                printf("Kerem a fajl nevet (Max 100 karakter): ");
                scanf("%s",&fajnev);
                int fd = open(fajnev,O_RDONLY);
                if(fd < 0) {
                    printf("Hiba a megnyitaskor!\n");
                    close(fd);
                    break;
                }
                if(read(fd,&va_size,sizeof(int)) < 0 ||
                   read(fd,&va_max_size,sizeof(int)) < 0) {
                    printf("Hiba a olvasaskor!\n");
                    close(fd);
                    break;
                }
                struct vendeg * va_ptr_new = (struct vendeg *)malloc(sizeof(struct vendeg)*va_max_size);
                if(read(fd,va_ptr_new,sizeof(struct vendeg)*va_size) < 0) {
                    printf("Hiba a olvasaskor!\n");
                    close(fd);
                    break;
                }
                free(va_ptr);
                va_ptr = va_ptr_new;
                
                if(read(fd,&ra_max_size,sizeof(int)) < 0) {
                    printf("Hiba a olvasaskor!\n");
                    close(fd);
                    break;
                }
                char * ra_ptr_new = (char *)malloc(sizeof(char)*ra_max_size);
                if(read(fd,ra_ptr_new,sizeof(char)*ra_max_size) < 0) {
                    printf("Hiba a olvasaskor!\n");
                    close(fd);
                    break;
                }
                free(ra_ptr);
                ra_ptr = ra_ptr_new;
                printf("Sikeres beolvasas!\n");
                close(fd);
                break;
            }
            default: {
                answer = 0;
            }
        }
    }while(answer != 0);
   
    return 0;
}

