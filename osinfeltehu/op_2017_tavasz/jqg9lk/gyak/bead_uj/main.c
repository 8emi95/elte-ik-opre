
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>

#define N 512
#define K 250
#define S 7

#pragma warning( push )
#pragma warning( disable : 4101)


struct question {
    char* created;
    char* s_question;
    char* a_answer;
    char* b_answer;
    char* c_answer;
    char* d_answer;
    char* finalized;
};

typedef struct question questionnaire;

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
}


char* timeString()
{
    time_t timer;
    char buffer[26];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S\n", tm_info);

    return buffer;
}

int checker(char input[], char check[], int meddig)
{
    int i, result = 1;
    for (i = 0; (input[i] != '\0' && check[i] != '\0') || i < meddig; i++) {
        if (input[i] != check[i]) {
            result = 0;
            break;
        }
    }
    return result;
}

void questionnaire_create(questionnaire* questionnaires)
{
    int szamlalo = 0;
    int j = 0;
    double i = 0;

    do {
        printf("Mennyi kerdest szeretne felvinni: ");
        scanf("%lf", &i);
        j = (int)i;
        if (i < 0 || j != i)
            printf("Pozitiv egesz szamot kerek!\n");
    } while (i < 0 || j != i);


    FILE* fp;
    fp = fopen("questionnaires.txt", "w+");
    time_t current_time;
    char* c_time_string;
    c_time_string = (char*)malloc(sizeof(timeString()));
    int db = 0;

    for (szamlalo = 0; szamlalo < i; szamlalo++) {
        //strcpy(c_time_string, timeString());
        strftime(c_time_string, 20, "%Y-%m-%d %H:%M:%S", localtime(&current_time));


        if (szamlalo == 0)
            gets(&(questionnaires->created));
        fputs(c_time_string, fp);
        fputs("\n", fp);

        printf("kerdes: ");
        gets(&(questionnaires->s_question));
        fprintf(fp, "kerdes: ");
        fputs(&(questionnaires->s_question), fp);
        fputs("\n", fp);

        do {
            printf("Hany valaszt szeretne biztositani ? (min:2 | max:4)");
            scanf("%i", &db);
        } while (db < 2 || db > 4);

        switch (db) {
        case 2:
            gets(&(questionnaires->created));

            printf("    (a) - valasz: ");
            gets(&(questionnaires->a_answer));
            fprintf(fp, "\t(a) - ");
            fputs(&(questionnaires->a_answer), fp);
            fputs("\n", fp);

            printf("    (b) - valasz: ");
            gets(&(questionnaires->b_answer));
            fprintf(fp, "\t(b) - ");
            fputs(&(questionnaires->b_answer), fp);
            fputs("\n", fp);

            char y = 'Y';
            char n = 'N';
            char c;
            int b = 2;
            do {
                
                printf("Veglegesiti? ( Y/N ) ");
                gets(&(questionnaires->finalized));
                c = (questionnaires->finalized);
                b = c != n;
                b += c != y;
            } while (b == 2);
            fputs(&(questionnaires->finalized), fp);
            fputs("\n", fp);
            break;
        case 3:
            gets(&(questionnaires->created));

            printf("    (a) - valasz: ");
            gets(&(questionnaires->a_answer));
            fprintf(fp, "\t(a) - ");
            fputs(&(questionnaires->a_answer), fp);
            fputs("\n", fp);

            printf("    (b) - valasz: ");
            gets(&(questionnaires->b_answer));
            fprintf(fp, "\t(b) - ");
            fputs(&(questionnaires->b_answer), fp);
            fputs("\n", fp);

            printf("    (c) - valasz: ");
            gets(&(questionnaires->c_answer));
            fprintf(fp, "\t(c) - ");
            fputs(&(questionnaires->c_answer), fp);
            fputs("\n", fp);

            y = 'Y';
            n = 'N';
            b = 2;
            do {    
                printf("Veglegesiti? (Y/N) ");
                gets(&(questionnaires->finalized));
                c = (questionnaires->finalized);
                b = c != n;
                b += c != y;
            } while (b == 2);
            fputs(&(questionnaires->finalized), fp);
            fputs("\n", fp);

            break;
        case 4:
            gets(&(questionnaires->created));

            printf("    (a) - valasz: ");
            gets(&(questionnaires->a_answer));
            fprintf(fp, "\t(a) - ");
            fputs(&(questionnaires->a_answer), fp);
            fputs("\n", fp);

            printf("    (b) - valasz: ");
            gets(&(questionnaires->b_answer));
            fprintf(fp, "\t(b) - ");
            fputs(&(questionnaires->b_answer), fp);
            fputs("\n", fp);

            printf("    (c) - valasz: ");
            gets(&(questionnaires->c_answer));
            fprintf(fp, "\t(c) - ");
            fputs(&(questionnaires->c_answer), fp);
            fputs("\n", fp);

            printf("    (d) - valasz: ");
            gets(&(questionnaires->d_answer));
            fprintf(fp, "\t(d) - ");
            fputs(&(questionnaires->d_answer), fp);
            fputs("\n", fp);

            y = 'Y';
            n = 'N';
            b = 2;
            do {
                printf("Veglegesiti? (Y/N) ");
                gets(&(questionnaires->finalized));
                c = (questionnaires->finalized);
                b = c != n;
                b += c != y;
            } while (b == 2);
            fputs(&(questionnaires->finalized), fp);
            fputs("\n", fp);

            break;
        }
    }
	free(c_time_string);
    fclose(fp);
}

