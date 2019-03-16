/*
 * 1. feladat:
 * Készítsen C programozási nyelven programot,
 * amivel egy adott könyvtár (és alkönyvtárainak)
 * link (hard, soft) bejegyzéseit tudjuk kezelni
 * (megnézni, módosítani, törölni).
 * 
 * (A kiválasztott műveletet a fork- exec
 * utasításpár segítségével oldja meg!)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>

#define BUFFERSIZE 512

void Konyvtar_Kiir(int argc, char** argv);
void HardLink(int argc, char** argv);
void SoftLink(int argc, char** argv);
void Link_Torol(int argc, char** argv);
void UjFile(int argc, char** argv);
void Help();

int main(int argc, char** argv)
{
    int child;
    char buffer[BUFFERSIZE];
 
    while(1)
	{
        printf("> ");
        scanf("%s", buffer);

		child = fork();
        if(child > 0)       /* SZÜLŐ */
        {        
            waitpid(-1, NULL, 0);
            sleep(1);
        }
        else if(child == 0) /* GYEREK */
        {
			if( strcmp(buffer, "link") == 0)
				HardLink(argc, argv);   /* HARD LINK */
				
            else if( strcmp(buffer, "symlink") == 0)
				SoftLink(argc, argv);   /* SOFT LINK */
				
            else if( strcmp(buffer, "unlink") == 0)
				Link_Torol(argc, argv); /* UNLINK    */
				
            else if( strcmp(buffer, "ls") == 0)
				Konyvtar_Kiir(argc, argv);
				
            else if( strcmp(buffer, "touch") == 0)
				UjFile(argc, argv);
				
            else if( strcmp(buffer, "exit") == 0)
				{
				printf("Press CTRL + C to exit.\n");
				sleep(10);
				}
            else if( strcmp(buffer, "help") == 0)
				Help();
            else
			{
                printf("Bad command. Type help for more information.\n");
            }  

        }
        else    /* HIBA */
        {
            perror("fork");
        }
    } 

    return 0;
}

void Konyvtar_Kiir(int argc, char** argv)
{
        DIR *p;
        struct dirent *akt;
        
        if(argc == 1) {          
            p = opendir("./");
        }
        else {
            p = opendir(argv[1]);
            chdir(argv[1]);
        }
  
        if(p!=NULL)
        {
            while( (akt = readdir(p)) != NULL ) {
                
                struct stat s;
                char* slink;

                lstat(akt->d_name, &s); // lstat jelzi a softlinket

                printf("%d\t", (int)s.st_ino);
                printf("%d", s.st_nlink);
                
                if(S_ISLNK(s.st_mode)) printf("  S\t");   // symlink megjelölése
                else if( !S_ISDIR(s.st_mode) && s.st_nlink >= 2 ) {
                    printf("  H\t");
                }
                
                else printf("\t");
				
                printf("%s", akt->d_name);  
				
                if(S_ISLNK(s.st_mode)) {
                    slink = (char*)malloc( BUFFERSIZE * sizeof(char) );
                    readlink(akt->d_name, slink, BUFFERSIZE);
                    printf(" -> %s\n", slink);
                    free(slink);
                }
                else printf("\n");  
            }
            closedir(p);
        }
        
        else { /* HIBA */
            perror("opendir");
        }
}

void HardLink(int argc, char** argv)
{
    char* old;
    char* new;
    old = (char *)malloc( BUFFERSIZE * sizeof(char) );
    new = (char *)malloc( BUFFERSIZE * sizeof(char) );
    
    
    printf("oldpath: ");
    scanf("%s", old);
    
    printf("newpath: ");
    scanf("%s", new);
    
    
    if(argc == 1) {          
        chdir("./");  
    }
    else {
        chdir(argv[1]);
    }
        
    link(old, new);
   
	if(errno) perror("link");
    
    free(old);
    free(new);
}

void SoftLink(int argc, char** argv)
{
    char* old;
    char* new;
    old = (char *)malloc( BUFFERSIZE * sizeof(char) );
    new = (char *)malloc( BUFFERSIZE * sizeof(char) );
    
    
    printf("oldpath(for this): ");
    scanf("%s", old);
    
    printf("newpath(from that): ");
    scanf("%s", new);
    
    
    if(argc == 1) {          
        chdir("./");  
    }
    else {
        chdir(argv[1]);
    }
        
    int a = symlink(old, new);
    
    if(a == -1)
    { 
        if(EEXIST)  // link felülírása
        {
         unlink(new);
         symlink(old, new);
        }
    }
    
    if(errno) perror("symlink");
        
    free(old);
    free(new);    
}

void Link_Torol(int argc, char** argv)
{
    char* old;
    old = (char *)malloc( BUFFERSIZE * sizeof(char) );
    printf("path: ");
    scanf("%s", old);
    
    if(argc == 1) {          
        chdir("./");  
    }
    else {
        chdir(argv[1]);
    }
    
    unlink(old);
    perror("unlink");
    
    free(old);
}

void UjFile(int argc, char** argv)
{
    char* new;
    new = (char *)malloc( BUFFERSIZE * sizeof(char) );
    printf("filename: ");
    scanf("%s", new);
    
    if(argc == 1) {          
        chdir("./");  
    }
    else {
        chdir(argv[1]);
    }

    mknod(new, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR, 0);
    perror("mknod");
}

void Help()
{
    printf("====== Help ======\n");
    printf("Available commands:\n");
    printf("ls\t- list the working directory\n");
    printf("link\t- make hardlink\n");
    printf("symlink\t- make softlink(forced mode)\n");
    printf("unlink\t- remove file\n");
    printf("touch\t- create a new empty file\n"); 
    printf("exit\t- Press CTRL + C to close the program.\n");
    printf("\n");
    printf("====== end ========\n");  
}






