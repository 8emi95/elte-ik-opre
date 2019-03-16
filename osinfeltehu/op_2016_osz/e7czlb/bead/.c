#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

// Készítő:     Harkai Krisztián
// Neptun kód:  E7CZLB
// E-mail cím:  krisz.harkai@gmail.com

struct visitors
{
  char name[80];
  int present; //0 nem jelent meg, 1 megjelent
};

static void handler(int signo)
{
  if (signo == SIGTERM)
        printf("A vendégek megérkeztek. \n");
}

void startEvent()
{
  const char* directoryName = "Events";
  if (!directoryExists(directoryName))
  {
      printLN("Nincs egy rendezvény sem jelenleg!");
      return;
  }
  
  printLN("Melyik eseményt szeretné kijelölni?");
  
  char name[256];
  char path[256];
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, name);
  int fileDesc = open(path, O_RDONLY);
  
  if (fileDesc < 0)
  {
    printLN("Nincs ilyen esemény!");
    return;
  }
    
  int parentToChild[2];
  pid_t cpid;

  if (pipe(parentToChild) == -1)
  {
    perror("Hiba a pipe létrehozásakor.");
    exit(EXIT_FAILURE);
  }
  
  struct msgbuf
  {
    long	mtype;
    char	mtext[80];
  } * msg;
  
  int id = 0;
  msg = (struct msgbuf*)malloc(sizeof(msg) + 1 + 80);
  
  if ((id = msgget(44, IPC_CREAT | 0666)) < 0)
  {
    perror("Nem hozható létre az üzenetsor.");
    exit(-1);
  }
  
  signal(SIGTERM, handler);
    
  cpid = fork();
  if (cpid == -1)
  {
    perror("Hiba forkolás közben.");
    exit(EXIT_FAILURE);
  }

  if (cpid == 0)
  {
    close(parentToChild[1]);
    char buffer[256];
    readLine(parentToChild[0], buffer);
    srand(time(NULL));
    int r;
    struct visitors names[20];
    int j = 0;
    int i = 0;
    
    while (readLine(parentToChild[0], buffer))
    {
      sscanf(buffer, "%s %s %d %s", names[j].name, NULL, NULL, NULL);
	  int presented = rand() % 10 + 1;
	  if(presented == 1){
		names[j].present = 0;
	  }else{
		names[j].present = 1;  
	  }
      j++;
    }
    close(parentToChild[0]);

    while (i < j)
    {
	  if(names[i].rating.present == 0){
		sprintf(buffer, "%s látogató megjelent az eseményen.", names[i].name);  
	  }
	  else{
		sprintf(buffer, "%s látogató nem jelent meg az eseményen.", names[i].name);  
	  }
      strcpy(msg->mtext, buffer);
      msg->mtype = (i % j) + 1;
      msgsnd(id, msg, strlen(msg->mtext) + 1, 0);
      i++;
    }
    pause();
    _exit(EXIT_SUCCESS);
  } else
  {
    close(parentToChild[0]);
    char buffer[256];
    int i = 0;
    int j = 1;
    
    while (readLine(fileDesc, buffer))
    {
      write(parentToChild[1], buffer, strlen(buffer));
      i++;
    }
    close(parentToChild[1]);
    while (j < i)
    {
      //wait(NULL);
      msgrcv(id, msg, 80, j - 1, IPC_NOWAIT);
      printLN(msg->mtext);
      ++j;
    }
    kill(cpid, SIGTERM);
  }
  free(msg);
  
  close(fileDesc);
}

int directoryExists(const char* absolutePath)
{
  struct stat status;
  stat( absolutePath, &status );
  return (status.st_mode & S_IFDIR) == 0;
}

 const char* currentDateTime(int add)
 {
   time_t now = time(0);
   struct tm tstruct;
   char* buff = (char*)malloc(sizeof(char) * 80);
   tstruct = *localtime(&now);
   sprintf( buff, "%d. %d. %d. %d:%d"
            , tstruct.tm_year + 1900, tstruct.tm_mon + 1
            , tstruct.tm_mday + add, tstruct.tm_hour, tstruct.tm_min);
   
   return buff;
 }
 