int questionnaire_read(questionnaire* questionnaires)
{
    FILE* fp;
    fp = fopen("questionnaires.txt", "r");
    char line[N];
    int i = 0;
    int j = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "kerdes: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);

            ++i;
        }
        else {
            printf("%s", line);
        }
        ++j;
    }
    fclose(fp);

    printf("Nyomjon [Enter] a folytatashoz.\n");
        while(getchar()!='\n'); // option TWO to clean stdin
        getchar(); // wait for ENTER 

    return j;
}

void question_answer_add(questionnaire* questionnaires)
{
    FILE* fp;
    fp = fopen("questionnaires.txt", "a");
    time_t current_time;
    char* c_time_string;
    c_time_string = (char*)malloc(sizeof(timeString()));
    int db = 0;

    strftime(c_time_string, 20, "%Y-%m-%d %H:%M:%S", localtime(&current_time));
    //strcpy(c_time_string, timeString());
    //printf("created : %s", c_time_string);
    gets(&(questionnaires->created));
    fputs(c_time_string, fp);
    fputs("\n", fp);

    printf("kerdes: ");
    gets(&(questionnaires->s_question));
    fprintf(fp, "kerdes: ");
    fputs(&(questionnaires->s_question), fp);
    fputs("\n", fp);

    do {
        printf("Hany valaszt szeretne biztositani ? (min:2 | max:4)");
        scanf("%i", &db);
    } while (db < 2 || db > 4);

    switch (db) {
    case 2:
        gets(&(questionnaires->created));

        printf("    (a) - valasz: ");
        gets(&(questionnaires->a_answer));
        fprintf(fp, "\t(a) - ");
        fputs(&(questionnaires->a_answer), fp);
        fputs("\n", fp);

        printf("    (b) - valasz: ");
        gets(&(questionnaires->b_answer));
        fprintf(fp, "\t(b) - ");
        fputs(&(questionnaires->b_answer), fp);
        fputs("\n", fp);

        char y = 'Y';
        char n = 'N';
        char c;
        int b = 2;
        do {
            printf("Veglegesiti? (Y/N) ");
            gets(&(questionnaires->finalized));
            c = (questionnaires->finalized);
            b = c != n;
            b += c != y;
        } while (b == 2);
        fputs(&(questionnaires->finalized), fp);
        fputs("\n", fp);

        break;

    case 3:
        gets(&(questionnaires->created));

        printf("    (a) - valasz: ");
        gets(&(questionnaires->a_answer));
        fprintf(fp, "\t(a) - ");
        fputs(&(questionnaires->a_answer), fp);
        fputs("\n", fp);

        printf("    (b) - valasz: ");
        gets(&(questionnaires->b_answer));
        fprintf(fp, "\t(b) - ");
        fputs(&(questionnaires->b_answer), fp);
        fputs("\n", fp);

        printf("    (c) - valasz: ");
        gets(&(questionnaires->c_answer));
        fprintf(fp, "\t(c) - ");
        fputs(&(questionnaires->c_answer), fp);
        fputs("\n", fp);

        y = 'Y';
        n = 'N';
        b = 2;
        do {
            printf("Veglegesiti? (Y/N) ");
            gets(&(questionnaires->finalized));
            c = (questionnaires->finalized);
            b = c != n;
            b += c != y;
        } while (b == 2);
        fputs(&(questionnaires->finalized), fp);
        fputs("\n", fp);

        break;
    case 4:
        gets(&(questionnaires->created));

        printf("    (a) - valasz: ");
        gets(&(questionnaires->a_answer));
        fprintf(fp, "\t(a) - ");
        fputs(&(questionnaires->a_answer), fp);
        fputs("\n", fp);

        printf("    (b) - valasz: ");
        gets(&(questionnaires->b_answer));
        fprintf(fp, "\t(b) - ");
        fputs(&(questionnaires->b_answer), fp);
        fputs("\n", fp);

        printf("    (c) - valasz: ");
        gets(&(questionnaires->c_answer));
        fprintf(fp, "\t(c) - ");
        fputs(&(questionnaires->c_answer), fp);
        fputs("\n", fp);

        printf("    (d) - valasz: ");
        gets(&(questionnaires->d_answer));
        fprintf(fp, "\t(d) - ");
        fputs(&(questionnaires->d_answer), fp);
        fputs("\n", fp);

        y = 'Y';
        n = 'N';
        b = 2;
        do {
            printf("Veglegesiti? (Y/N) ");
            gets(&(questionnaires->finalized));
            c = (questionnaires->finalized);
            b = c != n;
            b += c != y;
        } while (b == 2);
        fputs(&(questionnaires->finalized), fp);
        fputs("\n", fp);

        break;
    }
	free(c_time_string);
    fclose(fp);
}

