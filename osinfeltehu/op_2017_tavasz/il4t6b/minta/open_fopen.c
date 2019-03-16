//Read and print out the file given in the parameter 
#include <stdio.h>



/* FILE (tipus) pl.: FILE *f;    */
/* fopen (fileName, m�d) f==NULL ha sikertelen  m�d: karakter sorozat */
/* fclose (f) egy param�ter filepointer         */
/* fseek /
/* feof*/
/* fprintf sz�veges*/
/* fputs sz�veges*/
/* fgets sz�veges*/
/* fwrite bin�ris*/
/* fread bin�ris*/


/* sz�veges file kezel�s*/
/* 'r' �r�s*/
/* 'w' olvas�s*/
/* 'a' hozz��r�s hA NEM L�TEZIK L�TREHOZZA*/
/* 'r+' olvas�s �s �r�s*/
/* 'w+' l�trehozza ha nem l�tezik olvas�s �s �r�s*/
/* 'a+' olvas�s �s hozz��r�s*/

/* fputs(kiirand� sz�veg sztringpointer, file pointer)*/
/* fprintf(filepointer, form�tum string, v�ltoz�lista) return kiirt karakterek sz�ma;*/
/* fgets(string pointer ahova olvasunk, max beolvasand� karaktersz�m, filepointer) return 0 ha nem tudott beolvasni sorv�gig olvas vagy file v�gig olvas*/
/* feof(filepointer) nem nulla ha a file v�ge ut�n vagyunk*/

/* #include <unistd.h> */
/* rendszer h�v�sok */
/* access (file, m�d) hozz�f�r�si jog ellen�rz�se 0 ha van hozz�f�r�s*/
/* R_OK olvas�s*/
/* W_OK �r�s */
/* X_OK futtat�s */
/* F_OK l�tev�s fentiek vagyol�sa */

#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

/* bin�ris filekezel�s */
/*
rb
wb
ab
rb+
wb+
ab+

fwrite (kezd�c�m, ellem m�ret, elemsz�m, filepointer)
fread ugyanaz int az fwrite 

*/

/*
teljes t�mb felt�lt�sse olvas�sa

fread/fwrite (t, sizeof(t[0]), sizeof(t)/sizeof(t[0]), f)

egy v�ltoz� �rt�k�nek kiir�sa beolvas�sa

fread/fwrite (&v, sizeof(v), 1, f)




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
void use_fopen_text(char* fname){
 printf("\n******************\nUsing fopen -  reads in lines\n*****************\n");
 FILE * f;
 f=fopen(fname,"r");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 char line[160];
 while (!feof(f)){
   if(
   fgets(line,sizeof(line),f) != NULL)
 {
   printf("%s",line);
 }
 } 
 printf("\n");
 fclose(f);
}
void use_fopen_bin(char* fname){
 printf("\n******************\nUsing fopen -  binary \n*****************\n");
 FILE * f;
 f=fopen(fname,"rb");
 if (f==NULL){perror("File opening error\n"); exit(1);}
 char c; //instead of char, you can use any other type, struct as well
 while (!feof(f)){
   fread(&c,sizeof(c),1,f); //use fwrite for writing
   printf("%c",c);
 } 
 printf("\n");
 fseek(f,0,SEEK_SET); //position to the first character, SEEK_SET,SEEK_CUR,SEEK_END
 fread(&c,sizeof(c),sizeof(c),f);
 printf("First character - after positioning %c\n",c);
 
 fclose(f);
}
int main(int argc,char** argv){
 if (argc!=2) {perror("Give a filename as a command line argument\n");exit(1);}
 if (!access(argv[1],F_OK)==0){perror("The file is not exist!\n"); exit(1);}
 use_fopen_text(argv[1]);
 use_open_bin(argv[1]);
 use_fopen_bin(argv[1]);
 return 0;
}