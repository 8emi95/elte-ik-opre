#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h> //waitpid

struct Message {
	char name[100];
	int age;
};

int main(){ 
	int status;

	int fid = mkfifo("namedpipe", S_IRUSR|S_IWUSR); // Nevesített cső
	int fd;

	if (fd < 0){
		perror("error");
		exit(1);
	}

 	fd = open("namedpipe",O_RDWR);
	
	struct pollfd poll_fds[5]; // 5 elemu poll-t hozunk letre
	poll_fds[0].fd=fd; // Hozzarendeljuk a 0.-hoz az elobb deklaralt csovet
	poll_fds[0].events=POLLIN; // lehetne POLLPUT is, az IN elvasasra, az OUT irasra vonatkozik

	// POLLIN - igaz, ha tettunk a csobe
	// POLLOUT - igaz, ha vettunk a csobol
 

	int child = fork();
	if (child>0) { 
		// Szulo
		struct Message m;
		
		printf("A szulo megprobalja kiolvasni a csovet...\n");
   	int result = poll(poll_fds, 1, 8000); // 8 s
		if (result>0) { 
   		if (poll_fds[0].revents & POLLIN) // POLLIN esemeny tortent
   		{
   			printf("Sikerult olvasni! \n");
				
				read(fd, &m, sizeof(m));
				printf("A szoveg: %s\nAge: %d\n", m.name, m.age);          
 			} 
		} else {
			printf("Lejart az idozito!\n");
			kill(child, SIGABRT); // Leállítja a gyerekfolyamatot
		}

   	waitpid(child, &status, 0);   //waits for the child
	
	} else { 
		// Gyerek
		printf("A gyerek a csobe ir.\n");
		struct Message m = {"Akos", 16};
		printf("Nev: ");
		scanf("%s", m.name);
		printf("Kor: ");
		scanf("%d", &m.age);
   	write(fd, &m, sizeof(m)); //writes to the pip
	}
 
	unlink("namedpipe");
	return 0; 
}
