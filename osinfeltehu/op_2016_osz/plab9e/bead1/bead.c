#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void flushBuffer()
{
	int c;
	if(!feof(stdin))
		while((c = getchar()) != '\n' && c != EOF);
}

void trimStrEnd(char *str)
{
	char *ch = str + strlen(str) - 1;
	*ch = '\0';
}		

char* searchLineByBegining(char* str, FILE *f)
{
	char line[300];
	char* ret;
	while(!feof(f))
	{
		fgets(line, sizeof(line), f);
		if ( strncmp(str,line,strlen(str)) == 0)
		{	
			char *ret = line;
			return ret;
		}
	}
	return NULL;
}

char* getDataByID(int ID, FILE *f)
{
	char line[300];
	int currID;
	while (!feof(f))
	{
		char *ret = line;
		fgets(line, sizeof(line), f);
		currID = atoi(line);
		if (currID == ID)
			return ret;
	}
	return NULL;
}

int countLinesByID(int ID, FILE *f)
{
	char line[300];
	int currID;
	int count = 0;
	while(fgets(line, sizeof(line), f))
	{
		currID = atoi(line);
		if (currID == ID)
			++count;
	}
	return count;
}

int newVisitor()
{
	printf("====================\n");
	printf("Adja meg a vendeg nevet: ");
	fflush(stdin);
	flushBuffer();
	size_t size = 100;
	char *name = malloc(size);
	getline(&name,&size,stdin);
	trimStrEnd(name);
	printf("Adja meg a vendeg email cimet: ");
	char *email = malloc(size);
	getline(&email,&size,stdin);
	trimStrEnd(email);
	int visitorID = 0;
	FILE *f;
	f = fopen("VISITOR.TXT","r");
	if (f == NULL)
	{
		perror("Hiba a fajl megnyitasa kozben!\n");
		exit(1);
	}
	char line[200];
	while (!feof(f))
	{
		fgets(line, sizeof(line), f);
		visitorID = atoi(line);
	}
	++visitorID;
	fclose(f);

	f = fopen("VISITOR.TXT","a");
	fprintf(f, "%d\t%s\t%s\n", visitorID, name, email);
	fclose(f);
	
	free(name);
	free(email);

	printf("A vendeg azonositoja: %d \n", visitorID);
	return visitorID;
}

int newEvent()
{
	printf("===================\n");
	fflush(stdin);
	flushBuffer();
	size_t size = 100;
	char *name = malloc(size);
	printf("Adja meg a rendezveny nevet: ");
	getline(&name,&size,stdin);
	trimStrEnd(name);
	printf("Adja meg az rendezveny datumat: ");
	char *date = malloc(size);
	getline(&date,&size,stdin);
	trimStrEnd(date);
	printf("Adja meg a rendezveny helyet: ");
	char *venue = malloc(size);
	getline(&venue,&size,stdin);
	trimStrEnd(venue);
	int eventID = 0;	

	FILE *f;
	f = fopen("EVENT.TXT","r");
	if (f == NULL)
	{
		perror("Hiba a fajl megnyitasa kozben!\n");
		exit(1);
	}
	char line[300];
	while (!feof(f))
	{
		fgets(line, sizeof(line), f);
		eventID = atoi(line);
	}
	++eventID;
	fclose(f);

	f = fopen("EVENT.TXT","a");
	fprintf(f, "%d\t%s\t%s\t%s\n", eventID, name, date, venue);
	fclose(f);
	
	free(name);
	free(date);
	free(venue);
	printf("Az esemeny azonositoja: %d \n", eventID);
	return eventID;
}

