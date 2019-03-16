//Read and print out the file given in the parameter 
#include <stdio.h>



/* FILE (tipus) pl.: FILE *f;    */
/* fopen (fileName, mód) f==NULL ha sikertelen  mód: karakter sorozat */
/* fclose (f) egy paraméter filepointer         */
/* fseek /
/* feof*/
/* fprintf szöveges*/
/* fputs szöveges*/
/* fgets szöveges*/
/* fwrite bináris*/
/* fread bináris*/


/* szöveges file kezelés*/
/* 'r' írás*/
/* 'w' olvasás*/
/* 'a' hozzáírás hA NEM LÉTEZIK LÉTREHOZZA*/
/* 'r+' olvasás és írás*/
/* 'w+' létrehozza ha nem létezik olvasás és írás*/
/* 'a+' olvasás és hozzáírás*/

/* fputs(kiirandó szöveg sztringpointer, file pointer)*/
/* fprintf(filepointer, formátum string, változólista) return kiirt karakterek száma;*/
/* fgets(string pointer ahova olvasunk, max beolvasandó karakterszám, filepointer) return 0 ha nem tudott beolvasni sorvégig olvas vagy file végig olvas*/
/* feof(filepointer) nem nulla ha a file vége után vagyunk*/

/* #include <unistd.h> */
/* rendszer hívások */
/* access (file, mód) hozzáférési jog ellenörzése 0 ha van hozzáférés*/
/* R_OK olvasás*/
/* W_OK írás */
/* X_OK futtatás */
/* F_OK létevés fentiek vagyolása */

#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno
#include <string.h>

/* bináris filekezelés */
/*
rb
wb
ab
rb+
wb+
ab+

fwrite (kezdõcím, ellem méret, elemszám, filepointer)
fread ugyanaz int az fwrite 

*/

/*
teljes tömb feltöltésse olvasása

fread/fwrite (t, sizeof(t[0]), sizeof(t)/sizeof(t[0]), f)

egy változó értékének kiirása beolvasása

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