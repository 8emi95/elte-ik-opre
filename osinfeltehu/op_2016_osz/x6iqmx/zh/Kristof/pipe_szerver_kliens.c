#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>

struct Message {
	int a;
	char op;
	int b;
	int r;
};

int main() {
	int status;
	
	int pipe_keres[2];
	int pipe_valasz[2];
	
	if(pipe(pipe_keres) == -1) {
		printf("HIBA");
		exit(2);
	}
	
	if(pipe(pipe_valasz) == -1) {
		printf("HIBA");
		exit(2);
	}
	
	int child = fork();
	
	if(fork < 0) {
		printf("HIBA");
		exit(1);
	}
	
	if(child > 0) {
		// SZÜLÕ
		close(pipe_keres[1]);
		close(pipe_valasz[0]);
		
		struct Message keres;
		
		while(keres.op != 'q') {
			read(pipe_keres[0], &keres, sizeof(keres));
			printf("Szulo kerest kapott!\n");
			
			if(keres.op == '+') {
				keres.r = keres.a + keres.b;
			} else if(keres.op == '-') {
				keres.r = keres.a - keres.b;
			} else if(keres.op == '*') {
				keres.r = keres.a * keres.b;
			} else if(keres.op == '/') {
				keres.r = keres.a / keres.b;
			}
			
			if(keres.op != 'q') {
				write(pipe_valasz[1], &keres, sizeof(keres));
				fflush(NULL);
				
				printf("Szulo elkuldte a valaszt!\n");
			}
		}
		
		close(pipe_keres[0]);
		close(pipe_valasz[1]);
		
		waitpid(child, &status, 0);
		
		printf("A szulo kilep!\n");
	} else {
		// GYEREK
		close(pipe_keres[0]);
		close(pipe_valasz[1]);
		
		struct Message m;
		int a;
		char op = ' ';
		int b;
		
		while(op != 'q') {
			
			printf("a: ");
			scanf("%d", &a);
			
			fgetc(stdin);
			printf("op: ");
			scanf("%c", &op);
			
			printf("b: ");
			scanf("%d", &b);
			
			m.a = a;
			m.op = op;
			m.b = b;
			
			
			write(pipe_keres[1], &m, sizeof(m));
			fflush(NULL);
			
			if(op != 'q') {
				printf("Gyerek elkuldte a kereset!\n");
				read(pipe_valasz[0], &m, sizeof(m));
				printf("Gyerek megkapta a keresere a valaszt!\n");
				
				printf("%d %c %d = %d\n\n", m.a, m.op, m.b, m.r);
			} else {
				printf("A gyerek kilep!\n");
			}
		}
		
		close(pipe_keres[1]);
		close(pipe_valasz[0]);
		
		exit(0);
	}
	
	return 0;
}