void signUp()
{
	printf("===================\n");
	fflush(stdin);
	flushBuffer();
	int visitorID;
	int eventID;
	FILE *f;
	printf("Adja meg a vendeg azonositojat: ");
	scanf("%d",&visitorID);
	f = fopen("VISITOR.TXT","r");
	if (f == NULL)
	{
		perror("Hiba a VISITOR.TXT megnyitasa kozben!\n");
		exit(1);
	}
	if ( getDataByID(visitorID,f) == NULL)
	{
		printf("Nincs ilyen vendeg!\n");
		fclose(f);
		return;
	}
	fclose(f);
	printf("Adja meg a rendezveny azonositojat: ");
	scanf("%d",&eventID);
	f = fopen("EVENT.TXT","r");
	if (f == NULL)
	{
		perror("Hiba az EVENT.TXT megnyitasa kozben!\n");
		exit(1);
	}
	if ( getDataByID(eventID,f) == NULL)
	{
		printf("Nincs ilyen rendezveny!\n");
		fclose(f);
		return;
	}
	fclose(f);
	f = fopen("VISITS.TXT","r");
	if (f == NULL)
	{
		("Hiba a VISITS.TXT megnyitasa kozben!\n");
		exit(1);
	}
	char *conv1 = malloc(4), *conv2 = malloc(4);
	sprintf(conv1,"%d",eventID);
	strcat(conv1, "\t");
	sprintf(conv2,"%d",visitorID);
	strcat(conv1, conv2);
	if (searchLineByBegining(conv1,f) != NULL)
	{
		printf("Mar jelentkezett!\n");
		fclose(f);
		return;
	}
	time_t t = time(NULL);
	struct tm dt = *localtime(&t);
	
	fclose(f);
	f = fopen("VISITS.TXT","a");
	fprintf(f, "%s\t%d.%d.%d.\n", conv1,dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday);
	fclose(f);
	free(conv1);
	free(conv2);
	printf("Sikeres jelentkezes!\n");
	f = fopen("VISITS.TXT","r");
	printf("On a %d. jelentkezo.\n", countLinesByID(eventID,f));
	fclose(f);
}

void modifyVisitor()
{
	printf("===================\n");
	fflush(stdin);
	flushBuffer();
	FILE *old, *new;
	int visitorID;
	char line[200];
	size_t size = 100;
	char *newName = malloc(size);
	char *newEmail = malloc(size);
	printf("Adja meg a modositando szemely azonositojat: ");
	scanf("%d",&visitorID);
	old = fopen("VISITOR.TXT","r");
	if (old == NULL)
	{
		perror("Hiba a VISITOR.TXT megnyitasa kozben!\n");
		exit(1);
	}
	if ( getDataByID(visitorID,old) == NULL)
	{
		printf("Nincs ilyen vendeg!\n");
		fclose(old);
		return;
	}
	fflush(stdin);
	flushBuffer();
	printf("Adja meg az uj nevet: ");
	getline(&newName,&size,stdin);
	trimStrEnd(newName);
	printf("Adja meg az uj email cimet: ");
	getline(&newEmail,&size,stdin);
	trimStrEnd(newEmail);
	remove("VISITOR.TEMP");
	new = fopen("VISITOR.TEMP","w");
	fclose(old);
	old = fopen("VISITOR.TXT","r");
	while(fgets(line, sizeof(line), old))
	{
		if ( atoi(line) != visitorID )
		{
			fprintf(new,line);
		}
		else
		{
			char *token = strtok(line,"\t");
			token = strtok(NULL,"\t");
			char *oldName = token;
			token = strtok(NULL,"\t");
			char *oldEmail = token;
			if ( strlen(newName) == 0 ) newName = oldName;
			if ( strlen(newEmail) == 0 ) newEmail = oldEmail;
			printf("%s %s\n",newName,newEmail);
			fprintf(new,"%d\t%s\t%s\n",visitorID,newName,newEmail);
		}
	}
	fclose(old);
	fclose(new);
	remove("VISITOR.TXT");
	free(newEmail);
	free(newName);
	rename("VISITOR.TEMP", "VISITOR.TXT");
}

void deleteVisitor()
{
	printf("===================\n");
	fflush(stdin);
	flushBuffer();
	FILE *old, *new;
	int visitorID;
	char line[200];
	printf("Adja meg a torlendo szemely azonositojat: ");
	scanf("%d",&visitorID);
	//
	// VISITOR.TXT karbantartása
	//
	old = fopen("VISITOR.TXT","r");
	if (old == NULL)
	{
		perror("Hiba a VISITOR.TXT megnyitasa kozben!\n");
		exit(1);
	}
	remove("VISITOR.TEMP");
	new = fopen("VISITOR.TEMP","w");
	while(fgets(line, sizeof(line), old))
	{
		if ( atoi(line) != visitorID )	fprintf(new,line);
	}
	fclose(old);
	fclose(new);
	remove("VISITOR.TXT");
	rename("VISITOR.TEMP", "VISITOR.TXT");
	//
	// VISITS.TXT karbantartása
	//
	old = fopen("VISITS.TXT","r");
	if (old == NULL)
	{
		perror("Hiba a VISITS.TXT megnyitasa kozben!\n");
		exit(1);
	}
	new = fopen("VISITS.TEMP","w");
	while(fgets(line, sizeof(line), old))
	{
		char *token = strtok(line,"\t");
		int eventID = atoi(token);
		token = strtok(NULL, "\t");
		if (token == NULL) break;
		int currVisitorID = atoi(token);
		token = strtok(NULL, "\t");
		char *date = token;
		if (currVisitorID != visitorID) fprintf(new,"%d\t%d\t%s\n", eventID, currVisitorID, date);
	}
	fclose(old);
	fclose(new);
	remove("VISITS.TXT");
	rename("VISITS.TEMP", "VISITS.TXT");
}

