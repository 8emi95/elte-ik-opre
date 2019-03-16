#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


volatile int szamlalo = 0;

void pecset() {
	szamlalo++;
	printf("Pecsetelo vagyok");
}

void ell() {
	szamlalo++;
	printf("Ellenorzo vagyok");
}

void elnok() {
	printf("Elnok vagyooook");
}

int main(int argc, char** argv) {

	//signal(SIGUSR, handler); // TODO
	int szavazok_szama = atoi(argv[1]);

	// pipe hívás fork előtt (hogy mind a kettő ugyanazt lássa)

	//////////////////

	pid_t parent = getpid();

	pid_t child_pecset = fork();
	pid_t child_kiad;

	if (getpid() == parent) {
		child_kiad = fork();
	} else { // elvileg a pecsételő folyamat
		pecset();
	}

	if (getpid() == parent) { // még mindig a szülőben vagyok

		// itt jelentkezik be a két gyerek
		while (szamlalo != 2) {sleep(1);} // pause helyett, mert lehet h a szignál előbb megjön mint h elkezdjük a pause-zal várni

		elnok(); // amit csinál az elnök nyilván

		wait(NULL); wait(NULL); // bevárja mindkét gyereket

	} else { // elvileg az ellenörző folyamat
		ell();
	}


}