#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <time.h>

#include <errno.h>



typedef struct VisitorStruct {
        char name[50];
        char email[50];
        char showID[5];
        int numOfReg;
		time_t addedTime;
} VisStruct; 

typedef struct Show {
        char name[50];
		char showID[5];
		int numOfReg;
} ShowStruct; 

void writeDataToFile(int currVisitorsNumber, VisStruct* currVisitors, ShowStruct* shows, int currShowNumber);
void deleteLastChar(char* szoveg);
void deleteLastChars(char* szoveg, int bytes);
void fillLastChars(char* szoveg, int bytes, int maxBytes);
char ShowMenuInside(int latogatoszam, VisStruct* visitors, ShowStruct* shows, int currShowNumber) ;
void AddNewPerson(VisStruct* currVisitors, int addedVisitorsNum, ShowStruct* shows, int currShowNumber);
char ShowMenu(int latogatoszam, VisStruct* visitors, ShowStruct* shows, int eloadasSzam);
void ListVisitors();
void ModifyVisitor(VisStruct* visitors, int currVisNumber, ShowStruct* shows, int currShowNumber);
int getNumOfAddedVisitors();
int getCurrVisitors(VisStruct* currVisitors);
void writeToFile();
int getCurrShows(ShowStruct* shows);
void AddNewShow(VisStruct* visitors, int currVisNumber, ShowStruct* shows, int currShowNumber);
void writeShowDataToFile(ShowStruct* shows, int currShowNumber);
void ListShows(VisStruct* visitors, int addedVisitorsNum, int currShowNumber, ShowStruct* shows);
void DeleteShow(VisStruct* visitors, int currVisNumber, ShowStruct* shows, int currShowNumber);
void doShows(VisStruct* visitors, int currVisNumber, ShowStruct* shows, int currShowNumber);

void sighandler(int signum) {
	printf("CAUGHT SIGNAL!\n", signum);
}

void waitingForVisitors(int signum) {
}

void visitorsArrived(int signum) {
	printf("Megerkeztek a vendegek!\n");
}

void showEndedSignal(int signum) {
	printf("A rendezveny lezajlott! Sikeressege: %i\n", rand()%100);
}

struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

int sendM(int id) {
	const struct uzenet uz = { 5, "Azok a szemelyek akik nem jottek el: " }; 
	int status; 
	
	status = msgsnd(0, &uz, strlen ( uz.mtext ) + 1 , 0 ); 
	// a 3. param ilyen is lehet: sizeof(uz.mtext)
	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
	if ( status < 0 ) 
		perror("msgsnd"); 
		
}



int isShowExists(int currShowNumber, ShowStruct* shows, int id) {
    int i = 0;
    int isFound = 0;
    
    for (; i < currShowNumber; ++i) {
        int num;
        sscanf(shows[i].showID, "%d", &num);
        if (num == id) {
            isFound = 1;
        } 
    }
    
    return isFound;
}

void writeShowDataToFile(ShowStruct* shows, int currShowNumber) {
	int fd = open("shows.txt", O_CREAT | O_RDWR,  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd < 0) {
		
	}
	else {
		lseek(fd, 0, SEEK_SET);
	}
	
	
	char currShowNumberToChar[5];
	sprintf(currShowNumberToChar, "%ld", currShowNumber);
	write(fd, currShowNumberToChar, sizeof(currShowNumberToChar));
	
	int i = 0;
	
	if (currShowNumber > 0) {
		for (; i < currShowNumber; ++i) {
			lseek(fd, ((i) * sizeof(ShowStruct)) + 10, SEEK_SET);
			write(fd, &shows[i], sizeof(ShowStruct));
		}
	}
    close(fd);
}