void printLN(const char * text)
{
  write(1, text, strlen(text));
  const char * newLine = "\n";
  write(1, newLine, strlen(newLine));
}

int writeToFile(const char* fileName, const char* line, int create)
{
  int fileDesc = open(fileName, O_WRONLY | O_APPEND, 0777);
  const char * newLine = "\n";
  
  if(fileDesc > 0 && create == 0)
    return -1;
  
  if(fileDesc < 0)
    fileDesc = open(fileName, O_WRONLY | O_CREAT, 0777);
    if(fileDesc < 0)
      return -1;
      
  write(fileDesc, line, strlen(line));
  write(fileDesc, newLine, strlen(newLine));
  
  close(fileDesc);
  return 0;
}

void listEvents()
{
  DIR* dir;
  struct dirent * dp;
  struct stat st;
  dir = opendir("Events");
  if(dir == NULL)
  {
	printLN("Jelenlegi rendezvények:");
    printLN("Nincs egyetlen rendezvény sem jelenleg!");
  } else
  {
	int i = 0;
    printLN("Jelenlegi rendezvények:");
    while( (dp = readdir(dir)) )
    { 
      stat(dp->d_name, &st);
      if(dp->d_type == 8)
      {
		i = i + 1;
        printLN(dp->d_name);
      }
    }
	if ( i == 0 )
	{
		printLN("Nincs egyetlen rendezvény sem jelenleg!");	
	}
  }
  closedir(dir);
}

void addEvent()
{
  const char* directoryName = "Events";
  if(!directoryExists(directoryName))
    mkdir(directoryName, 0777);

  printLN("Kérem adja meg a rendezvény nevét!");
  char name[256];
  char path[256];
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, name);
  writeToFile(path, currentDateTime(1), 0);
}

void deleteEvent()
{
  const char* directoryName = "Events";

  printLN("Kérem adja meg a rendezvény nevét!");
  char name[256];
  char path[256];
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, name);
  int error = remove(path);
  if(error < 0)
    printLN("A rendezvény törlése nem sikerült!");
}

int readEvent(const char* fileName)
{
  int fileDesc = open(fileName, O_RDONLY);
  
  if(fileDesc < 0)
    return -1;
    
  char c;
  while( read(fileDesc, &c, sizeof(c)) )
  {
    write(1, &c, sizeof(c));
  }
  
  close(fileDesc);
  return 0;
}

void eventInfo()
{
  const char* directoryName = "Events";
  if(!directoryExists(directoryName))
    mkdir(directoryName, 0777);

  printLN("Melyik rendezvényre kíváncsi?");
  char name[256];
  char path[256];
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, name);
  if(readEvent(path) < 0)
  {
    printLN("Nincs ilyen rendezvény!");
  }
}

