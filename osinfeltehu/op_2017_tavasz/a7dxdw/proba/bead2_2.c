/*
MAKAI JÁNOS
-----------
a 7 d x d w
-----------
TASK:
A Magyar Labdajátékok Szervezete (MLSZ) nemzeti konzultációt hirdetett a labdajátékok felvirágoztatására!
Első lépésként megalkotjuk azt a kérdéssort és hozzá kérdésenként azt a minimum kettő, 
maximum 4 választási lehetőséget amelyet majd a labdajátékok jobbítása iránt felelősséget érzők kitölthetnek. 
Miután véglegesítettük a kérdéseket az MLSZ (szülő) kérdezőbiztost(gyerek) küld a paraméterként megadott 
labdajáték centrumba, városba.
Az MLSZ véletlenszerűen kiválaszt 3 kérdést, amit csővezetéken kiküld a kérdezőbiztosnak azután, 
hogy az bejelentkezett az MLSZ-hez, hogy készen áll a feladat végrehajtására.
A kérdezőbiztos véletlenszerűen választ 10 és 20 közti konzultálni kívánó polgárt, 
akik kitöltik a kérdőívet (véletlenszerűen). Az eredményt visszaküldi csővezetéken az MLSZ-be, 
majd a kérdezőbiztos befejezi a konzultációt. Az MLSZ fogadja az eredményt és a képernyőre írja a 
konzultáció eredményét.

Készítsünk C programot, amellyel a leírt konzultáció elvégezhető.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h> 		//open,creat
#include <sys/types.h> 	//open
#include <errno.h> 		//perror, errno
#include <unistd.h> 	// for pipe()
#include <signal.h>

//A kérdések struktúrája
struct question{
	int 	id; 			
	char 	question[200];	
	char* 	answers[4];		
	int 	answers_c;		
	char	date[30]; 		
	int  	final; 			
	int		deleted;		
};

//betölti a véglegesített kérdéseket és visszaadja a számukat
int load_final_questions(struct question all_fq[100]){
	int c;
	int fc = 0; //véglegesített kérdés számláló
	FILE *file;
	file = fopen("questions.txt", "r");
	int all_q_c = 0;
	if (file) {
		//ez adja meg a kérdések számát
		fscanf (file, "%d", &all_q_c);
		if(all_q_c == EOF) {all_q_c = 0;}
		int y;
		
		for(y = 0; y < all_q_c; ++y){
			struct question q;
			int 			id;
			char* 			answers[4];
			int 			answers_c;
			int  			final;
			int 			len;
			fscanf (file, "%d", &id); 	
			fscanf (file, "%d", &len); 	//következő kérdés hosszát adja meg
			
			//olvassa a kérdést
			int x = -1;
			while ((c = getc(file)) != EOF && x < len){
				if(x != -1){ 
					q.question[x] = c;
				}
				x++;
			}
			q.question[x] = 0;
			
			//kérdések számát megadja
			fscanf (file, "%d", &answers_c); 
			
			
			int i;
			for(i = 0; i < answers_c; ++i){
				fscanf (file, "%d", &len); 
				
				//válaszokat olvas
				x = -1;
				q.answers[i] = (char*)malloc(len +1);
				while ((c = getc(file)) != EOF && x < len){
					if(x != -1){ //skipping last line
						q.answers[i][x] = c;
					}
					x++;
				}
				q.answers[i][x] = 0;
			}
			
			//dátumot olvas
			x = 0;
			while ((c = getc(file)) != EOF && x < 20){
				q.date[x] = c;
				x++;
			}	
			q.date[x] = 0;
			
			//megbézni véglegesítve van e a kérdés
			fscanf (file, "%d", &final); 
			
			q.id 		= id;
			q.answers_c = answers_c;
			q.final 	= final;
			q.deleted	= 0;
			
			//csak azokat a kérdéseket adja hozzá melyek véglegesítve vannak
			if(final){
				all_fq[fc] = q;
				fc++;
			}
		}
		
		fclose(file);
	}
	
	return fc;
}

//összes kérdést kilistázza
void write_result(struct question all_q[3], int all_q_c, int result[3][4]){
	int i;
	for(i = 0; i < all_q_c; ++i){
		if(!all_q[i].deleted){ 
			printf("ID-k: %d\n", all_q[i].id);
			printf("Kérdések:\n%s\n", all_q[i].question);
			int j;
			for(j = 0; j < all_q[i].answers_c; ++j){
				printf("%d: %s\n", j+1, all_q[i].answers[j]);
			}
			printf("VOTE RESULT: ");
			for(j = 0; j < 4 && result[i][j] != -1; ++j){
				printf("%d: %d\t", j+1, result[i][j]);
			}			
			printf("\n____________________(következő kérdés)_________________________\n");
		}
	}
}

void vote(struct question selected[3], int result[3][4]){
	int voterc = rand()%11 + 10; //szavazók számát adja meg ami 10 és 20 közé eshet
	
	
	int i, j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < 4; j++){
			if(j < selected[i].answers_c) result[i][j] = 0;
			else result[i][j] = -1;
		}
	}
	
	//szavazás
	for(i = 0; i < 3; i++){
		for(j = 0; j < voterc; j++){
			int r =  rand()%selected[i].answers_c;
			result[i][r]++;
		}
	}
}


//3 kérdést kiválaszt random
void select_random(struct question selected[3], struct question all_fq[100], int all_fq_c){
	int sc = 0; //selected counter
	int selected_i[3];
	while (sc != 3){
		int i = rand()%all_fq_c;
		
		int j, f = 0; 
		for(j = 0; j < sc; j++){
			if(selected_i[j] == i) f = 1;
		}
		if(!f) {
			selected_i[sc] = i;
			selected[sc] = all_fq[i];
			sc++;
		}
	}
}


//signal kezelő
void handler(int signumber){
  printf("A jel megérkezett\n");
}

int main(int argc,char ** argv) {
	srand(time(NULL)); //random seedet besettel
	
	if (argc < 2) { 
		perror("HIBA! ADJON MEG ARGUMENTUMOT"); 
		return -1;
	}
	
	struct question all_fq[100];			//összes kérdést ebbe a kontéerbe tárolom
	int    all_fq_c = load_final_questions(all_fq);	//betölti a kérdéseket az adatbázisból
	if(all_fq_c < 3) { 
		perror("VÉGLEGESÍTÉS KÖZBEN HIBA LÉPETT FEL. VAN 3 KÉRDÉS LEGALÁBB?"); 
		return -1;
	}	
	//kiválaszt 3 kérdést
	struct question selected[3];
	int result[3][4];
	int pipefd[2]; 
	
	//bellítja a signalt
	struct sigaction sigact;
	sigact.sa_handler=handler; 
	sigemptyset(&sigact.sa_mask); 
	sigact.sa_flags=0; 
	sigaction(SIGTERM,&sigact,NULL);

	sigaction(SIGTERM,&sigact,NULL);
	// pipe-ot létrehozza
	if (pipe(pipefd) == -1) {
	   perror("Cső meghívása közben hiba történt!");
	   exit(EXIT_FAILURE);
	}
	// szülő-gyerek létrehozása
	pid_t pid = fork();	
	if (pid == -1) {
	   perror("Fork közben hiba történt!");
	   exit(EXIT_FAILURE);
	}
	
	if (pid == 0) {	// gyerek folyamat
		printf("%s Vár a kérdésekre.\n", argv[1]);
		
		read(pipefd[0],selected,sizeof(selected)); // olvas a pipebol
		close(pipefd[0]);
		printf("Kérdések megérkeztek, szavazás folyamatban...\n");
		kill(getppid(),SIGTERM); //szulo felkeltes
		
		vote(selected, result);
		
		write(pipefd[1], result, sizeof(result));
		close(pipefd[1]); // lezarjuk a readet
	} 
	else {    // szülő folyamat
	
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset,SIGTERM);
	
		select_random(selected, all_fq, all_fq_c);
		
		write(pipefd[1], selected,sizeof(selected)); //írás
		close(pipefd[1]); // write descriptort lezar
		
		sigsuspend(&sigset); //addig várunkamíg a gyerek befejezte az olvasást
		
		read(pipefd[0], result, sizeof(result));
		close(pipefd[0]);
		
		printf("Eredmények megérkeztek!\n");
		write_result(selected, 3, result);
		
		fflush(NULL); 	// mident bufferbe ír
		wait();		// a gyerekfolyamatra vár
	}



	return 0;
}