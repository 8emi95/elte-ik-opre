#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "kos.h"

#define MAX_RENDEZVENY 256

char fn_rendezvenyek[] = "rendezveny.data";
char fn_vendegek[] = "vendeg.data";
char fn_tmp[] = "tmp.data";

struct rendezveny_t rendezvenyek[MAX_RENDEZVENY];
int rendezvenyek_n = 0;
int rendezveny_max_id = 0;
int pipe_megbizas[2]; // unnamed pipe file descriptor array
int pipe_jelentes[2]; // unnamed pipe file descriptor array

void beolvas_rendezvenyek() {
	FILE * f;
	f = fopen(fn_rendezvenyek, "r");
	if (f) {
		while (!(feof(f))) {
			if (fread(&(rendezvenyek[rendezvenyek_n]), sizeof(struct rendezveny_t), 1, f)) {
				if (rendezveny_max_id<rendezvenyek[rendezvenyek_n].id) {
					rendezveny_max_id = rendezvenyek[rendezvenyek_n].id;
				}
				rendezvenyek[rendezvenyek_n].vendegek = 0;
				rendezvenyek_n++;
			}
		}
		fclose(f);
	}
	int i;
	struct vendeg_t vendeg;
	f = fopen(fn_vendegek, "r");
	int n = 0;
	if (f) {
		while (!(feof(f))) {
			if (fread(&vendeg, sizeof(struct vendeg_t), 1, f)) {
				for (i=0; i<rendezvenyek_n; i++) {
					if (rendezvenyek[i].id == vendeg.rendezveny_id) {
						rendezvenyek[i].vendegek++;
						break;
					}
				}
			}
		}
		fclose(f);
	}
}

void kiir_rendezvenyek() {
	FILE * f;
	f = fopen(fn_rendezvenyek, "w");
	fwrite(rendezvenyek, sizeof(struct rendezveny_t), rendezvenyek_n, f);
	fclose(f);
}

int szervezo_handled = 0;

void szervezo_handler(int signumber){
	//printf("SIGNAL %d, PID: %d", signumber, getpid());
	switch (signumber) {
		case SIGUSR1: szervezo_handled = 1; break;
	}
}

void szervezo() {
	close(pipe_megbizas[0]); // lezárjuk az olvasási oldalt, mi csak írunk
	close(pipe_jelentes[1]); // lezárjuk az írási oldalt, mi csak olvasunk
	int i,j;
	struct rendezveny_t endoflist;
	struct vendeg_t vendeg;
	struct vendeg_t nem_erkezett_meg[4096];
	int nem_erkezett_meg_n=0; // ebbe kapjuk majd meg a partnertol a darabszamot
	int nem_erkezett_meg_n_ossz=0;
	int sikeressegi_mutato;
	FILE * f;

	endoflist.id = 0;
	for (i=0; i<rendezvenyek_n; i++) {
		szervezo_handled = 0;
		write(pipe_megbizas[1], rendezvenyek+i, sizeof(struct rendezveny_t));
		if (szervezo_handled==0) { pause(); }
		f = fopen(fn_vendegek, "r");
		int n = 0;
		if (f) {
			while (!(feof(f))) {
				if (fread(&vendeg, sizeof(struct vendeg_t), 1, f)) {
					if (rendezvenyek[i].id == vendeg.rendezveny_id) {
						write(pipe_megbizas[1], &vendeg, sizeof(struct vendeg_t));
					}
				}
			}
			fclose(f);
		}
		read(pipe_jelentes[0], &sikeressegi_mutato, sizeof(int));
		printf("-szervezo- >> sikeressegi mutato - %s: %d\n", rendezvenyek[i].nev, sikeressegi_mutato);
		read(pipe_jelentes[0], &nem_erkezett_meg_n, sizeof(int));
		printf("-szervezo- >> nem_erkezett_meg_n - %s: %d\n", rendezvenyek[i].nev, nem_erkezett_meg_n);
		for (j=0; j<nem_erkezett_meg_n; j++) {
			read(pipe_jelentes[0], nem_erkezett_meg+nem_erkezett_meg_n_ossz, sizeof(struct vendeg_t));
			nem_erkezett_meg_n_ossz++;
		}
	}
	write(pipe_megbizas[1], &endoflist, sizeof(struct rendezveny_t));
	printf("nem erkezett meg:\n");
	for (j=0; j<nem_erkezett_meg_n_ossz; j++) {
		printf("- %s\n", nem_erkezett_meg[j].nev);
	}
}

void partner(pid_t szervezo_pid) {
	close(pipe_megbizas[1]); // lezárjuk az írási oldalt, mi csak olvasunk
	close(pipe_jelentes[0]); // lezárjuk az olvasási oldalt, mi csak írunk
	struct rendezveny_t rendezveny;
	struct vendeg_t vendeg;
	struct vendeg_t nem_erkezett_meg[4096];
	int nem_erkezett_meg_n=0;
	int sikeressegi_mutato;
	int i;
	
	srand(time(NULL)+getpid());
	
	while (1) {
		nem_erkezett_meg_n=0;
		read(pipe_megbizas[0], &rendezveny, sizeof(struct rendezveny_t));
		if (rendezveny.id == 0) break;
		printf("Rendezvény: %s\n", rendezveny.nev);
		sleep(1); // utazunk
		kill(szervezo_pid, SIGUSR1);
		for (i=0; i<rendezveny.vendegek; i++) {
			read(pipe_megbizas[0], &vendeg, sizeof(struct vendeg_t));
			printf("  vendég: %s\n", vendeg.nev);
			if (rand()%100 < 10) {
				nem_erkezett_meg[nem_erkezett_meg_n++] = vendeg;
			}
		}
		sleep(1); // lebonyolitas
		sikeressegi_mutato = rand()%100;
		write(pipe_jelentes[1], &sikeressegi_mutato, sizeof(int));
		write(pipe_jelentes[1], &nem_erkezett_meg_n, sizeof(int));
		for (i=0; i<nem_erkezett_meg_n; i++) {
			write(pipe_jelentes[1], nem_erkezett_meg+i, sizeof(struct vendeg_t));
		}
	}
}

void main(int argc, char** argv) {
	printf("King Of Stands - lebonyolítás\n");
	printf("-----------------------------\n");
	beolvas_rendezvenyek();
	if (pipe(pipe_megbizas) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipe_jelentes) == -1) 
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	pid_t child = fork();
	if (child<0) { perror("Forkolási hiba"); exit(1); }
	if (child>0) {
		signal(SIGUSR1, szervezo_handler);
		szervezo();
		int status;
		wait(&status);
	} else {
		partner(getppid());
	}
	exit(0);
}
