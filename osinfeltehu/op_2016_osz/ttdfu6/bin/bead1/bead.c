#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define KEP_MAGASSAG 25

int main(void)
{
	int i;
	for (i = 0; i < KEP_MAGASSAG; i++)
	{
		putchar ( '\n' );
	}

    
	int menupont;

    do
    {
    	printf(".:King of Stands:.\n");
    	printf("Valasszon az alabbi menupontok kozul!\n\n");

   		printf("[1] Vendeg jelentkezese\n"
    	   "[2] Vendegek listazasa\n"
    	   "[3] Adott vendeg adatainak torlese\n"
    	   "[4] Adott vendeg adatainak modositasa\n"
    	   "[5] Uj rendezveny inditasa\n"
    	   "[6] Adott rendezveny osszes adatanak torlese\n\n");

    	//Melyik menupontot valasztotta
    	menupont = getchar();
    	menupont = -1 * (48 - menupont);

   	} while(menupont < 1 || menupont > 6);

   	char nev[100];
   	char mail[100];
   	char azon[100];

   	char rnev[100];
   	char razon[100];

   	switch(menupont) {
    	case 1 :
    	{
    		
        	printf("Adja meg a nevet!\n");
        	scanf("%s", nev);

        	printf("Adja meg az e-mail cimet!\n");
        	scanf("%s", mail);

        	printf("Adja meg a renzezveny azonosito szamat!\n");
        	scanf("%s", azon);

        	time_t ido;
    		ido = time(NULL);

    		FILE *fp;
			fp = fopen("jelentkezok.txt", "a");
			//TODO - Space eseten atugrik egy adatot!!!
			fprintf(fp, "%s	%s	%s	%s", nev, mail, azon, ctime(&ido));
			fclose(fp);

			printf("\nSikeres jelentkezés, ");


			FILE* myfile = fopen("jelentkezok.txt", "r");
			int ch, number_of_lines = 0;

			do 
			{
				ch = fgetc(myfile);
			    if(ch == '\n')
			    {
			    	number_of_lines++;
			    }
			} while (ch != EOF);

			if(ch != '\n' && number_of_lines != 0) 
			{
			    number_of_lines++;
			}

			fclose(myfile);

			printf("On a %d. regisztralt vendegunk.\n", number_of_lines-1);

         	break;
        }
    	case 2 :
    	{
        	
    		printf("\nA jelentkezok:\n\n");
	    	static const char filename[] = "jelentkezok.txt";
	    	FILE *fp1;
	    	char c;

	    	fp1 = fopen(filename, "r");
	    	c = getc(fp1);
	    	while (c != EOF)
	        {
	        	
	        	printf("%c", c);
	        	c = getc(fp1);
	        }
	        rewind(fp1);
	        fclose(fp1);
			
         	break;
        }
        case 3 :
        {

        	printf("\nA jelentkezok:\n\n");
	    	static const char filename[] = "jelentkezok.txt";
	    	FILE *fp1, *fp2;
	    	char c;
	    	int del_line;
	    	int temp = 1;

	    	fp1 = fopen(filename, "r");
	    	c = getc(fp1);
	    	while (c != EOF)
	        {
	        	
	        	printf("%c", c);
	        	c = getc(fp1);
	        }
	        rewind(fp1);
	        printf("\n\nAdja meg a torolni kivant jelentkezo sorszamat!\n");
	        scanf("%d", &del_line);
	        if (del_line != 0)
	        {
	        	fp2 = fopen("jelentkezokmasolat.txt", "w");
	        	c = getc(fp1);
	        	//Ez az 1 sor nélkül lecsapja a string első karakterét is!!!
	        	putc(c, fp2);
	        	//
	        	while (c != EOF) 
	        	{
	          		c = getc(fp1);
	          		if (c == '\n')
	          		{
	          			temp++;
	          		}
	          		if (temp != del_line)
	          		{
	           			putc(c, fp2);
	          		}
	        	}
	        	fclose(fp1);
	        	fclose(fp2);

	        	remove(filename);
	        	rename("jelentkezokmasolat.txt", filename);

	        	printf("A %d. jelentkezo sikeresen torolve\n", del_line);
	        }
	        else
	        {
	        	printf("A 0. sort nem lehet torolni!\n1-tol kezdodik az indexeles!\n");
	        }
			
         	break;
        }
        case 4 :
        {
        	
        	printf("\nA jelentkezok:\n\n");
            static const char filename[] = "jelentkezok.txt";
            FILE *fp1, *fp2;
            char c;
            int del_line;
            int temp = 1;
            char atmenetisor[256];
            char ujnev[100];
            char ujmail[100];
            char ujazon[100];

            fp1 = fopen(filename, "r");
            c = getc(fp1);
            while (c != EOF)
            {
            
                printf("%c", c);
                c = getc(fp1);
            }

            rewind(fp1);
            printf("\n\nAdja meg a modositani kivant jelentkezo sorszamat!\n");
            scanf("%d", &del_line);
            printf("\nUj nev:");
            scanf("%s", &ujnev);
            printf("\nUj e-mail cim:");
            scanf("%s", &ujmail);
            printf("\nUj rendezveny azonosito:");
            scanf("%s", &ujazon);

            time_t ido;
    		ido = time(NULL);

            if (del_line != 0)
            {
                fp2 = fopen("jelentkezokmasolat.txt", "w");
                c = getc(fp1);
                //Ez az 1 sor nélkül lecsapja a string első karakterét is!!!
	        	putc(c, fp2);
	        	//
                while (c != EOF) 
                {
                    c = getc(fp1);
                    if (c == '\n')
                    {
                    	temp++;
                    }
                    if (temp != del_line)
                    {
                    	putc(c, fp2);
                    }
                }
                fclose(fp1);
                fclose(fp2);

                remove(filename);
                rename("jelentkezokmasolat.txt", filename);

                //Újra megnyitom és a végére teszem a módosítottat
	    		FILE *fp;
				fp = fopen("jelentkezok.txt", "a");
				//TODO - Space eseten atugrik egy adatot!!!
				fprintf(fp, "%s	%s	%s	%s", ujnev, ujmail, ujazon, ctime(&ido));
				fclose(fp);

                printf("\nA %d. jelentkezo sikeresen modositva.\n", del_line);
            }
            else
            {
                printf("A 0. sort nem lehet modositani\n1-tol kezdodik az indexeles!");
            }

         	break;
        }
        case 5 :
        {
        	
        	printf("Adja meg a rendezveny nevet!\n");
        	scanf("%s", rnev);

        	printf("\nAdja meg a renzezveny azonosito szamat!\n");
        	scanf("%s", razon);

    		FILE *fp;
			fp = fopen("rendezveny.txt", "a");
			//TODO - Space eseten atugrik egy adatot!!!
			fprintf(fp, "%s	%s\n", rnev, razon);
			fclose(fp);

			printf("\nSikeres rendezveny inditas");

         	break;
        }
        case 6 :
        {
        	
        	printf("\nA rendezvenyek:\n\n");
	    	static const char filename[] = "rendezveny.txt";
	    	FILE *fp1, *fp2;
	    	char c;
	    	int del_line;
	    	int temp = 1;

	    	fp1 = fopen(filename, "r");
	    	c = getc(fp1);
	    	while (c != EOF)
	        {
	        	
	        	printf("%c", c);
	        	c = getc(fp1);
	        }
	        rewind(fp1);
	        printf("\n\nAdja meg a torolni kivant rendezveny sorszamat!\n");
	        scanf("%d", &del_line);
	        if (del_line != 0)
	        {
	        	fp2 = fopen("rendezvenymasolat.txt", "w");
	        	c = getc(fp1);
	        	//Ez az 1 sor nélkül lecsapja a string első karakterét is!!!
	        	putc(c, fp2);
	        	//
	        	while (c != EOF) 
	        	{
	          		c = getc(fp1);
	          		if (c == '\n')
	          		{
	          			temp++;
	          		}
	          		if (temp != del_line)
	          		{
	           			putc(c, fp2);
	          		}
	        	}
	        	fclose(fp1);
	        	fclose(fp2);

	        	remove(filename);
	        	rename("rendezvenymasolat.txt", filename);

	        	printf("A %d. rendezveny sikeresen torolve\n", del_line);
	        }
	        else
	        {
	        	printf("A 0. sort nem lehet torolni!\n1-tol kezdodik az indexeles!\n");
	        }
			

         	break;
        }
   }

    printf("\n\n");
    return 0;
}