#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

struct Member {
   char  name[20];
   char  email[20];
   int   stand_id;
};

void use_fopen_bin_read(char* fname){
 printf("\n******************\nUsing fopen -  binary \n*****************\n");
 FILE * f;
 f=fopen(fname,"rb");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 struct Member person;
 
 while (fread(&person,sizeof(struct Member),1,f) != NULL) {
		printf("Name: %s ", &person.name);
		printf("Email: %s ", &person.email);
		printf("Stand ID: %d\n", &person.stand_id);
	}
 printf("\n");
 
 fclose(f);
}

void use_fopen_bin_write(char* fname){

 struct Member person;
 FILE * f;
 
 f=fopen(fname,"rb");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 int counter=1;
 while (fread(&person,sizeof(struct Member),1,f) != NULL) {
		counter+=1;
	}
 
 f=fopen(fname,"a");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 
 printf("Name: ");
 scanf("%s", &person.name);
 printf("Email: ");
 scanf("%s", &person.email);
 printf("Stand ID: ");
 scanf("%d", &person.stand_id);
 
 fwrite(&person, sizeof(person), 1, f);
 printf("You are the %d th in the event.\n",counter);

 fclose(f);
}


void use_fopen_bin_delete(char* fname, char *searchname){
 FILE * f;
 FILE * f_tmp;
 
 struct Member person;
 
 f=fopen(fname,"rb");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 
 f_tmp=fopen("tmp.bin", "wb");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
 
 while (fread(&person,sizeof(struct Member),1,f) != NULL) {
		if (strcmp (searchname, person.name) == 0) {
			printf("A record found and deleted.\n");
		} else {
			fwrite(&person, sizeof(struct Member), 1, f_tmp);
		}
	}
    fclose(f);
	fclose(f_tmp);

	remove(fname);
	rename("tmp.bin", fname);
}



void use_fopen_bin_modify(char* fname, char *searchname){
 FILE * f;
 FILE * f_tmp;
 
 struct Member person;
 
 f=fopen(fname,"rb");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 
 f_tmp=fopen("tmp.bin", "wb");
	if (f==NULL){perror("File opening error\n"); exit(1);}
	
 
 while (fread(&person,sizeof(struct Member),1,f) != NULL) {
		if (strcmp (searchname, person.name) == 0) {
			printf("The record found.\n");
			printf("New Name: ");
			scanf("%s", &person.name);
			printf("\nNew Email: ");
			scanf("%s", &person.email);
			printf("\nNew Stand ID: ");
			scanf("%d", &person.stand_id);
			fwrite(&person, sizeof(struct Member), 1, f_tmp);
		} else {
			fwrite(&person, sizeof(struct Member), 1, f_tmp);
		}
	}
    fclose(f);
	fclose(f_tmp);

	remove(fname);
	rename("tmp.bin", fname);
}


void use_fopen_bin_create(char* fname){

 FILE * f;
 f=fopen(fname,"wb");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 fclose(f);
}

int main(int argc,char** argv){
 
 
int option;
char fname[20];
char searchname[20];
do
{
	printf("What should I do?\n");
	printf("1: add new applicant\n");
	printf("2: list the applicants\n");
	printf("3: modifying the applicants\n");
	printf("4: delete an applicant\n");
	printf("5: create new stand\n");
	printf("6: delete a stand\n");
	printf("7: exit\n");
	
	scanf("%d", &option);
	
	
	switch(option)
	{
		case 1:
			printf("which stand?\n");			
			scanf("%s",fname);
			use_fopen_bin_write(fname);
			break;
		case 2:
			printf("which stand?\n");
			scanf("%s",fname);
			use_fopen_bin_read(fname);
			break;
		case 3:
			printf("which stand?\n");
			scanf("%s",fname);
			printf("which name?\n");
			scanf("%s",searchname);
			use_fopen_bin_modify(fname,searchname);
			break;
		case 4:
			printf("which stand?\n");
			scanf("%s",fname);
			printf("which name?\n");
			scanf("%s",searchname);
			use_fopen_bin_delete(fname,searchname);
			break;
		case 5:
		    printf("What is the name of the new stand?\n");
			scanf("%s",fname);
			use_fopen_bin_create(fname);
			break;
		case 6:
			printf("which stand?\n");
			scanf("%s",fname);
			remove(fname);
			break;
		case 7:
			option = 0;
			break;
			
	}
}
while(option != 0);
	
	
 
 return 0;
}