void writeDataToFile(int currVisitorsNumber, VisStruct* currVisitors, ShowStruct* shows, int currShowNumber) {
	int fd = open("visitors.txt", O_CREAT | O_RDWR | O_TRUNC,  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    
	if (fd < 0) {
		
	}
	else {
		lseek(fd, 0, SEEK_SET);
	}
	
	
	char currVisNumberToChar[5];
	sprintf(currVisNumberToChar, "%ld", currVisitorsNumber);
	write(fd, currVisNumberToChar, sizeof(currVisNumberToChar));
	
	int i = 0;
	
	if (currVisitorsNumber > 0) {
		for (; i < currVisitorsNumber; ++i) {
			lseek(fd, ((i) * sizeof(VisStruct)) + 10, SEEK_SET);
			write(fd, &currVisitors[i], sizeof(VisStruct));
		}
	}
    
	writeShowDataToFile(shows, currShowNumber);
}


void deleteLastChar(char* szoveg) {
    int i = 0;
    while (szoveg[i] != '\n' && i < 50) {
        i++;
    }

    if (!(i == 50)) {
        szoveg[i] = '\0';
    }
}

void deleteLastChars(char* szoveg, int bytes) {
    int i = 0;
    while (szoveg[i] != '\n' && i < bytes) {
        i++;
    }

    if (!(i == 50)) {
        szoveg[i] = '\0';
    }
}

void deleteBinaryChars(char* szoveg, int maxBytes) {
	int i = maxBytes;
	for (; i > 0 && szoveg[i] != '\0'; i--) {
		szoveg[i] = '\0';
	}
}

void fillLastChars(char* szoveg, int bytes, int maxBytes) {
	int i = 0;
	while (szoveg[i] != '\n' && i < bytes) {
        i++;
    }

	for (; i < maxBytes; i++) {
		szoveg[i] = '\0';
	}
	
	
	szoveg[maxBytes] = '\0';
}

char ShowMenuInside(int latogatoszam, VisStruct* visitors, ShowStruct* shows, int currShowNumber) {
	
	char title[] = "King of Stands\n";
    char question[] = "Kerem adja meg a kivant menupontot(1-7): ";

    char choosenMenu[2] ;
	

    write(1, title, sizeof(title));
	write(1, title, 0);

	char intToChar[3];
	char showIntToChar[3];
	sprintf(intToChar, "%ld", latogatoszam);
	sprintf(showIntToChar, "%ld", currShowNumber);
	deleteBinaryChars(intToChar, 3);
	deleteBinaryChars(showIntToChar, 3);
	
	char latogatokSzama[24] = "Jelenlegi latogatoszam: ";
	write(1, latogatokSzama, sizeof(latogatokSzama));
    write(1, intToChar, sizeof(intToChar));
	
	write(STDOUT_FILENO, "\n", sizeof("\n"));
	
	write(1, "Hozzaadott eloadasok szama: ", sizeof("Hozzaadott eloadasok szama: "));
	write(1, showIntToChar, sizeof(showIntToChar));

	write(STDOUT_FILENO, "\n", sizeof("\n"));
	
    const char menuPoints[7][40] = {"1. Uj latogato felvetele", "2. Latogatok listazasa", "3. Adatmodositas\n", "4. Kilepes es mentes\n", "5. Rendezveny felvetele", "6. Rendezvenyek megtekintese", "7. Rendezveny torlese"};

    write(STDOUT_FILENO, "\n", sizeof("\n"));

    int i = 0;

    for (i = 0; i < sizeof(menuPoints)/sizeof(menuPoints[0]); ++i) {
        write(STDOUT_FILENO, menuPoints[i], sizeof(menuPoints[i]) - 1);
        write(STDOUT_FILENO, "\n", sizeof("\n"));
    }

    write(STDOUT_FILENO, "\n", sizeof("\n"));


    while (choosenMenu[0] != '1' && choosenMenu[0] != '2' && choosenMenu[0] != '3' && choosenMenu[0] != '4' && choosenMenu[0] != '5' && choosenMenu[0] != '6' && choosenMenu[0] != '7' && choosenMenu[0] != '8') {
        write(STDOUT_FILENO, question, sizeof(question)-1);
        read(0, choosenMenu, 2);

        write(STDOUT_FILENO, "\n", sizeof("\n"));
    }


    switch (choosenMenu[0]) {
    case '1':
        AddNewPerson(visitors, latogatoszam, shows, currShowNumber);
        break;
    case '2':
        ListVisitors(latogatoszam, visitors, shows, currShowNumber);
        break;
    case '3':
        ModifyVisitor(visitors, latogatoszam, shows, currShowNumber);
        break;
	case '4':
		writeDataToFile(latogatoszam, visitors, shows, currShowNumber);
		break;
	case '5':
		AddNewShow(visitors, latogatoszam, shows, currShowNumber);
		break;
    case '6':
        ListShows(visitors, latogatoszam, currShowNumber, shows);
        break;
    case '7':
        DeleteShow(visitors, latogatoszam, shows, currShowNumber);
        break;
    }
	case '8':
        doShows(visitors, latogatoszam, shows, currShowNumber);
        break;
    }

    return choosenMenu[0];

}


