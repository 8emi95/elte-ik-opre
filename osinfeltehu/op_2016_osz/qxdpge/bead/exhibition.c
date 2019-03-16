#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <dirent.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 

void handleCommand(char *argv[]) {
	if (strcmp("ex_new", argv[1]) == 0) {
		FILE *f;
		f = fopen(argv[2], "rb+");
		if(f == NULL) {
			f = fopen(argv[2], "wb");
			printf("Exhibition created successfully");
		}
	} else if (strcmp("ex_guests", argv[1]) == 0) {
		 printf("\n*****************\nGuests\n*****************\n");
		 FILE * f;
		 f=fopen(argv[2],"r");
		 if (f==NULL){perror("File opening error\n"); exit(1);}
		 char line[256];
		 while (!feof(f)){
		   fgets(line,sizeof(line),f);
		   printf("%s",line);
		 } 
		 
		 fclose(f);
	} else if (strcmp("ex_del", argv[1]) == 0) {
		int ret = remove(argv[2]);
		if(ret == 0) {
		  printf("Exhibition deleted successfully");
		}
		else {
		  printf("Error: unable to delete the exhibition");
		}
	} else if (strcmp("gu_new", argv[1]) == 0) {
		FILE *f = fopen(argv[4], "a");
		if (f == NULL) {
			printf("Error opening file!\n");
			exit(1);
		}
		
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		fprintf(f, "%s %s %s", argv[2],argv[3], asctime (timeinfo));
		fclose(f);
		
		FILE* myfile = fopen(argv[4], "r");
		int ch, number_of_lines = 0;

		do {
			ch = fgetc(myfile);
			if(ch == '\n')
				number_of_lines++;
		} while (ch != EOF);

		fclose(myfile);

		printf("You are the %d. guest.", number_of_lines);
	} else if (strcmp("gu_del", argv[1]) == 0) {
		char line[1024] ;
		FILE* fp = fopen(argv[2], "r+") ;
		long pos = ftell(fp);
		int a = 0;
		int found = 0;
		while (fgets(line , sizeof(line) , fp )!= NULL) {
			a++;
			if (strstr(line , argv[3] )!= NULL) {
				found = 1;
			}
		}
		
		fclose(fp);
		
		if(found == 1) {
		FILE *fileptr1, *fileptr2;

		char ch;
		int temp = 1;
		fileptr1 = fopen(argv[2], "r");
		fileptr2 = fopen("replica.c", "w");
		
		while (!feof(fileptr1))
		{
			ch = getc(fileptr1);
			if (ch == '\n'){
				temp++;
			}
			if (temp != a-1) {
				putc(ch, fileptr2);
			}
		}
		fclose(fileptr1);
		fclose(fileptr2);
		remove(argv[2]);
		rename("replica.c", argv[2]);
		}
	} else if (strcmp("gu_edit", argv[1]) == 0) {
		char line[1024] ;
		FILE* fp = fopen(argv[2], "r+") ;
		long pos = ftell(fp);
		int a = 0;
		int found = 0;
		while (fgets(line , sizeof(line) , fp )!= NULL) {
			a++;
			if (strstr(line , argv[3] )!= NULL) {
				found = 1;
			}
		}
		
		fclose(fp);
		
		if(found == 1) {
		FILE *fileptr1, *fileptr2;

		char ch;
		int temp = 1;
		fileptr1 = fopen(argv[2], "r");
		fileptr2 = fopen("replica.c", "w");
		
		while (!feof(fileptr1))
		{
			ch = getc(fileptr1);
			if (ch == '\n'){
				temp++;
			}
			
			if (temp != a-1) {
				putc(ch, fileptr2);
			}
		}
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		fprintf(fileptr2, "%s %s %s", argv[4],argv[5], asctime (timeinfo));
		fclose(fileptr1);
		fclose(fileptr2);
		remove(argv[2]);
		rename("replica.c", argv[2]);
		}
	}
	
}



void printHelp() {
	printf("Arguments:\n");
	printf("ex_new [exhibition_id]\n\tCreate new exhibition.\n");
	printf("ex_del [exhibition_id]\n\tDelete the specified exhibition with all of the data.\n");
	printf("ex_guests [exhibition_id]\n\tGuests list for the specified exhibition.\n");
	printf("gu_new [guest_name] [guest_email] [exhibition_id]\n\tAdd new guest for the specified exhibition.\n");
	printf("gu_edit [exhibition_id] [guest_name] [guest_email]\n\tEdit the data of the specified user in a specified exhibition.\n");
	printf("gu_del [exhibition_id] [guest_name]\n\tDelete the specified user in a specified exhibition.\n");
	exit(0);
}

int main( int argc, char *argv[] ) {

    if (argc < 3) {
		printHelp();
    }
	handleCommand(argv);
	
	return 0;
}