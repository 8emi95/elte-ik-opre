#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define P1_READ     0
#define P2_WRITE    1
#define P2_READ     2
#define P1_WRITE    3

#define NUM_PIPES   2


typedef struct{
	char vezetek[100];
	char kereszt[100];
	int jelenlet; //0 ha igen, 1 ha nem
} visitor;


int getRan(){return rand() % 101;}

int main(int argc, char *argv[])
{
	int fd[2*NUM_PIPES];
    int val = 0, len, len2, ken1, ken2, ken3, i, k, temp, size, absent;
	float avg, tempavg;
    pid_t pid;
	char helyszin[100];
	char gyerekOk[10];
	visitor lista[30];
	visitor jelen[25];
	srand(time(NULL));
	
	
	
	for (i=0; i<NUM_PIPES; ++i)
    {
        if (pipe(fd+(i*2)) < 0)
        {
            perror("Csovek allokalasanak hibaja.\n");
            exit(EXIT_FAILURE);
        }
    }
	
	 if ((pid = fork()) < 0)
    {
        perror("Fork hiba.\n");
        return EXIT_FAILURE;
    }
	
	if (pid == 0)  // pid0 --> gyerek
    {
		//gyereknek nem kellenek leirok
        close(fd[P1_READ]);
        close(fd[P1_WRITE]);

        // kimenethez
        pid = getpid();

        // szulo elso send-jere varni
        len = read(fd[P2_READ], &helyszin, sizeof(helyszin));
        if (len < 0)
        {
            perror("Gyerek nem tudott olvasni.\n");
            exit(EXIT_FAILURE);
        }
        else if (len == 0)
        {
            fprintf(stderr, "Gyerek EOF-ot olvasott.\n");
        }
        else
        {
            // Kiirja amit olvasott
            printf("Gyerek(%d): ezt kapta: %s\n", pid, helyszin);

			if(strcmp(helyszin,"Kiskunfelegyhaza") == 0)
			{
				strcpy(gyerekOk,"OK");
			}
			else
			{
				strcpy(gyerekOk, "nemOk");
			}

            printf("Gyerek(%d): visszakuldi %s -t\n", pid, gyerekOk);
            if (write(fd[P2_WRITE], &gyerekOk, sizeof(gyerekOk)) < 0)
            {
                perror("Gyerek nem tudott visszakuldeni.\n");
                exit(EXIT_FAILURE);
            }
        }
		
		
		//szulo size send-jere var
		
		len2 = read(fd[P2_READ], &size, sizeof(size));
		if(len2 < 0)
		{
			perror("Gyerek nem tudott olvasni.\n");
            exit(EXIT_FAILURE);
		}
		else if( len2 == 0)
		{
            fprintf(stderr, "Gyerek EOF-ot olvasott.\n");
		}
		else
		{
			printf("A beerkezett size ertek: %d\n", size);
		}
		
		//szulo lista send-jere varni
		for(k = 0; k < 4; k++)
		{
		ken1 = read(fd[P2_READ], &lista[k].vezetek, sizeof(lista[k].vezetek));
		ken2 = read(fd[P2_READ], &lista[k].kereszt, sizeof(lista[k].kereszt));
		}
        if (ken1 < 0 || ken2 < 0)
        {
            perror("Gyerek nem tudott olvasni.\n");
            exit(EXIT_FAILURE);
        }
        else if (ken2 == 0 || ken1 == 0)
        {
            fprintf(stderr, "Gyerek EOF-ot olvasott.\n");
        }
        else
        {
            // Kiirja amit olvasott
			for (k = 0; k < 4; k++)
			{
				if(lista[k].vezetek[0] != '\0')
				{
				printf("Gyerek(%d): ezt kapta: %s %s\n", pid,
				lista[k].vezetek, lista[k].kereszt );
				}
			}
			
			absent = 0;
			
			for(k = 0; k < 4; k++)
			{
				strcpy(jelen[k].vezetek, lista[k].vezetek);
				strcpy(jelen[k].kereszt, lista[k].kereszt);
				temp = getRan();
				printf("random temp = %d\n", temp);
				if(temp >= 11)
				{
					jelen[k].jelenlet = 0;
				}
				else
				{
					jelen[k].jelenlet = 1;
					absent++;
				}
			}
			
			sleep(1);
			
			tempavg = 0.0;
			avg = 100.0;
			for(k = 0; k < 4; k++)
			{
				if(jelen[k].jelenlet == 1) tempavg = tempavg + 1.0;
				else {}
			}
			avg = 100.0 - (100.0 * (tempavg / 4.0) );
			printf("childAVG: %f\n", avg);
			
			//atkuldi a hianyzok szamat
			printf("Gyerek(%d): visszakuldi %d-t\n", pid, absent);
			if((write(fd[P2_WRITE], &absent, sizeof(absent))) < 0)
			{
				perror("Gyerek nem tudott visszakuldeni.\n");
						exit(EXIT_FAILURE);
			}
			
			
			
			for(k = 0; k < 4; k++)
			{
				if(jelen[k].jelenlet == 1)
				{
					 printf("Gyerek(%d): visszakuldi %s %s -t\n",
					 pid, jelen[k].vezetek, jelen[k].kereszt);
					 
					if ((write(fd[P2_WRITE], &jelen[k].vezetek,
					sizeof(jelen[k].vezetek)) < 0) ||
					(write(fd[P2_WRITE], &jelen[k].kereszt,
					sizeof(jelen[k].kereszt)) < 0) 
					)
					{
						perror("Gyerek nem tudott visszakuldeni.\n");
						exit(EXIT_FAILURE);
					}
				}
								
		
			}
				printf("Gyerek(%d): visszakuldi %f szazalekot\n", avg);
				if(write(fd[P2_WRITE], &avg, sizeof(avg)) < 0)
				{
					perror("Gyerek nem tudott visszakuldeni.\n");
						exit(EXIT_FAILURE);
				}
			
        }
		
	
		close(fd[P2_READ]);
        close(fd[P2_WRITE]);

        return EXIT_SUCCESS;
	}

	close(fd[P2_READ]);
    close(fd[P2_WRITE]);
	
	//kimenethez
	pid = getpid();
	
	// uzenet a gyereknek
    strcpy(helyszin, "Kiskunfelegyhaza");
    printf("Szulo(%d): %s -t  kuld a gyereknek.\n", pid, helyszin);
    if (write(fd[P1_WRITE], &helyszin, sizeof(helyszin)) != sizeof(helyszin))
    {
        perror("Szulo kuldesi hiba.\n");
        exit(EXIT_FAILURE);
    }

    // valaszra var
    len = read(fd[P1_READ], &gyerekOk, sizeof(gyerekOk));
    if (len < 0)
    {
        perror("Szulo valaszolvasasi hiba.\n");
        exit(EXIT_FAILURE);
    }
    else if (len == 0)
    {
        fprintf(stderr, "Szulo(%d): EOF-t kapott vissza.\n", pid);
    }
    else
    {
        // kiirja a valaszt
        printf("Szulo(%d) %s -t kapott vissza.\n", pid, gyerekOk);
    }
	
	strcpy(lista[0].vezetek, "Petofi");
	strcpy(lista[0].kereszt, "Sandor");
	strcpy(lista[1].vezetek, "Mora");
	strcpy(lista[1].kereszt, "Ferenc");
	strcpy(lista[2].vezetek, "Kossuth");
	strcpy(lista[2].kereszt, "Lajos");
	strcpy(lista[3].vezetek, "Juhasz");
	strcpy(lista[3].kereszt, "Gyula");
	
	//size_t array_size = *(&lista + 1) - lista;
	
	size =  4; //array_size; //4; //sizeof(lista) / sizeof(*lista);
	
	if ( strcmp(gyerekOk, "OK") == 0)
	{
		
		//atkuldi a lista hosszat
		
		printf("Szulo(%d): %d -t kuld a gyereknek.\n", pid, size);
		if((write(fd[P1_WRITE], &size, sizeof(size)) != sizeof(size)))
		{
			perror("Szulo kuldesi hiba.\n");
			exit(EXIT_FAILURE);
		}
	
		for(k = 0; k < 4; k++)
		{
			if(jelen[k].vezetek[0] == '\0')
			{
				printf("Szulo(%d): %s %s -t  kuld a gyereknek.\n",
				pid, lista[k].vezetek, lista[k].kereszt);
			}
				if ((write(fd[P1_WRITE], &lista[k].vezetek,
				sizeof(lista[k].vezetek)) != sizeof(lista[k].vezetek)) ||
				(write(fd[P1_WRITE], &lista[k].kereszt, sizeof(lista[k].kereszt))
				!= sizeof(lista[k].kereszt)))
				{
					perror("Szulo kuldesi hiba.\n");
					exit(EXIT_FAILURE);
				}
		}
	}
	else{return;}
	
	wait(NULL);
	
	//varja a jelenlevok listajat es a szazalekot
	//avg = 100.0;
	ken1 = read(fd[P1_READ], &absent, sizeof(absent));
	printf("Szulo(%d): ennyien nem voltak: %d\n", pid, absent);
	
	for(k = 0; k < absent; k++)
		{
		ken1 = read(fd[P1_READ], &jelen[k].vezetek, sizeof(jelen[k].vezetek));
		ken2 = read(fd[P1_READ], &jelen[k].kereszt, sizeof(jelen[k].kereszt));
		}
		sleep(1);
		ken3 = read(fd[P1_READ], &avg, sizeof(avg));
		
	
	for (k = 0; k < 4; k++)
		{
			if(jelen[k].vezetek[0] == '\0')
			{
			
			}
			else
			{
				printf("Szulo(%d): nem volt jelen: %s %s\n", pid,
				jelen[k].vezetek, jelen[k].kereszt );
			}
		}
		
		//printf("%f%\n",avg);
		
		printf("Az esemeny sikeressege: %f% \n" , avg);
		
	

    close(fd[P1_READ]);
    close(fd[P1_WRITE]);

	
	return EXIT_SUCCESS;
}