void AddNewPerson(VisStruct* currVisitors, int addedVisitorsNum, ShowStruct* shows, int currShowNumber) {

    
   int c = 0;

    VisStruct newVisitor;
	

    const char menuPoints[3][50] = {"1. Latogato neve: ", "2. Latogato e-mail: ", "3. Rendezveny azon.: "};


    write(1, menuPoints[0], 41);
    int numOfBytesInName = read(0, newVisitor.name, 51);
	fillLastChars(newVisitor.name, numOfBytesInName, 50);

    write(1, menuPoints[1], 41);
    int numOfBytesInEmail = read(0, newVisitor.email, 51);
	fillLastChars(newVisitor.email, numOfBytesInEmail, 50);

    write(1, menuPoints[2], 41);
    int numOfBytesInShowID = read(0, newVisitor.showID, 6);
	fillLastChars(newVisitor.showID, numOfBytesInShowID, 5);
    
    int isShowFound = 0;
    for (c = 0; c < currShowNumber; c++) {
        if (strcmp(newVisitor.showID, shows[c].showID) == 0) {
            isShowFound = 1;
        }
        else {
        }       
   }
   
   if (isShowFound == 0) {
       write(1, "Nincs ilyen azonositoju rendezveny!\n", sizeof("Nincs ilyen azonositoju rendezveny!\n"));

       ShowMenuInside(addedVisitorsNum, currVisitors, shows, currShowNumber);    
   }
   else {

	
	int increasedVisNum = addedVisitorsNum + 1;
	char numOfRegToChar[5];
	newVisitor.numOfReg = increasedVisNum;
	sprintf(numOfRegToChar, "%ld", increasedVisNum);
	deleteBinaryChars(numOfRegToChar, 5);
	
	
	newVisitor.addedTime = time(0);
    
   
  
	
	currVisitors[addedVisitorsNum] = newVisitor;
    
   
    
    write(1, "Uj latogato hozzadva: ",  sizeof("Uj latogato hozzadva: "));
	write(1, numOfRegToChar, sizeof(numOfRegToChar));
	
   
   write(1, "\n", sizeof("\n"));
   ShowMenuInside(increasedVisNum, currVisitors, shows, currShowNumber);
   }
	
}