void question_modify(questionnaire* questionnaires)
{
    FILE* fp;
    fp = fopen("questionnaires.txt", "r");
    char line[N];
    int i_kerdes[K]; 
    int i = 0;
    int db = 1; 
    for (i = 0; i < K; ++i) {
        i_kerdes[i] = 0;
    }
    i = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "kerdes: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);

            ++i;
        }
        i_kerdes[i] = ++db;
    }

    long answer;
    do {
        printf("valasz: ");
        scanf("%i", &answer);
        if (answer < 0 || answer > i - 1)
            printf("A kerdes sorszamaval megegyezo szamot adja meg!\n");
    } while (answer < 0 || answer > i - 1);


    char s_question[N];
    printf("Kerem adja meg az uj kerdest: ");
    gets(&s_question);
    gets(&s_question);
    fclose(fp);

    fp = fopen("questionnaires.txt", "r");
    FILE* fp_w;
    fp_w = fopen("kerdoivek_2.txt", "w");

    line[N];
    int line_counter = 1;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        if (checker(line, "kerdes: ", 7) > 0 && line_counter == i_kerdes[answer]) {
            fputs("kerdes: ", fp_w);
            fputs(&s_question, fp_w);
            fputs("\n", fp_w);
        }
        else {
            fputs(&line, fp_w);
        }
        line_counter++;
    }

    fclose(fp_w);
    fclose(fp);

    remove("questionnaires.txt");
    rename("kerdoivek_2.txt", "questionnaires.txt");
}

