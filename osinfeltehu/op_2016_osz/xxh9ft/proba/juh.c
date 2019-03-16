//Read and print out the file given in the parameter 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

/** /
void use_open_bin(char* fname){
 printf("\n**********\nUsing open - binary \n****************\n");
 int f=open(fname,O_RDONLY); 
 if (f<0){ perror("Error at opening the file\n");exit(1);}
 
 char c; //instead of char, you can use any other type, struct as well
 while (read(f,&c,sizeof(c))){ //use write for writing
    printf("%c",c); //we prints out the content of the file on the screen
 } 
 printf("\n"); 
 lseek(f,0,SEEK_SET); //position to the first character, SEEK_SET,SEEK_CUR,SEEK_END
 read(f,&c,sizeof(c));
 printf("First character - after positioning %c\n",c);
 close(f);
}
/**/
struct Jelentkezes {
   char nev[160];
   char email[160];
   int rAzon;
};

void olvas_rendezveny(char* fname, int ferohely){
    struct Jelentkezes tmp[ferohely];
    printf("\n******************\nOlvasom rendezvenyt\n*****************\n");
    FILE * f;
    f=fopen(fname,"r");
    if (f==NULL){perror("File opening error\n"); exit(1);}
    char nev[160];
    char email[160];
    int strRAzon;
    int rAzon;
    int i=0;

    while (!feof(f)){
        fgets(nev,sizeof(nev),f);
        fgets(email,sizeof(email),f);
        //fgets(strRAzon,sizeof(strRAzon),f);
        fscanf(f, "%d", &rAzon);
        /** /
        rAzon = atoi(strRAzon);

        strcpy( Jelentkezes[i].nev, nev);
        strcpy( Jelentkezes[i].author, email); 
        Jelentkezes[i].rAzon = rAzon;
        /**/
        printf("%s",nev);
        printf("%s",email);
        printf("%s",strRAzon);

        i=i+1;
    }

    printf("\n");
    fclose(f);
}
/** /
void use_fopen_bin(char* fname){
 printf("\n******************\nUsing fopen - binary \n*****************\n");
 FILE * f;
 f=fopen(fname,"rb");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 char c; //instead of char, you can use any other type, struct as well
 while (!feof(f)){
   fread(&c,sizeof(c),sizeof(c),f); //use fwrite for writing
   printf("%c",c);
 } 
 printf("\n");
 fseek(f,0,SEEK_SET); //position to the first character, SEEK_SET,SEEK_CUR,SEEK_END
 fread(&c,sizeof(c),sizeof(c),f);
 printf("First character - after positioning %c\n",c);
 
 fclose(f);
}
/**/
int main(int argc,char** argv){
 //if (argc!=2) {perror("Give a filename as a command line argument\n");exit(1);}
 //if (!access(argv[1],F_OK)==0){perror("The file does not exist!\n"); exit(1);}
 //use_open_bin(argv[1]);
 olvas_rendezveny(argv[1], 100);
 //use_fopen_bin(argv[1]);
 
FILE *f = fopen("file.txt", "w");
if (f == NULL) {
    printf("Error opening file!\n");
    exit(1);
}

/* print some text */
const char *text = "Write this to the file";
fprintf(f, "Some text: %s\n", text);

/* print integers and floats */
int i = 1;
float py = 3.1415927;
fprintf(f, "Integer: %d, float: %f\n", i, py);

/* printing single chatacters */
char c = 'A';
fprintf(f, "A character: %c\n", c);

fclose(f);


 
 return 0;
}