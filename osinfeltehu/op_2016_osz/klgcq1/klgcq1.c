#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>



int AddGuest (const int eventsNumber, const int guestsNumber);
void List();
void EditGuest (const int guestsNumber);
void DeleteGuest (const int guestsNumber);
void CreateEvent (const int eventsNumber);
void DeleteEvent (const int eventsNumber);
void CopyFile (const char *from, const char *to);


int main (int argc, char** argv) 
{	
	int in = 7;
	int file;
	int fileA;
	int eventsNumber = 0;
	int guestsNumber = 0;
	int actGuest = 0;
	
	file = open ("fileB.txt", O_CREAT | O_RDWR | O_TRUNC);
	close (file);
	
	while (in != 0) 
	{
		printf ("0: Exit\n");
		printf ("1: Add guest\n");
		printf ("2: Lists guests\n");
		printf ("3: Edit guest\n");
		printf ("4: Delete guest\n");
		printf ("5: Create event\n");
		printf ("6: Delete event\n");
		scanf ("%d", &in);
		
		switch (in) 
		{
			case (1) : 
			{
				if(eventsNumber==0)
				{
					printf("There are no events available.\n");
					break;
				}
				
				++actGuest;
				++guestsNumber;
				int place = AddGuest (eventsNumber, actGuest);
				printf ("%d", place);
				printf (". guest");
				printf ("\n");
				break;
			}
			case (2) : 
			{
				List ();
				break;
			}
			case (3) : 
			{
				EditGuest (guestsNumber);
				break;
			}
			case (4) : 
			{
				DeleteGuest (guestsNumber);
				--guestsNumber;
				break;
			}
			case (5) : 
			{
				++eventsNumber;
				CreateEvent (eventsNumber);
				break;
			}
			case (6) : 
			{
				--eventsNumber;
				DeleteEvent (eventsNumber);
				break;
			}
		}

	}
	return 0;
}


int AddGuest (const int eventsNumber, const int actGuest)
{
	FILE *file;
	FILE *fileA;
	file = fopen ("fileB.txt", "r");
	fileA = fopen ("fileA.txt", "w");
	
	if (eventsNumber == 0)
		return 0;
	
	int eventID;
	char GuestName[512];
	char GuestEmail[512];
	printf ("Event ID: ");
	scanf ("%d", &eventID);
	printf ("Guest name: ");
	scanf ("%s", GuestName);
	printf ("Guest e-mail address: ");
	scanf ("%s", GuestEmail);
	
	char line[512];
	while (fgets (line, 100, file) != NULL) 
	{
		char temp[10];
		temp[0] = line[1];		
		int i=2;
		while(line[i]!=' ' && i<10)
		{
			temp[i-1]=line[i];
			i++;
		}
		fprintf (fileA ,"%s", line);
		if (line[0] == '\'' && atoi (temp) == eventID)
		{
			fprintf (fileA ,"%s", "+");
			fprintf (fileA ,"%d", actGuest);
			fprintf (fileA ,"%s", " ");
			fprintf (fileA ,"%s", GuestName);
			fprintf (fileA ,"%s", " ");
			fprintf (fileA ,"%s", GuestEmail);
			fprintf (fileA ,"%s", "\n");
		}		
	};
	
	fclose (file);
	fclose (fileA);
	CopyFile ("fileA.txt", "fileB.txt");
	return actGuest;
}


void List ()
{
	FILE *file;
	file = fopen ("fileB.txt", "r");
	
	char write[512];
	while (fgets (write, 512, file) != NULL) 
	{
		if(write[0]=='\'')printf("Event: ");
		if(write[0]=='+')printf("Guest: ");
		printf (write+1);
	};
	
	fclose (file);
}