void answer_modify(questionnaire* questionnaires)
{
    FILE* fp;
    fp = fopen("questionnaires.txt", "r");
    char line[N];
    int i_kerdes[K]; 
    int valaszok_szama[K];
    int i = 0;
    int db = 1; 
    for (i = 0; i < K; ++i) {
        valaszok_szama[i] = 0;
        i_kerdes[i] = 0;
    }
    i = 0;
    int valaszok = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "kerdes: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);
            
            ++i;
            valaszok = 1;
        }
        if (checker(line, "\t(", 2) > 0) {
            printf("%s", line);
            valaszok_szama[i - 1] = valaszok++;
        }
        i_kerdes[i] = ++db;
    }

    int valasz_szama = 0;
    int kerdes_szama;
    char valasz_jele;
    do {
        printf("Valtoztatas melyik kerdesnek melyik pontjat: \n");
        printf("    kerdes: ");
        scanf("%i", &kerdes_szama);
        int kerdes_szam = kerdes_szama;

        printf("    valasz: ");
        scanf("%c", &valasz_jele);
        scanf("%c", &valasz_jele);

        if (kerdes_szama < 0 || kerdes_szama > db - 1) {
            printf("A kerdes sorszamaval megegyezo szamot adja meg!\n");
            valasz_szama = 0;
        }
        else {

            switch (valaszok_szama[kerdes_szama]) {
            case 2:
                if (valasz_jele == 'a')
                    valasz_szama = 1;
                if (valasz_jele == 'b')
                    valasz_szama = 2;
                break;
            case 3:
                if (valasz_jele == 'a')
                    valasz_szama = 1;
                if (valasz_jele == 'b')
                    valasz_szama = 2;
                if (valasz_jele == 'c')
                    valasz_szama = 3;
                break;
            case 4:
                if (valasz_jele == 'a')
                    valasz_szama = 1;
                if (valasz_jele == 'b')
                    valasz_szama = 2;
                if (valasz_jele == 'c')
                    valasz_szama = 3;
                if (valasz_jele == 'd')
                    valasz_szama = 4;
                break;
            }
            if (valasz_szama < 1) {
                printf("A valaszok mellet levo betuk kozul valaszon!");
                valasz_szama = 0;
            }
        }
    } while (valasz_szama == 0);

    char answer[N];
    printf("Kerem adja meg az uj valaszt: ");
    gets(&answer);
    gets(&answer);

    fclose(fp);

    fp = fopen("questionnaires.txt", "r");
    FILE* fp_w;
    fp_w = fopen("kerdoivek_2.txt", "w+");

    int line_counter = 1;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        if (line_counter == i_kerdes[kerdes_szama] + valasz_szama) {
            fprintf(fp_w, "	(%c) - %s\n", valasz_jele, &answer);
        }
        else {
            fputs(&line, fp_w);
        }
        line_counter++;
    }

    fclose(fp_w);
    fclose(fp);

    remove("questionnaires.txt");
    rename("kerdoivek_2.txt", "questionnaires.txt");
}

void question_remove(questionnaire* questionnaires)
{
    FILE* fp;
    fp = fopen("questionnaires.txt", "r");
    char line[N];
    int i_kerdes[K]; 
    int valaszok_szama[K];
    int i = 0;
    int db = 1;
    for (i = 0; i < K; ++i) {
        valaszok_szama[i] = 0;
        i_kerdes[i] = 0;
    }
    i = 0;
    int valaszok = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "kerdes: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);
           
            ++i;
            valaszok = 1;
        }
        if (checker(line, "\t(", 2) > 0) {
          
            valaszok_szama[i - 1] = valaszok++;
        }
        i_kerdes[i] = ++db;
    }

    int valasz_szama = 1;
    int kerdes_szama;
    char valasz_jele;
    do {
        printf("Valtoztatas melyik kerdesnek melyik pontjat: \n");
        printf("    kerdes: ");
        scanf("%i", &kerdes_szama);
        int kerdes_szam = kerdes_szama;
        if (kerdes_szama < 0 || kerdes_szama > db - 1) {
            printf("A kerdes sorszamaval megegyezo szamot adja meg!\n");
            valasz_szama = 0;
        }
    } while (valasz_szama == 0);

    fclose(fp);

    fp = fopen("questionnaires.txt", "r");
    FILE* fp_w;
    fp_w = fopen("kerdoivek_2.txt", "w+");

    int line_counter = 1;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        if (line_counter > i_kerdes[kerdes_szama] - 2 && line_counter < i_kerdes[kerdes_szama] + valaszok_szama[kerdes_szama] + 2) {
        }
        else {
            fputs(&line, fp_w);
        }
        line_counter++;
    }

    fclose(fp_w);
    fclose(fp);

    remove("questionnaires.txt");
    rename("kerdoivek_2.txt", "questionnaires.txt");
}

