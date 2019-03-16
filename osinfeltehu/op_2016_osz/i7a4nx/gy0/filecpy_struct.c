//make a copy of file given in argv[1] to file given in argv[2] 
//fajl masolas program
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> //open,creat
#include <sys/types.h> //open
#include <sys/stat.h>
#include <errno.h> //perror, errno

struct toCopy {
   char  name[25];
   int   id;
} toCopy; 

int main(int argc,char** argv){
 
 struct toCopy toCopy1;
 
 if (argc!=3){perror("You have to use program with two arguments, the file names copy_from copy_to");exit(1);}
 int f,g;
 
 //alacsony szinten a file leiro egy egesz szam
 //2.par milyen tipusu muveletet vegeznenk a fileval
 //binaris tipusu filekezelest fogunk csinalni a gyakorlaton
	//szovegesnel sorvege jelig akarunk olvasni
	//binarisnal en mondom meg hogy hany bajtot / bajtonkent akarunk dolgozni
 f=open(argv[1],O_RDONLY); 
 //there is an access function with which we can see whethet the file is exist or not
 //access(filename,F_OK);
 //open 1. parameter= file name
 //open 2. parameter for what we want to use the file 
 //O_RDONLY=only for reading,O_WRONLY-only for writing 

 
 //ellenorzes
	//C ben jellemzoen sikertelen muveletnel altalban -1 et
	//vagy nullptrt kapunk
	//ebbe az 	errno	nevezetu valtozoba
	//ehhez az 	error.h 	include kell
 if (f<0){ perror("Error at opening the file\n");exit(1);}
 //There is errno variable in which there is the number of error -- 
 //if (errno!=0) there is an error
 
//				writeonly	letrehozas	eltorolni és ujrakezdeni
// iruser olvasasra az usernek tulajdonosnak 
// iwuser	irasra csak az usernek tulajdosnonsak  
 g=open(argv[2],O_WRONLY	|O_CREAT	|O_TRUNC,S_IRUSR		|S_IWUSR);
 //the three parameter long version of open - it can create the file if it doesnt exist
 //there is a creat function as well - creat(filename,permissions);
 //O_TRUNC = if it existed, clear the content,
 //O_CREAT=create if it doesn't exist
 //3. parameter the permission, if it has to be created
 //S_IRUSR=permission for reading by the owner e.g.
 if (g<0){ perror("Error at opening the file\n");exit(1);}
 
 
 //tenyleges olvasas
 char c;
 char* name;
 int id;
 // read masodik parameter egy valtozora mutato mutato
 //		mert nincsen referencia tipusu atadas C ben
 //		athidalas	valtozo cimet adjuk at ertek szerinti par atadassal
 //a read ott allitja le az olvasast, ahol a read sikertelen lesz
 //		azaltal hogy 0 bajtot sikerult beolvasni 0t ad vissza ez leallitja a ciklust
 while (read(f,&toCopy1,sizeof(toCopy))){ 
    //read gives back the number of bytes
    //1. parameter the file descriptor
    //2. parameter the address of variable, we read into
    //3. parameter the number of bytes we want to read in
    
	printf("%c",c); //we prints out the content of the file on the screen
	strcpy( toCopy1.name, )
	strcpy( toCopy1.id, )
//atmasolas
//az utolso parameter az hogy az adott cimtol kezdodoen hany bajtrol van szo
//		write!=sizeof	azt adja vissza hogy hany karaktert sikerult kiirni
//		igy tud hibat jelezni
    /*if */(write(g,&toCopy1,sizeof(toCopy));
      //{perror("There is a mistake in writing\n");exit(1);}
      //write gives back the number of written bytes
      //1. parameter the file descriptor
      //2. parameter the address of variable we want to write out
      //3. parameter the number of bytes we want to write out 
 } 
 close(f);
 close(g);
 return 0;
}