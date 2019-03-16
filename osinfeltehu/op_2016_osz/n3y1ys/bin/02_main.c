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

// Készítő:     Andó Sándor Zsolt
// Neptun kód:  N3Y1YS
// E-mail cím:  andosandor601@gmail.com

struct guests
{
  char name[80];
  int isCame;
};

int readLine(int fo, char* buffer)
{
  int i = 0;
  int end = 1;
  char c;
  while ((end = read(fo, &c, sizeof(c))) && c != '\n')
  {
    buffer[i++] = c;
  }
  buffer[i++] = c;
  buffer[i] = '\0';
  return end;
}

int directoryExists(const char* absolutePath)
{
  struct stat status;
  stat( absolutePath, &status );
  return S_ISDIR(status.st_mode);
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
  int fileDesc = open(fileName, O_WRONLY | O_APPEND);
  const char * newLine = "\n";
  
  if(fileDesc > 0 && create == 0)
    return -1;
  
  if(fileDesc < 0)
    fileDesc = open(fileName, O_WRONLY | O_CREAT , 006666 | S_IWOTH);
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
  if(dir < 0)
  {
    printLN("Nincs egyetlen rendezvény se jelenleg!");
  } else
  {
    printLN("Jelenlegi rendezvények:");
    while( (dp = readdir(dir)) )
    { 
      stat(dp->d_name, &st);
      if(dp->d_type == 8)
      {
        printLN(dp->d_name);
      }
    }
  }
  closedir(dir);
}

