#include<stdio.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/stat.h> 
#include<fcntl.h> 

int main() { 
    int child; 
    
/* Itt hozzuk l�tre a FIFO f�jlt. A felhaszn�l�nak �r�si �s olvas�si joga lesz. */ 
    mkfifo("fifo_teszt", S_IRUSR | S_IWUSR ); 
    
    child = fork(); 
    
    if(child > 0) { 
        char s[1024]; 
        int fd; 
        
/* A sz�l�folyamat megnyitja a f�jlt olvas�sra, mert csak olvasni fog, majd kiolvassa az �zenetet. */ 
        fd = open("fifo_teszt", O_RDONLY); 
        read(fd, s, sizeof(s)); 
        printf("%s", s); 
/* Lez�rja �s t�rli a f�jlt. */ 
        close(fd); 
        unlink("fifo_teszt"); 
    } else if(child == 0) { 
        int fd;
        
/* A gyerekfolyamat megnyitja a f�jlt �r�sra, mert csak �rni fog, majd bele�rja az �zenetet, v�g�l lez�rja. */ 
        fd = open("fifo_teszt", O_WRONLY); 
        write(fd, "Szia Mami!\n", 12); 
        close(fd); 
    } 
    
    return 0; 
}