void EditGuest (const int guestsNumber)
{
	if (guestsNumber <= 0)
		return;
	
	FILE *file;
	FILE *fileA;
	file = fopen ("fileB.txt", "r");
	fileA = fopen ("fileA.txt", "w");
	
	int visChange;
	printf ("Guest ID: ");
	scanf ("%d", &visChange);
	
	if (visChange <= 0 || visChange > guestsNumber)
		return;
	
	printf ("Guest name: ");
	char GuestName[512];
	scanf ("%s", GuestName);
	printf ("Guest e-mail address: ");
	char GuestEmail[512];
	scanf ("%s", GuestEmail);
	
	char line[512];
	while (fgets (line, 100, file) != NULL) 
	{
		char temp[10];
		temp[0] = line[1];		
		int i=2;
		while(line[i]!=' ' && i<10)
		{
			temp[i-1]=line[i];
			i++;
		}
		if (line[0] == '+' && atoi (temp) == visChange) 
		{
			fprintf (fileA ,"%s", "+");
			fprintf (fileA ,"%d", visChange);
			fprintf (fileA ,"%s", " ");
			fprintf (fileA ,"%s", GuestName);
			fprintf (fileA ,"%s", " ");
			fprintf (fileA ,"%s", GuestEmail);
			fprintf (fileA ,"%s", "\n");
		} else 
		{
			fprintf (fileA ,"%s", line);			
		}
	};
	
	fclose (file);
	fclose (fileA);
	CopyFile ("fileA.txt", "fileB.txt");
}


void DeleteGuest (const int guestsNumber)
{
	if (guestsNumber <= 0)
		return;
	
	FILE *file;
	FILE *fileA;
	file = fopen ("fileB.txt", "r");
	fileA = fopen ("fileA.txt", "w");
	
	int visDel;
	printf ("GuestID: ");
	scanf ("%d", &visDel);
	
	if (visDel <= 0 || visDel > guestsNumber)
		return;
	
	char line[512];
	while (fgets (line, 100, file) != NULL) 
	{
		char temp[10];
		temp[0] = line[1];		
		int i=2;
		while(line[i]!=' ' && i<10)
		{
			temp[i-1]=line[i];
			i++;
		}
		if (line[0] != '+') 
		{
			fprintf (fileA ,"%s", line);
		}
		if (line[0] == '+' && atoi (temp) != visDel)
		{
			fprintf (fileA ,"%s", line);
		}			
	};
	
	fclose (file);
	fclose (fileA);
	CopyFile ("fileA.txt", "fileB.txt");
}


void CreateEvent (const int eventsNumber)
{
	char eventName [512];
	FILE *file = fopen ("fileB.txt", "a+");
	printf ("Event name: ");
	scanf ("%s", eventName);
	fprintf (file ,"%s", "\'");
	fprintf (file ,"%d", eventsNumber);
	fprintf (file ,"%s", " ");
	fprintf (file ,"%s", eventName);
	fprintf (file ,"%s", "\n");
	
	fclose (file);
}


void DeleteEvent (const int eventsNumber)
{
	if (eventsNumber <= 0) return;
	
	FILE *file = fopen ("fileB.txt", "r");
	FILE *fileA = fopen ("fileA.txt", "w");
	
	int partyDel;
	printf ("Event ID: ");
	scanf ("%d", &partyDel);
	
	if (partyDel <= 0 || partyDel > eventsNumber) return;
	
	int eventMatch = 0;
	char line[512];
	while (fgets (line, 100, file) != NULL) 
	{
		char temp[10];
		temp[0] = line[1];
		if (line[0] == '\'' && eventMatch != 0) 
		{
			eventMatch = 0;
		}
		if (line[0] == '\'' && atoi (temp) == partyDel && eventMatch == 0) 
		{
			eventMatch = 1;
		}
		if (eventMatch == 0) 
		{
			fprintf (fileA ,"%s", line);
		}
	};
	
	fclose (file);
	fclose (fileA);
	CopyFile ("fileA.txt", "fileB.txt");
}


void CopyFile (const char *from, const char *to)
{
	FILE *fr;
	FILE *t;
	fr = fopen (from,"r");
	t = fopen (to,"w");
	
	char copy[512];
	
	while (fgets (copy, 512, fr) != NULL) 
	{
		fprintf (t ,"%s", copy);
	};
	
	fclose (fr);
	fclose (t);
	
	remove (from);
}