char ShowMenu(int latogatoszam, VisStruct* visitors, ShowStruct* shows, int eloadasSzam) {
    char title[] = "King of Stands\n";
    char question[] = "Kerem adja meg a kivant menupontot(1-7): ";

    char choosenMenu[1] ;
	

    write(1, title, sizeof(title));
	write(1, title, 0);

	char intToChar[3];
	char showIntToChar[3];
	sprintf(intToChar, "%ld", latogatoszam);
	sprintf(showIntToChar, "%ld", eloadasSzam);
	deleteBinaryChars(intToChar, 3);
	deleteBinaryChars(showIntToChar, 3);
	
	char latogatokSzama[24] = "Jelenlegi latogatoszam: ";
	write(1, latogatokSzama, sizeof(latogatokSzama));
    write(1, intToChar, sizeof(intToChar));
	
	write(STDOUT_FILENO, "\n", sizeof("\n"));
	
	write(1, "Hozzaadott eloadasok szama: ", sizeof("Hozzaadott eloadasok szama: "));
	write(1, showIntToChar, sizeof(showIntToChar));

	write(STDOUT_FILENO, "\n", sizeof("\n"));
	
    const char menuPoints[7][40] = {"1. Uj latogato felvetele", "2. Latogatok listazasa", "3. Adatmodositas\n", "4. Kilepes es mentes\n", "5. Rendezveny felvetele", "6. Rendezvenyek megtekintese", "7. Rendezveny torlese"};

    write(STDOUT_FILENO, "\n", sizeof("\n"));

    int i = 0;

    for (i = 0; i < sizeof(menuPoints)/sizeof(menuPoints[0]); ++i) {
        write(STDOUT_FILENO, menuPoints[i], sizeof(menuPoints[i]) - 1);
        write(STDOUT_FILENO, "\n", sizeof("\n"));
    }

    write(STDOUT_FILENO, "\n", sizeof("\n"));

    while (choosenMenu[0] != '1' && choosenMenu[0] != '2' && choosenMenu[0] != '3' && choosenMenu[0] != '4' && choosenMenu[0] != '5' && choosenMenu[0] != '6' && choosenMenu[0] != '7') {
        write(STDOUT_FILENO, question, sizeof(question)-1);
        read(0, choosenMenu, 2);

        write(STDOUT_FILENO, "\n", sizeof("\n"));
    }


    switch (choosenMenu[0]) {
    case '1':
        AddNewPerson(visitors, latogatoszam, shows, eloadasSzam);
        break;
    case '2':
        ListVisitors(latogatoszam, visitors, shows, eloadasSzam);
        break;
    case '3':
        ModifyVisitor(visitors, latogatoszam, shows, eloadasSzam);
        break;
	case '4':
		writeDataToFile(latogatoszam, visitors, shows, eloadasSzam);
		break;
	case '5':
		AddNewShow(visitors, latogatoszam, shows, eloadasSzam);
		break;
    case '6':
        ListShows(visitors, latogatoszam, eloadasSzam, shows);
        break;
    case '7':
        DeleteShow(visitors, latogatoszam, shows, eloadasSzam);
        break;
    }
	case '8':
        doShows(visitors, latogatoszam, shows, currShowNumber);
        break;
    }

    return choosenMenu[0];

}

void DeleteShow(VisStruct* visitors, int currVisNumber, ShowStruct* shows, int currShowNumber) {
    char choosenShowChar[5];
    
    write(1, "Hanyadik rendezvenyt szeretne torolni? : ", sizeof("Hanyadik rendezvenyt szeretne torolni? : "));
	int readBytes = read(0, choosenShowChar, 6);
	fillLastChars(choosenShowChar, readBytes, 5);
	
    int choosenShow;
    sscanf(choosenShowChar, "%d", &choosenShow);
    
            
    if (choosenShow <= 0 || choosenShow > currShowNumber) {
        write(1, "Nincs ilyen rendezveny!\n\n", sizeof("Nincs ilyen rendezveny!\n\n"));
        ShowMenuInside(currVisNumber, visitors, shows, currShowNumber);
    }
	
    char choosenShowID[5];
    
    int i = 0;
    for (; i < 5; ++i) {
        choosenShowID[i] = shows[choosenShow - 1].showID[i];
    }
   
    int c;
	for ( c = choosenShow - 1; c < currShowNumber - 1; ++c ) {
	    shows[c] = shows[c+1];
	}
    
    
    int cInner;
    int currVisNumberInner = currVisNumber;
    for (c = 0; c < currVisNumberInner; ++c) {
        if (strcmp(choosenShowID, visitors[c].showID) == 0) {
            for ( cInner = c; cInner < currVisNumber - 1; ++cInner ) {
	            visitors[cInner] = visitors[cInner+1];
	        }
            currVisNumberInner--;
            c = c - 1;
        }
    }
    
			
    write(1, "Az adatok torlesre kerultek!\n", sizeof("Az adatok torlesre kerultek!\n"));
	ShowMenuInside(currVisNumberInner, visitors, shows, currShowNumber - 1);
}