void questions_finalize(questionnaire* questionnaires)
{
    FILE* fp;
    fp = fopen("questionnaires.txt", "r");
    char line[N];
    int i_kerdes[K]; 
    int valaszok_szama[K];
    int i = 0;
    int db = 1; 
    for (i = 0; i < K; ++i) {
        i_kerdes[i] = 0;
    }
    i = 0;
    int valaszok = 0;
    while (fgets(line, N, fp) != NULL) {
        if (checker(line, "kerdes: ", 7) > 0) {
            printf("%i. ", i);
            printf("%s", line);

            ++i;
            valaszok = 0;
        }
        else if (checker(line, "N", 1) > 0 || checker(line, "Y", 1) > 0) {
            printf("Vegleges: %s", line);
        }
        else if (checker(line, "\t(", 2) > 0) {
            printf("%s", line);
            valaszok_szama[i - 1] = ++valaszok;
        }
        i_kerdes[i] = ++db;
    }

    int sorszam_valasz = 0;
    do {
        printf("valasz: ");
        scanf("%i", &sorszam_valasz);
        if (sorszam_valasz < 0 || sorszam_valasz > i - 1)
            printf("A kerdes sorszamaval megegyezo szamot adja meg!\n");
    } while (sorszam_valasz < 0 || sorszam_valasz > i - 1);


    int sorszam = sorszam_valasz;
    char answer[1];
    char y = 'Y';
    char n = 'N';
    char c[1];
    do {
        printf("Kerem csak az alabbi betuk kozul valaszon: Y / N!\n");
        printf(
            "Kerem adja meg hogy ez a kerdes szerepeljen-e a vegleges kerdoiven? "
            "(Y/N)");
        scanf("%s", &answer);
        c[0] = answer[0];
    } while (checker(c, "Y ", 1) > 0 || checker(c, "N ", 1) > 0);

    fclose(fp);

    fp = fopen("questionnaires.txt", "r");
    FILE* fp_w;
    fp_w = fopen("kerdoivek_2.txt", "w");

    line[N];
    int line_counter = 1;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        // if (strcmp(line,"kerdes: "))
        if ((checker(line, "Y", 1) > 0 || checker(line, "N", 1) > 0) && line_counter == (i_kerdes[sorszam] + valaszok_szama[sorszam] + 1)) {
            fprintf(fp_w, "%s\n", &answer);
        }
        else {
            fputs(&line, fp_w);
        }
        line_counter++;
    }

    fclose(fp_w);
    fclose(fp);

    remove("questionnaires.txt");
    rename("kerdoivek_2.txt", "questionnaires.txt");
	
	
}

int final_questionnaire_create(questionnaire* questionnaires)
{
    FILE* fp;
    fp = fopen("questionnaires.txt", "r");
    int db = 1; 
    int valaszok = 0;
    char line[N];
    while (fgets(line, N, fp) != NULL) {
        ++db;
    }
    fclose(fp);

    fp = fopen("questionnaires.txt", "r");
    FILE* fp_w;
    fp_w = fopen("vegleges_kerdoiv.txt", "w+");


    int line_counter = 1;

    char finalized[S][K];
    int i = 0;
    int j = 0;
	int kerdes_szam = 0;
    while (fgets(line, N, fp) != NULL && line_counter < db) {
        strcpy(finalized[i], line);
        if (checker(line, "Y", 1) > 0) {
            for (j = 0; j < i; ++j) {
                fputs(&finalized[j], fp_w);
                // printf("i : %s",finalized[j]);
            }
			kerdes_szam++;
            i = 0;
        }
        else if (checker(line, "N", 1) > 0) {
			
            i = 0;
        }
        else {
            i++;
            line_counter++;
        }
    }

    fclose(fp_w);
    fclose(fp);
	printf("DB kerdes : %i \n", kerdes_szam);
	
	return kerdes_szam;
}

void kivalaszottt_kerdesek(int q1, int q2, int q3) {
	FILE* fp;
	fp = fopen("vegleges_kerdoiv.txt", "r");
    FILE* fp_w;
	fp_w = fopen("kivalaszott_kerdesek_2.txt", "w+");
	char line[N];
	int i = 0;
	while (fgets(line, N, fp) != NULL) {
		if (checker(line, "2", 1) > 0) ++i;
		if ( q1 == i) {
			fputs(&line, fp_w);
			
		}
		if ( q2 == i) {
			fputs(&line, fp_w);
		}
		if ( q3 == i) {
			fputs(&line, fp_w);
		}
		
	}
	fclose(fp_w);
    fclose(fp);
}

void kivalaszottt_kerdes(int q1, char s_question[]) {
	FILE* fp;
	fp = fopen("vegleges_kerdoiv.txt", "r");
	char line[N];
	int i = 0;
	while (fgets(line, N, fp) != NULL) {
		if ( q1 == i) {
			strcpy(s_question, line);
			break;
		}
		if (checker(line, "2", 1) > 0) ++i;
	}
    fclose(fp);
}