void deleteEvent()
{
	printf("====================\n");
	fflush(stdin);
	flushBuffer();
	FILE *old, *new;
	int eventID;
	char line[300];
	printf("Adja meg a torlendo rendezveny azonositojat: ");
	scanf("%d",&eventID);
	//
	// EVENT.TXT karbantartása
	//
	old = fopen("EVENT.TXT","r");
	if (old == NULL)
	{
		perror("Hiba az EVENT.TXT megnyitasa kozben!\n");
		exit(1);
	}
	remove("EVENT.TEMP");
	new = fopen("EVENT.TEMP","w");
	while(fgets(line, sizeof(line), old))
	{
		if ( atoi(line) != eventID )	fprintf(new,line);
	}
	fclose(old);
	fclose(new);
	remove("EVENT.TXT");
	rename("EVENT.TEMP", "EVENT.TXT");
	//
	// VISITS.TXT karbantartása
	//
	old = fopen("VISITS.TXT","r");
	if (old == NULL)
	{
		perror("Hiba a VISITS.TXT megnyitasa kozben!\n");
		exit(1);
	}
	new = fopen("VISITS.TEMP","w");
	while(fgets(line, sizeof(line), old))
	{
		char *token = strtok(line,"\t");
		int currEventID = atoi(token);
		token = strtok(NULL, "\t");
		if (token == NULL) break;
		int visitorID = atoi(token);
		token = strtok(NULL, "\t");
		char *date = token;
		if (currEventID != eventID) fprintf(new,"%d\t%d\t%s\n", currEventID, visitorID, date);
	}
	fclose(old);
	fclose(new);
	remove("VISITS.TXT");
	rename("VISITS.TEMP", "VISITS.TXT");
}

void listAssignees()
{
	printf("===================\n");
	fflush(stdin);
	flushBuffer();
	FILE *visits, *visitor;
	visits = fopen("VISITS.TXT","r");
	if (visits == NULL)
	{
		perror("Hiba a VISITS.TXT megnyitasa kozben!\n");
		exit(1);
	}
	printf("Adja meg a rendezveny azonositojat: ");
	int eventID;
	scanf("%d",&eventID);
	printf("A megadott rendezvenyre jelentkezok adatai:\n -- Lista eleje --\n");
	char line[100];
	while (!feof(visits))
	{
		fgets(line, sizeof(line), visits);
		char *token = strtok(line, "\t");
		int currEvent = atoi(token);
		token = strtok(NULL, "\t");
		if (token == NULL) break;
		int currVisitor = atoi(token);
		if ( currEvent == eventID )
		{
			visitor = fopen("VISITOR.TXT","r");
			if (visitor == NULL)
			{
				perror("Hiba a VISITOR.TXT megnyitasa kozben!\n");
				fclose(visits);
				exit(1);
			}
			printf( getDataByID(currVisitor, visitor) );
			fclose(visitor);
		}
	}
	printf(" -- Lista vege --\n");
	fclose(visits);
}


int main()
{
	char c = 'a';
	while (c != 'q')
	{
		printf("King of Stands nyilvantartas\n");
		printf("1 : Uj vendeg\n");
		printf("2 : Uj rendezveny\n");
		printf("3 : Vendeg jelentkezese\n");
		printf("4 : Vendeg modositasa\n");
		printf("5 : Vendeg torlese\n");
		printf("6 : Rendezveny torlese\n");
		printf("7 : Rendezvenyre jelentkezettek listazasa\n");
		printf("q : Kilepes\n");
		scanf(" %c", &c);
		switch(c)
		{
		case '1' : newVisitor(); break;
		case '2' : newEvent(); break;
		case '3' : signUp(); break;
		case '4' : modifyVisitor(); break;
		case '5' : deleteVisitor(); break;
		case '6' : deleteEvent(); break;
		case '7' : listAssignees(); break;
		}
	}
	return 0;
}