void AddNewShow(VisStruct* visitors, int currVisNumber, ShowStruct* shows, int currShowNumber) {
	ShowStruct newShow;
	

    const char menuPoints[2][50] = {"1. Eloadas neve: ", "2. Eloadas szama: "};

    write(1, menuPoints[0], 50);
    int numOfBytesInName = read(0, newShow.name, 51);
	fillLastChars(newShow.name, numOfBytesInName, 50);

    write(1, menuPoints[1], 50);
    int numOfBytesInShowID = read(0, newShow.showID, 6);
	fillLastChars(newShow.showID, numOfBytesInShowID, 5);
	
    int c = 0;
    int isFound = 0;
    for (; c < currShowNumber; ++c) {
        if (strcmp(newShow.showID, shows[c].showID) == 0) {
            isFound = 1;
        }
    }
    
    if (isFound == 1) {
        write(1, "Mar van ilyen rendezveny szam!\n\n", sizeof("Mar van ilyen rendezveny szam!\n\n"));
        ShowMenuInside(currVisNumber, visitors, shows, currShowNumber);
    }
    
	int increasedShowNum = currShowNumber + 1;
	char numOfRegToChar[5];
	newShow.numOfReg = increasedShowNum;
	sprintf(numOfRegToChar, "%ld", increasedShowNum);
	deleteBinaryChars(numOfRegToChar, 5);
	
	shows[currShowNumber] = newShow;
	
	write(1, "Uj eloadas hozzaadva: ",  sizeof("Uj eloadas hozzaadva: "));
	write(1, numOfRegToChar, sizeof(numOfRegToChar));
   
   write(1, "\n", sizeof("\n"));
   ShowMenuInside(currVisNumber, visitors, shows, increasedShowNum);
}