void people_answer(int question, int answer[]){
	

	FILE* fp_w;
	fp_w = fopen("kivalaszott_kerdesek_2.txt", "r");
	char line[N];
	int i = 0; 
	int db = -1; 
	while (fgets(line, N, fp_w) != NULL) {
		if (question < i) break;
		if (checker(line, "2", 1) > 0){
			++i;
		}else if( question == i) {
			db++;
		}
		
	}
	

	for ( i = 0; i < db; ++i){
		if (answer[i] == -1) answer[i] = 0;
	}
	

	srand(time(NULL));
	int p = rand()%db;
	answer[p]++;
	
	
	fclose(fp_w);
}


int main(int argc, char* argv[])
{
	
	unlink("fifo.ftc");
	int fd=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); 
	if (fd==-1)
    {
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
    }
	unlink("fifo2.ftc");
	int fd2=mkfifo("fifo2.ftc", S_IRUSR|S_IWUSR ); 
	if (fd2==-1)
    {
		printf("Error number: %i",errno);
		exit(EXIT_FAILURE);
    }
	

	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);
	char sz[100];
	
	pid_t pid = fork();
	if (pid == 0) { 

		pause(); 

		printf("Parent is ready! But Men is not!\n");
		

		sleep(1);
		

		printf("MEN sends the sign that I arrived!\n",SIGUSR1);
		kill(getppid(),SIGUSR1);
		

		int q1 = 0;
		int q2 = 0;
		int q3 = 0;
		

		fd=open("fifo.ftc",O_RDONLY);
		read(fd,&q1,sizeof(q1));

		read(fd,&q2,sizeof(q2));

		read(fd,&q3,sizeof(q3));

		kivalaszottt_kerdesek(q1,q2,q3);
		

		srand(time(NULL));
		int people = rand()%10 +10;
		printf("MEN -> Ennyi emberrel fogjuk ki töltettni : %i \n",people);
		

		int answer_1[4] = {-1,-1,-1,-1};
		int answer_2[4] = {-1,-1,-1,-1};
		int answer_3[4] = {-1,-1,-1,-1};
		

		int k = 0;
		while (people > k){
			printf("%i ember megkerdezese! \n",k+1);
			people_answer(1, answer_1);
			people_answer(2, answer_2);
			people_answer(3, answer_3);
			++k;
		}

		
		int j;
		
	
		printf("Elso kerdesre adott valasz :\n");
		for (j = 0; j < 4; ++j){
			if (answer_1[j] != -1)
				printf("%i : %d\n",j,answer_1[j]);
		}
		printf("\n");
		printf("Masodik kerdesre adott valasz :\n");
		for (j = 0; j < 4; ++j){
			if(answer_2[j] != -1)
				printf("%i : %d\n",j,answer_2[j]);
		}
		printf("\n");
		printf("Harmadik kerdesre adott valasz :\n");
		for (j = 0; j < 4; ++j){
			if(answer_3[j] != -1)
				printf("%i : %d\n",j,answer_3[j]);
		}
		

		close(fd);
		
		
		fd2=open("fifo2.ftc",O_WRONLY);


		for (j = 0; j < 4; ++j){
		
				write(fd2,&answer_1[j],sizeof(answer_1[j]));
				
		}
		for (j = 0; j < 4; ++j){
			
				write(fd2,&answer_2[j],sizeof(answer_2[j]));
				
		}
		for (j = 0; j < 4; ++j){
		
				write(fd2,&answer_3[j],sizeof(answer_3[j]));
				
		}
		close(fd2);
		printf("Kerdezobiztos process ended\n");

	}else{
		
		int answer;
		questionnaire* questionnaires = (questionnaire*)malloc(K * sizeof(questionnaire));
		questionnaires->created = (char*)malloc(N * sizeof(questionnaire));
		questionnaires->s_question = (char*)malloc(N * sizeof(questionnaire));
		questionnaires->a_answer = (char*)malloc(N * sizeof(questionnaire));
		questionnaires->b_answer = (char*)malloc(N * sizeof(questionnaire));
		questionnaires->c_answer = (char*)malloc(N * sizeof(questionnaire));
		questionnaires->d_answer = (char*)malloc(N * sizeof(questionnaire));
		questionnaires->finalized = (char*)malloc(N * sizeof(questionnaire));
		
		int question_number = -1;
		int v = 1;
		while (v) {
			system("clear");
			printf("MLSZ Kerdoiv Kezelo - JQG9LK\n");
			printf("-----------------------------------------------\n");
			printf("1 - Kerdoiv letrehozasa \n");
			printf("2 - Kerdoiv listazasa \n");
			printf("3 - Uj kerdes es valaszok felvetele \n");
			printf("4 - Meglevo kerdes valtoztatas \n");
			printf("5 - Meglevo valasz valtoztatas \n");
			printf("6 - Kerdes torlese \n");
			printf("7 - Kerdesek veglegesitese! \n");
			printf("8 - Vegleges kerdoiv letrehozasa! \n");
			printf("0 - Kilepes \n");
			printf("Valasztas: ");

			scanf("%i", &answer);
			switch (answer) {
			case 1:
				system("clear");
				questionnaire_create(questionnaires);
				break;
			case 2:
				system("clear");
				questionnaire_read(questionnaires);
				break;
			case 3:
				system("clear");
				question_answer_add(questionnaires);
				break;
			case 4:
				system("clear");
				question_modify(questionnaires);
				break;
			case 5:
				system("clear");
				answer_modify(questionnaires);
				break;
			case 6:
				system("clear");
				question_remove(questionnaires);
				break;
			case 7:
				system("clear");
				questions_finalize(questionnaires);
				break;
			case 8:
				system("clear");
				question_number = final_questionnaire_create(questionnaires);
				printf("Kapott szam : %i\n",question_number);
				v = 0;
				break;
			case 0:
				system("clear");
				return;
			}
			//system("clear");
		}
		/*free(questionnaires->created);
		free(questionnaires->s_question);
		free(questionnaires->a_answer);
		free(questionnaires->b_answer);
		free(questionnaires->c_answer);
		free(questionnaires->d_answer);
		free(questionnaires->finalized);
		free(questionnaires); */
		

		printf("PARENT sends the message that I'm ready!\n",SIGUSR2);
		kill(pid,SIGUSR2);
		

		pause();
		printf("Men arrived! So Parent sends three question\n",SIGUSR1);
		

		fd=open("fifo.ftc",O_WRONLY);
		

		srand(time(NULL));
		int q_1 = rand()%question_number + 1;

		int q_2 = check(q_1,-1,-1, question_number);
		int q_3= check(q_1,q_2,-2, question_number);
		
		

        write(fd,&q_1,sizeof(q_1));
        write(fd,&q_2,sizeof(q_2));
        write(fd,&q_3,sizeof(q_3));
        

		close(fd);
		
		fd2=open("fifo2.ftc",O_RDONLY);
		char a[4] = {'a','b','c','d'};
		int b,j;
		printf("Elso kerdesre adott valasz :\n");
		char s_question[N];
		kivalaszottt_kerdes(q_1, s_question);
		printf("%s ",s_question);

		for (j = 0; j < 4; ++j){
			read(fd2,&b,sizeof(b));
			printf("%c : %i\n",a[j],b);
		}
		printf("Masodik kerdesre adott valasz :\n");
		kivalaszottt_kerdes(q_2, s_question);
		printf("%s ",s_question);
		for (j = 0; j < 4; ++j){
			read(fd2,&b,sizeof(b));
			printf("%c : %i\n",a[j],b);
		}
		printf("Harmadik kerdesre adott valasz :\n");
		kivalaszottt_kerdes(q_3, s_question);
		printf("%s ",s_question);
		for (j = 0; j < 4; ++j){
			read(fd2,&b,sizeof(b));
			printf("%c : %i\n",a[j],b);
		}
		
		close(fd2);	
		printf("\nMLSZ process ended\n");
	}

    return 0;
}

int check(int q1, int q2, int q3, int question_number) {
	if (q2 == -1){
		srand(time(NULL));
		q2 = rand()%question_number + 1;
		while(1) {
			if (q1 == q2) {
				srand(time(NULL));
				q2 = rand()%question_number + 1;
			}else{
				return q2;
			}
		}
	}else if (q3 == -2) {
		srand(time(NULL));
		q3 = rand()%question_number + 1;
		while(1) {
			if (q1 == q3 || q2 == q3) {
				srand(time(NULL));
				q3 = rand()%question_number + 1;
			}else{
				return q3;
			}
		}
	}
	
	
}