void addVisitor()
{
  const char* directoryName = "Events";
  char fileName[256];
  char path[256];
  char visitor[256];
  char email[128];
  char name[128];
  if(!directoryExists(directoryName))
    mkdir(directoryName, 0777);

  printLN("Kérem adja meg a rendezvény nevét!");
  read(1, fileName, 256);
  fileName[strcspn(fileName, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, fileName);
  printLN("Kérem adja meg a látogató nevét!");
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  printLN("Kérem adja meg az email címét!");
  read(1, email, 256);
  email[strcspn(email, "\n")] = '\0';
  sprintf(visitor, "%s  %s  %s", name, email, currentDateTime(0));
  writeToFile(path, visitor, 1);
}

int readLine(int fo, char* buffer)
{
  int i = 0;
  int end = 1;
  char c;
  while( ( end = read(fo, &c, sizeof(c)) ) && c != '\n' )
  {
    buffer[i++] = c;
  }
  buffer[i++] = c;
  buffer[i] = '\0';
  return end;
}

void deleteVisitor()
{
  const char* directoryName = "Events";
  const char* temp = "deleteTemp";
  char fileName[256];
  char path[256];
  char name[128];
  if(!directoryExists(directoryName))
    mkdir(directoryName, 0777);

  printLN("Melyik rendezvény látogatóját kívánja törölni?");
  read(1, fileName, 256);
  fileName[strcspn(fileName, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, fileName);
  printLN("Kérem adja meg a nevét!");
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  
  int fo = open(path, O_RDONLY);
  int go = open(temp, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if(fo < 0 || go < 0)
  {
    printLN("Hiba keletkezett a törlés során!");
  }
  
  char buffer[256];
  while( readLine(fo, buffer) )
  {
    int dontWrite = 0;
    int j;
    for(j = 0; j < strlen(name); ++j)
    {
      if(name[j] == buffer[j])
        dontWrite++;
    }
    if(dontWrite != strlen(name))
      write(go, &buffer, strlen(buffer));
  }
  
  close(fo);
  remove(path);
  rename(temp, path);
  close(go);
}

void modifyVisitor()
{
  const char* directoryName = "Events";
  const char* temp = "deleteTemp";
  char fileName[256];
  char path[256];
  char name[128];
  if(directoryExists(directoryName))
    printLN("Nincsen egy rendezvény sem!");
  else
  {
    printLN("Melyik rendezvény látogatóját kívánja módosítani?");
    read(1, fileName, 256);
    fileName[strcspn(fileName, "\n")] = '\0';
    sprintf(path, "%s/%s", directoryName, fileName);
    printLN("Kérem adja meg a látogató nevét!");
    read(1, name, 256);
    name[strcspn(name, "\n")] = '\0';
    
    int fo = open(path, O_RDONLY);
    int go = open(temp, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    char* tempBuff = 0;
    if(fo < 0 || go < 0)
    {
      printLN("Nincs ilyen rendezvény!");
    } else
    {
      char buffer[256];
      while( readLine(fo, buffer) )
      {
        int dontWrite = 0;
        int j;
        for(j = 0; j < strlen(name); ++j)
        {
          if(name[j] == buffer[j])
            dontWrite++;
        }
        if(dontWrite != strlen(name))
          write(go, &buffer, strlen(buffer));
        
        if(dontWrite == strlen(name) && dontWrite != 0)
        {
          tempBuff = (char*)malloc(sizeof(char) * 256);
          memcpy(tempBuff, buffer, strlen(buffer) + 1);
        }
      }
      
      close(fo);
      if(!tempBuff)
      {
        printLN("Nincs ilyen nevű látogató!");
        remove(temp);
      } else
      {
        remove(path);
        rename(temp, path);
        
        char email[256];
        char visitor[256];
        char date[256];
        
        printLN("Kérem adja meg a látogató új nevét!");
        read(1, name, 256);
        name[strcspn(name, "\n")] = '\0';
        printLN("Kérem adja meg az új email címét!");
        read(1, email, 256);
        email[strcspn(email, "\n")] = '\0';
        printLN("Kérem adja meg az új időpontját a jelentkezésnek!");
        read(1, date, 256);
        date[strcspn(date, "\n")] = '\0';
        sprintf(visitor, "%s  %s  %s", name, email, date);
        writeToFile(path, visitor, 1);
        close(go);
      }
    }
  }
}

int main()
{
  printLN("Kérem válasszon az alábbi lehetőségek közül!");
  printLN("(A művelet sorszámát beírva az lefut)");
  printLN("-- Rendezvényekkel kapcsolatos műveletek --");
  printLN("-> 1. Rendezvények listázása");
  printLN("-> 2. Rendezvény felvétele");
  printLN("-> 3. Rendezvény törlése");
  printLN("-> 4. Rendezvény információk");
  printLN("-> 5. Rendezvény indítása");
  printLN("-- Látogatókkal kapcsolatos műveletek --");
  printLN("-> 6. Látogató felvétele");
  printLN("-> 7. Látogató módosítása");
  printLN("-> 8. Látogató törlése");
  printLN("-- Egyéb műveletek --");
  printLN("-> 9. Kilépés");
  char answer = ' ';
  while(answer != '8')
  {
    read(1, &answer, 2);
    switch(answer)
    {
      case '1':
        listEvents();
        break;
      case '2':
        addEvent();
        break;
      case '3':
        deleteEvent();
        break;
      case '4':
        eventInfo();
        break;
	  case '5':
        startEvent();
        break;
      case '6':
        addVisitor();
        break;
      case '7':
        modifyVisitor();
        break;
      case '8':
        deleteVisitor();
        break;
    }
  }
  return 0;
}