void ListVisitors(int currVisNumber, VisStruct* visitors, ShowStruct* shows, int currShowNumber) {
    int i = 0;
	
	if (currVisNumber > 0) {
		for (; i < currVisNumber; ++i) { 
			char currNum[5];
			sprintf(currNum, "%ld", i + 1);
			deleteBinaryChars(currNum, 3);
			write(1, currNum, sizeof(currNum));
			write(1, ".: ", sizeof(".: "));
			write(1, "\n", sizeof("\n"));
			write(1, " ", sizeof(" "));
			write(1, visitors[i].name, sizeof(visitors[i].name));
			write(1, "\n", sizeof("\n"));
			write(1, " ", sizeof(" "));
			write(1, visitors[i].email, sizeof(visitors[i].email));
			write(1, "\n", sizeof("\n"));
			write(1, " ", sizeof(" "));
			write(1, visitors[i].showID, sizeof(visitors[i].showID));
			write(1, "\n", sizeof("\n"));
			write(1, " ", sizeof(" "));
			
			
			
			char buff[20];
			struct tm *timeptr;
			timeptr = localtime(&visitors[i].addedTime);
			strftime(buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", timeptr);
			write(1, buff, sizeof(buff));
			write(1, "\n", sizeof("\n"));
			write(1, " ", sizeof(" "));
			sprintf(currNum, "%ld", visitors[i].numOfReg);
			deleteBinaryChars(currNum, 3);
			write(1, currNum, sizeof(currNum));
			
			write(1, "\n", sizeof("\n"));
			write(1, "\n", sizeof("\n"));
			
			
		}
	}
	ShowMenuInside(currVisNumber, visitors, shows, currShowNumber);

}

void ListShows(VisStruct* visitors, int addedVisitorsNum, int currShowNumber, ShowStruct* shows) {
   int i = 0;
	
	if (currShowNumber > 0) {
		for (; i < currShowNumber; ++i) { 
			char currNum[5];
			sprintf(currNum, "%ld", i + 1);
            deleteBinaryChars(currNum, 5);
			write(1, currNum, sizeof(currNum));
			write(1, ".: ", sizeof(".: "));
			write(1, "\n", sizeof("\n"));
			write(1, " ", sizeof(" "));
			write(1, shows[i].name, sizeof(shows[i].name));
			write(1, "\n", sizeof("\n"));
			write(1, " ", sizeof(" "));
			write(1,  shows[i].showID, sizeof(shows[i].showID));
			write(1, "\n", sizeof("\n"));
			write(1, " ", sizeof(" "));
			sprintf(currNum, "%ld", shows[i].numOfReg);
			deleteBinaryChars(currNum, 3);
			write(1, currNum, sizeof(currNum));
			
			write(1, "\n", sizeof("\n"));
			write(1, "\n", sizeof("\n"));
			
			
		}
    }
    
    ShowMenuInside(addedVisitorsNum, visitors, shows, currShowNumber);
}

void doShows(VisStruct* visitors, int currVisNumber, ShowStruct* shows, int currShowNumber) {
	
}

void ModifyVisitor(VisStruct* visitors, int currVisNumber, ShowStruct* shows, int currShowNumber) {
	
    write(1, "Modositani szeretne vagy torolni?\n", sizeof("Modositani szeretne vagy torolni?\n"));
	write(1, " 1. Modositas\n", sizeof(" 1. Modositas\n"));
	write(1, " 2. Torles\n", sizeof(" 2. Torles\n"));
	
	char choosenMenu[2];
	int choosenVis;
	char choosenVisChar[5];
	char choosenVisCharTrimmed[5];
	
	
	while (choosenMenu[0] != '1' && choosenMenu[0] != '2') {
        read(0, choosenMenu, 2);

        write(STDOUT_FILENO, "\n", sizeof("\n"));
    }
	int readBytes;
	switch(choosenMenu[0]) {
		case '1':
			write(1, "Hanyadik latogato adatait szeretne modositani? ", sizeof("Hanyadik latogato adatait szeretne modositani? "));
			readBytes = read(0, choosenVisChar, 6);
			
			sscanf(choosenVisChar, "%d", &choosenVis);
			
            if (choosenVis <= 0 || choosenVis > currVisNumber) {
                write(1, "Nincs ilyen latogato!\n\n", sizeof("Nincs ilyen latogato!\n\n"));
                ShowMenuInside(currVisNumber, visitors, shows, currShowNumber);
            }

			
			write(1, "Kerem adja meg az uj adatokat: \n ", sizeof("Kerem adja meg az uj adatokat: \n "));
			write(1, "Nev: ", sizeof("Nev: "));
			int numOfBytesInName = read(0, &visitors[choosenVis - 1].name, 51);
			fillLastChars(visitors[choosenVis - 1].name, numOfBytesInName, 50);
			write(1, "E-mail: ", sizeof("E-mail: "));
			int numOfBytesInEmail = read(0, &visitors[choosenVis - 1].email, 51);
			fillLastChars(visitors[choosenVis - 1].email, numOfBytesInEmail, 50);
			write(1, "Rendezveny szama: ", sizeof("Rendezveny szama: "));
			int numOfBytesInShowID = read(0, &visitors[choosenVis - 1].showID, 6);
			fillLastChars(visitors[choosenVis - 1].showID, numOfBytesInShowID, 5);
			
			write(1, "Az adatok modositasra kerultek!", sizeof("Az adatok modositasra kerultek!"));
			ShowMenuInside(currVisNumber, visitors, shows, currShowNumber);
			break;
		case '2':
			write(1, "Hanyadik latogato adatait szeretne torolni? ", sizeof("Hanyadik latogato adatait szeretne torolni? "));
			readBytes = read(0, choosenVisChar, 6);
			
			sscanf(choosenVisChar, "%d", &choosenVis);
			
            if (choosenVis <= 0 || choosenVis > currVisNumber) {
                write(1, "Nincs ilyen latogato!\n\n", sizeof("Nincs ilyen latogato!\n\n"));
                ShowMenuInside(currVisNumber, visitors, shows, currShowNumber);
            }
            
			int c;

			for ( c = choosenVis - 1; c < currVisNumber - 1; ++c ) {
				visitors[c] = visitors[c+1];
			}
			
			write(1, "Az adatok torlesre kerultek!\n", sizeof("Az adatok torlesre kerultek!\n"));
			ShowMenuInside(currVisNumber - 1, visitors, shows, currShowNumber);
			break;
	}
	
	
}

void writeToFile() {
    int fd = open("visitors.txt", O_RDWR | O_CREAT, S_IRWXU);

    if (fd < 0) {
	write(1, "DDERROR", sizeof("ERROR"));

    }
    else {
        lseek(fd, 0, SEEK_SET);
        write(fd, "123\n", sizeof("123\n"));
		lseek(fd, 10, SEEK_SET);
    }
    close(fd);
}

int getCurrVisitors(VisStruct* currVisitors) {
	int fd = open("visitors.txt", O_RDONLY|O_CREAT, S_IRWXU);
    int currVisNumber;
	int counter;
    char numOfVisitors[3];
    ssize_t ret_in;
	

    if (fd < 0) {
        write(1, "ERROR", sizeof("ERROR"));
		
    }
    else {
		write(1, &fd, sizeof(fd));
        lseek(fd, 0, SEEK_SET);
        int readBytes = read(fd, numOfVisitors, sizeof(numOfVisitors));
    }
	
	sscanf(numOfVisitors, "%d", &currVisNumber);
	counter = currVisNumber;
	
	
	while (counter > 0) {
		lseek(fd, (currVisNumber - counter) * sizeof(VisStruct) + 10, SEEK_SET);
		read(fd, &currVisitors[currVisNumber - counter], sizeof(VisStruct));
		counter --;
	}
    close(fd);
	return currVisNumber;
}

int getCurrShows(ShowStruct* currShows) {
	int fd = open("shows.txt", O_RDONLY|O_CREAT, S_IRWXU);
    int currShowNumber;
	int counter;
    char numOfShows[5];	

    if (fd < 0) {
        write(1, "ERROR", sizeof("ERROR"));
    }
    else {
		write(1, &fd, sizeof(fd));
        lseek(fd, 0, SEEK_SET);
        int readBytes = read(fd, numOfShows, sizeof(numOfShows));
    }
	
	sscanf(numOfShows, "%d", &currShowNumber);
	counter = currShowNumber;
	
	
	while (counter > 0) {
		lseek(fd, (currShowNumber - counter) * sizeof(ShowStruct) + 10, SEEK_SET);
		read(fd, &currShows[currShowNumber - counter], sizeof(ShowStruct));
		counter --;
	}
    close(fd);
	return currShowNumber;
}

int main()
{
    VisStruct visitors[10000];
	ShowStruct shows[10000];
	
	int addedVisitorsNumAtStart = getCurrVisitors(visitors);
	int addedShows = getCurrShows(shows);
	
	
    char intToChar[3];
	char showIntToChar[3];
	sprintf(intToChar, "%ld", addedVisitorsNumAtStart);
	
	sprintf(showIntToChar, "%ld", addedShows);

	ShowMenu(addedVisitorsNumAtStart, visitors, shows, addedShows);

    return 0;
}