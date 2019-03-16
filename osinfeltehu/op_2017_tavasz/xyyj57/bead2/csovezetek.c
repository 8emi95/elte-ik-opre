#include<stdio.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/stat.h> 
#include<fcntl.h> 

int main() { 
    int child; 
    
/* Itt hozzuk létre a FIFO fájlt. A felhasználónak írási és olvasási joga lesz. */ 
    mkfifo("fifo_teszt", S_IRUSR | S_IWUSR ); 
    
    child = fork(); 
    
    if(child > 0) { 
        char s[1024]; 
        int fd; 
        
/* A szülõfolyamat megnyitja a fájlt olvasásra, mert csak olvasni fog, majd kiolvassa az üzenetet. */ 
        fd = open("fifo_teszt", O_RDONLY); 
        read(fd, s, sizeof(s)); 
        printf("%s", s); 
/* Lezárja és törli a fájlt. */ 
        close(fd); 
        unlink("fifo_teszt"); 
    } else if(child == 0) { 
        int fd;
        
/* A gyerekfolyamat megnyitja a fájlt írásra, mert csak írni fog, majd beleírja az üzenetet, végül lezárja. */ 
        fd = open("fifo_teszt", O_WRONLY); 
        write(fd, "Szia Mami!\n", 12); 
        close(fd); 
    } 
    
    return 0; 
}