void addEvent()
{
  const char* directoryName = "Events";
  if(!directoryExists(directoryName))
    mkdir(directoryName, 00766);

  printLN("Kérem adja meg a rendezvény azonosítóját!");
  char id[256];
  char path[256];
  read(1, id, 256);
  
  id[strcspn(id, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, id);
  writeToFile(path, id, 0); //ez még nem egyezik
}

void deleteEvent()
{
  const char* directoryName = "Events";
  
  printLN("Kérem adja meg a rendezvény azonosítóját!");
  char id[256];
  char path[256];
  read(1, id, 256);
  id[strcspn(id, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, id);
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
    mkdir(directoryName, 0666);

  printLN("Melyik rendezvényre kíváncsi?");
  char id[256];
  char path[256];
  read(1, id, 256);
  id[strcspn(id, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, id);
  if(readEvent(path) < 0)
  {
    printLN("Nincs ilyen rendezvény!");
  }
}

static void handler(int signo)
{
  if (signo == SIGTERM)
        printf("A rendezvénynek vége. \n");
}

void startEvent()
{
  const char* directoryName = "Events";
  if (!directoryExists(directoryName))
  {
      printLN("Nincs egy rendezvény se jelenleg!");
      return;
  }
  
  printLN("Melyik rendezvényt kívánja elindítani?");
  
  char name[256];
  char path[256];
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, name);
  int fileDesc = open(path, O_RDONLY);
  
  if (fileDesc < 0)
  {
    printLN("Nincs ilyen rendezvény!");
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
    char buffer[256]="";
    readLine(parentToChild[0], buffer);
    srand(time(NULL));
    int r;
    struct guests names[20];
    int j = 0;
    int i = 0;
    
    while (readLine(parentToChild[0], buffer))
    {
      sscanf(buffer, "%s %s %d %s", names[j].name, NULL, NULL, NULL);
      names[j].isCame = 1;
      if(rand() % 10 + 1 == 1){
        names[j].isCame = 0;
      }
      
      j++;
    }
    close(parentToChild[0]);

    sprintf(buffer, "rendezvény sikeressége(1-100) %d", rand() % 100 + 1);
    printLN(buffer);

    //strcpy(msg->mtext, buffer);
    //msgsnd(id, msg, strlen(msg->mtext) + 1, 0);
    
    while (i < j)
    {
      sprintf(buffer, "%s vendég megjelent-e? (1-igen/0-nem) %d", names[i].name, names[i].isCame);
      printLN(buffer);
      //strcpy(msg->mtext, buffer);
      //msg->mtype = (i % j) + 1;
      //msgsnd(id, msg, strlen(msg->mtext) + 1, 0);
      i++;
    }
    pause();
    _exit(EXIT_SUCCESS);
  } else
  {
    close(parentToChild[0]);
    char buffer[256]="";
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
      //printLN(msg->mtext);
      ++j;
    }
    kill(cpid, SIGTERM);
  }
  free(msg);
  
  close(fileDesc);
}

void addGuest()
{
  const char* directoryName = "Events";
  char fileName[256];
  char path[256];
  char buffer[256];
  char guest[256];
  char mail[128];
  char name[128];
  int lines = 0;
  if(!directoryExists(directoryName))
    mkdir(directoryName, 00766);

  printLN("Kérem adja meg a rendezvény azonosítóját!");
  read(1, fileName, 256);
  fileName[strcspn(fileName, "\n")] = '\0';
  sprintf(path, "%s/%s", directoryName, fileName);
  printLN("Kérem adja meg a nevét!");
  read(1, name, 256);
  name[strcspn(name, "\n")] = '\0';
  printLN("Kérem adja meg a mailcímét!");
  read(1, mail, 256);
  mail[strcspn(mail, "\n")] = '\0';
  sprintf(guest, "%s  %s  %s", name, mail, currentDateTime(0));
  lines = countLines(path);

  if(writeToFile(path, guest, 1) == 0)
    printLN("Sorszám: ");
  if(lines > 0)
    lines=lines-1;
  printf("%d\n", lines);
}

int countLines(const char* path)
{
  int sum = 0;
  FILE * f;
  f=fopen(path,"rb");

  if (f==NULL){perror("File opening error\n"); exit(1);}

  char line[160];
  while (!feof(f)){
    fgets(line,sizeof(line),f);
    ++sum;
  }
 
  fclose(f);

  return sum;
}

void deleteGuest()
{
  const char* directoryName = "Events";
  const char* temp = "deleteTemp";
  char fileName[256];
  char path[256];
  char name[128];
  if(!directoryExists(directoryName))
    mkdir(directoryName, 00766);

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

void modifyGuest()
{
  const char* directoryName = "Events";
  const char* temp = "deleteTemp";
  char fileName[256];
  char path[256];
  char name[128];
  if(directoryExists(directoryName))
    printLN("Nincsen egy rendezvény se!");
  else
  {
    printLN("Melyik rendezvény látogatóját kívánja módosítani?");
    read(1, fileName, 256);
    fileName[strcspn(fileName, "\n")] = '\0';
    sprintf(path, "%s/%s", directoryName, fileName);
    printLN("Kérem adja meg a nevét!");
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
        printLN("Nincs ilyen nevű vendég!");
        remove(temp);
      } else
      {
        remove(path);
        rename(temp, path);
        
        char mail[256];
        char guest[256];
        
        printLN("Kérem adja meg az új nevét!");
        read(1, name, 256);
        name[strcspn(name, "\n")] = '\0';
        printLN("Kérem adja meg az új mailcímét!");
        read(1, mail, 256);
        mail[strcspn(mail, "\n")] = '\0';
        sprintf(guest, "%s  %s  %s", name, mail, currentDateTime(0));
        writeToFile(path, guest, 1);
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
  printLN("-- Vendégekkel kapcsolatos műveletek --");
  printLN("-> 6. Látogató felvétele");
  printLN("-> 7. Látogató adatainak módosítása");
  printLN("-> 8. Látogató törlése");
  printLN("-- Egyéb műveletek --");
  printLN("-> 9. Kilépés");
  char answer = ' ';
  while(answer != '9')
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
        addGuest();
        break;
      case '7':
        modifyGuest();
        break;
      case '8':
        deleteGuest();
        break;
    }
  }
  return 0;
}
