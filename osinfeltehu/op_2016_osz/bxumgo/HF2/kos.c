//#include <ctype.h> //checks and converts characters
#include <fcntl.h>
//#include <stdarg.h> //Változó argumentumok kezelése - ha parancsokat adnék át függvényeknek.
//#include <stdbool.h> //boooool
#include <stdio.h>	//Fájl kezelés + kiiratás
#include <stdlib.h>	//Typus konverziók
#include <string.h> //memóriaterület másolás/mozgatás, string hasonlítás, keresés
#include <time.h> //idő kezelés
#include <signal.h> //signalok

void			AskForHelp();
int				ClearEvent();
struct event*	CreateEvent();
void			DestroyEvent();
int				FindEventByEventID();
int				FindEventByName();
int				FindGuestByEmail();
void			Help();
int				JoinEvent();
void			ListEvents();
int				ListGuests();
int				ModifyGuest();
int				ReadEvents();
struct guest*	ReadGuests();
int				RemoveGuest();
void			SignalHandler();
int				StartNextEvent();
int				WriteEventsFile();

void KillAll();

struct event
{
	int					EventID;
	char				EventName[32];
	int					NumOfGuests;
};

struct guest
{
	char				Name[32];
	char				Email[32];
	time_t				Join;
};

int EventsFile;		//Created when the the program initialize, so it can be accessed everywhere.
int NumOfEvents;
int LastID;

int main(int argc, char** argv)
{
	printf("\n");
	
	if (argc <= 1 || argv[1][0] != '/')	//Filtering the uncommanded executions.
	{
		printf("No command.\n");
		AskForHelp();
		return 1;
	}
	
	char* Command = argv[1];
	EventsFile = open("Events", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);	//Tries to open the Events file.
	NumOfEvents = 0;
	LastID = 0;
	
	if (read(EventsFile, &NumOfEvents, sizeof(NumOfEvents)) == 0		//If it is not possible to read out the two variable...
		|| read(EventsFile, &LastID, sizeof(LastID)) == 0)
	{
		write(EventsFile, &NumOfEvents, sizeof(NumOfEvents));		//...writes them in. (Maybe the file should be recreated)
		write(EventsFile, &LastID, sizeof(LastID));
	}
	
	struct event Events[NumOfEvents + 1];
	
	if (ReadEvents(&Events) != NumOfEvents)				//Tries to read as many events as it was told...
	{
		printf("Some events could've been lost\n");		//...and prints out if it couldn't.
	}
	
	if (strcmp(Command, "/ClearEvent") == 0)
	{
		if (argc != 3)
		{
			printf("Not correct amount of arguments.\n");
			AskForHelp();
			return 1;
		}
		
		int ErrorCode = ClearEvent(Events, atoi(argv[2]));
		if (ErrorCode == 1)
		{
			printf("There is no event with %s EventID!\n\n", argv[2]);
			return 1;
		}
	}
		
	else if (strcmp(Command, "/CreateEvent") == 0)				//Creating a new Event
	{
		if (argc != 3)										//Checks if there are enough arguments
		{
			printf("Not correct amount of arguments.\n");
			AskForHelp();
			return 1;
		}
		
		if (strcmp(argv[2], "Events") == 0 || strcmp(argv[2], "kos") == 0 || strcmp(argv[2], "kos.c") == 0)		//Checks some special file names that can't be used
		{
			printf("You can't name an Event 'Events', 'kos', or 'kos.c'!\n\n");
			return 1;
		}
		
		int FoundEvent = FindEventByName(Events, argv[2]);
		if (FoundEvent != NumOfEvents)								//Checks if an event already exists with the same name.
		{
			printf("An event with %s name already exists with the EventID of %d\n\n", Events[FoundEvent].EventName, Events[FoundEvent].EventID);
			return 1;
		}
		
		struct event *NewEvent = CreateEvent(argv[2]);		//Creates the new Event with the given name.
		
		if (NewEvent == NULL)								//Checks if it didn't happen for some reason.
		{
			printf("Couldn't create the ", argv[2], " Event.\n\n");
			return 1;
		}
		
		Events[NumOfEvents - 1] = *NewEvent;
		WriteEventsFile(Events, -1);							//Writes out all the all and refreshed events.
		
		printf("%s event was successfully created with %d EventID.\n\n", argv[2], LastID);	//Informs the user about the success of the event creation.
	}
	
	else if (strcmp(Command, "/DestroyEvent") == 0)			//Destroying an event
	{
		if (argc != 4)										//Checks if there are enough arguments
		{
			printf("Not correct amount of arguments.\n");
			AskForHelp();
			return 1;
		}
		
		int EventNumber;
		
		if (strcmp(argv[2], "/ByName") == 0)
		{
			EventNumber = FindEventByName(Events, argv[3]);
			if (EventNumber == NumOfEvents)
			{
				printf("There is no event with the name %s\n\n", argv[3]);
				return 1;
			}
		}
		
		else if (strcmp(argv[2], "/ByEventID") == 0)
		{
			EventNumber = FindEventByEventID(Events, atoi(argv[3]));
			if (EventNumber == NumOfEvents)
			{
				printf("There is no event with the EventID %s\n\n", argv[3]);
				return 1;
			}
		}
		
		else
		{
			printf("You have to specify the searching method for the event.\n");
			AskForHelp();
			return 1;
		}
		
		DestroyEvent(Events, EventNumber);
		WriteEventsFile(Events, EventNumber);
		printf("%s event was successfully destroyed.\n\n", Events[EventNumber].EventName);
	}
	
	else if (strcmp(Command, "/JoinEvent") == 0)
	{
		if (argc != 5)
		{
			printf("Not correct amount of arguments.\n");
			AskForHelp();
			return 1;
		}
		
		int ErrorCode = JoinEvent(Events, atoi(argv[2]), argv[3], argv[4]);
		if (ErrorCode == 1)
		{
			printf("There is no event with %s EventID!\n\n", argv[2]);
			return 1;
		}
		
		else if (ErrorCode == 2)
		{
			printf("A guest with the email %s already joined this event.\n\n", argv[4]);
			return 1;
		}
		
		else if (ErrorCode != 0)
		{
			printf("Unknown error\n\n");
			return 1;
		}
	}
	
	else if (strcmp(Command, "/Help") == 0)	//Lists all the commands.
		Help();
	
	else if (strcmp(Command, "/ListEvents") == 0)	//Lists all the existing events.
		ListEvents(Events);
		
	else if (strcmp(Command, "/ListGuests") == 0)
	{
		if (argc != 3)
		{
			printf("Not correct amount of arguments.\n");
			AskForHelp();
			return 1;
		}
		
		int ErrorCode = ListGuests(Events, atoi(argv[2]));
		if (ErrorCode == 1)
		{
			printf("There is no event with %s EventID!\n\n", argv[2]);
			return 1;
		}
		
		if (ErrorCode == 2)
		{
			printf("There are no guests at the event.\n\n");
			return 1;
		}
	}
	
	else if (strcmp(Command, "/ModifyGuest") == 0)	//Modify the name of a guest, found by email address
	{
		if (argc != 5)
		{
			printf("Not correct amount of arguments.\n");
			AskForHelp();
			return 1;
		}
		
		int ErrorCode = ModifyGuest(Events, atoi(argv[2]), argv[3], argv[4]);
		if (ErrorCode == 1)
		{
			printf("There is no event with %s EventID!\n\n", argv[2]);
			return 1;
		}
		
		else if (ErrorCode == 2)
		{
			printf("A guest with the e-mail %s hasn't joined this event.\n\n", argv[4]);
			return 1;
		}
		
		else if (ErrorCode != 0)
		{
			printf("Unknown error\n\n");
			return 1;
		}
	}
	
	else if (strcmp(Command, "/RemoveGuest") == 0)	//Removes a guest, found by email address
	{
		if (argc != 4)
		{
			printf("Not correct amount of arguments.\n");
			AskForHelp();
			return 1;
		}
		
		int ErrorCode = RemoveGuest(Events, atoi(argv[2]), argv[3]);
		if (ErrorCode == 1)
		{
			printf("There is no event with %s EventID!\n\n", argv[2]);
			return 1;
		}
		
		else if (ErrorCode == 2)
		{
			printf("A guest with the e-mail %s hasn't joined this event.\n\n", argv[4]);
			return 1;
		}
		
		else if (ErrorCode != 0)
		{
			printf("Unknown error\n\n");
			return 1;
		}
	}
		
	else if (strcmp(Command, "/StartNextEvent") == 0) //Sends out child to organize the event with the next ID
	{
		if (argc != 2)
		{
			printf("Not correct amount of arguments.\n");
			AskForHelp();
			return 1;
		}
		
		int ErrorCode = StartNextEvent(Events);
		if (ErrorCode == 1)
		{
			printf("There is no event to start.\n\n");
			return 1;
		}
		if (ErrorCode == 2)
		{
			printf("Couldn't open a pipe.\n\n");
			return 1;
		}
		
	}
	
	else if (strcmp(Command, "/KillAll") == 0)	//Developer tool, not public for users.
		KillAll(Events);
	
	else	//Invalid command.
	{
		printf("Not a valid command.\n");
		AskForHelp();
	}
	
	return 0;
}

void AskForHelp(void)
{
	printf("For more information start the program with /Help command.\n\n");
}

int ClearEvent(struct event *Events, int EventID)
{
	int EventNumber = FindEventByEventID(Events, EventID);
	if (EventNumber == NumOfEvents)
	{
		return 1;
	}
	Events[EventNumber].NumOfGuests = 0;
	int EventFile = open(Events[EventNumber].EventName, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	WriteEventsFile(Events, -1);
	printf("%s event is cleared.\n\n", Events[EventNumber].EventName);
	
	return 0;
}

struct event* CreateEvent(char* EventName)
{
	struct event *NewEvent = malloc(sizeof(struct event));
	NewEvent->EventID = LastID + 1;
	memcpy(NewEvent->EventName, EventName, sizeof(EventName));
	
	int newFile = open(EventName, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (newFile < 0)
		return NULL;
	
	++LastID;
	++NumOfEvents;
	
	return NewEvent;
}

void DestroyEvent(struct event *Events, int EventNumber)
{
	remove(Events[EventNumber].EventName);
	--NumOfEvents;
}

int FindEventByEventID(struct event *Events, int EventID)
{
	int i = 0;
	
	for (i = 0; i < NumOfEvents; ++i)
	{
		if (Events[i].EventID == EventID)
			break;
	}
	
	return i;
}

int FindEventByName(struct event *Events, char* EventName)
{
	int i = 0;
	
	for (i = 0; i < NumOfEvents; ++i)
	{
		if (strcmp(Events[i].EventName, EventName) == 0)
			break;
	}
	
	return i;
}

int FindGuestByEmail(struct guest *Guests, int NumOfGuests, char *Email)
{
	int i;
	for (i = 0; i < NumOfGuests; ++i)
	{
		if (strcmp(Guests[i].Email, Email) == 0)
			break;
	}
	
	return i;
}

void Help(void)
{
	printf("List of possible commands:\n\n");
	printf("   %-57s\t%s\n", "/ClearEvent EventID", "Clears an event with the given EventID");
	printf("   %-57s\t%s\n", "/CreateEvent Event_Name[32]", "Creates a new event with the given name");
	printf("   %-57s\t%s\n", "/DestroyEvent /ByName Event_Name[32]", "Finds and destroys an event with the given name");
	printf("   %-57s\t%s\n", "/DestroyEvent /ByEventID EventID", "Finds and destroys an event with the given EventID");
	printf("   %-57s\t%s\n", "/JoinEvent EventID Guest_Name[32] email@address.com[32]", "Registrates a new guest to an event");
	printf("   %-57s\t%s\n", "/Help", "Lists the possible commands");
	printf("   %-57s\t%s\n", "/ListEvents", "Lists all the events");
	printf("   %-57s\t%s\n", "/ListGuests EventID", "Lists the guests of the given event");
	printf("   %-57s\t%s\n", "/ModifyGuest EventID Guest_Name[32] email@address.com[32]", "Modifies a guest's name with the given e-mail address");
	printf("   %-57s\t%s\n", "/RemoveGuest EventID email@address.com[32]", "Removes a guest with the given e-mail address from an event");
	printf("   %-57s\t%s\n", "/StartNextEvent", "Sends out partner to organize the next event, then deletes it");
	printf("\n");
}

int JoinEvent(struct event *Events, int EventID, char* GuestName, char* Email)
{
	int EventNumber = FindEventByEventID(Events, EventID);
	
	if (EventNumber == NumOfEvents)
		return 1;
	
	struct guest *Guests = ReadGuests(&(Events[EventNumber]));
	
	int GuestNumber = FindGuestByEmail(Guests, Events[EventNumber].NumOfGuests, Email);
	if (GuestNumber !=  Events[EventNumber].NumOfGuests)
		return 2;
	
	memcpy(Guests[GuestNumber].Name, GuestName, sizeof(char) * 32);
	memcpy(Guests[GuestNumber].Email, Email, sizeof(char) * 32);
	Guests[GuestNumber].Join = time(NULL);
	++Events[EventNumber].NumOfGuests;
	
	int EventFile = open(Events[EventNumber].EventName, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	int i;
	for (i = 0; i < Events[EventNumber].NumOfGuests; ++i)
	{
		write(EventFile, &(Guests[i]), sizeof(struct guest));
	}
	
	WriteEventsFile(Events, -1);
	
	printf("You have successfully joined the %s event as the #%d guest.\n\n", Events[EventNumber].EventName, Events[EventNumber].NumOfGuests);
	
	return 0;
}

void ListEvents(struct event *Events)
{
	if (NumOfEvents == 0)
	{
		printf("There are no events.\n\n");
		return;
	}
	
	int i;
	
	printf("List of the events:\n");
	printf("%7s\t%-32s\t%12s\n", "EventID", "Event Name", "Guests");
	for(i = 0; i < NumOfEvents; ++i)
	{
		printf("%7d\t%-32s\t%12d\n", Events[i].EventID, Events[i].EventName, Events[i].NumOfGuests);
	}
	printf("\n");
}

int ListGuests(struct event *Events, int EventID)
{
	int EventNumber = FindEventByEventID(Events, EventID);
	if (EventNumber == NumOfEvents)
		return 1;
	
	if (Events[EventNumber].NumOfGuests == 0)
		return 2;
	
	struct guest *Guests = ReadGuests(&(Events[EventNumber]));
	
	printf("List of the guests of the %s event:\n\n", Events[EventNumber].EventName);
	printf("  %3s\t%-32s\t%-32s\t%s\n", "#", "Name", "E-mail address", "Joined at");
	int i;
	for (i = 0; i < Events[EventNumber].NumOfGuests; ++i)
	{
		printf("  %3d\t%-32s\t%-32s\t%s", i, Guests[i].Name, Guests[i].Email, asctime(localtime(&(Guests[i].Join))));
	}
	printf("\n");
	return 0;
}

int ModifyGuest(struct event *Events, int EventID, char* GuestName, char* Email)
{
	int EventNumber = FindEventByEventID(Events, EventID);
	
	if (EventNumber == NumOfEvents)
		return 1;
	
	struct guest *Guests = ReadGuests(&(Events[EventNumber]));
	
	int GuestNumber = FindGuestByEmail(Guests, Events[EventNumber].NumOfGuests, Email);
	if (GuestNumber ==  Events[EventNumber].NumOfGuests)
		return 2;
	
	memcpy(Guests[GuestNumber].Name, GuestName, sizeof(char) * 32);
	
	int EventFile = open(Events[EventNumber].EventName, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	int i;
	for (i = 0; i < Events[EventNumber].NumOfGuests; ++i)
	{
		write(EventFile, &(Guests[i]), sizeof(struct guest));
	}
		
	printf("You have modified the name of the guest to %s.\n\n", Guests[GuestNumber].Name);
	
	return 0;
}

int ReadEvents(struct event *Events)
{
	int CurrEventID;
	char CurrEventName[32];
	int CurrNumOfGuests;
	struct event *CurrEvent;
	int i = 0;
	
	for (i = 0; i < NumOfEvents; ++i)
	{
		if (read(EventsFile, &CurrEventID, sizeof(CurrEventID)) == 0 || read(EventsFile, &CurrEventName, sizeof(CurrEventName)) == 0
			|| read(EventsFile, &CurrNumOfGuests, sizeof(CurrNumOfGuests)) == 0)
		{
			printf ("Event file was probably damaged.");
			return i-1;
		}
		
		CurrEvent = (struct event *)malloc(sizeof(*CurrEvent));
		CurrEvent->EventID = CurrEventID;
		memcpy(CurrEvent->EventName, &CurrEventName, sizeof(CurrEventName));
		CurrEvent->NumOfGuests = CurrNumOfGuests;
		
		Events[i] = *CurrEvent;
	}
	return i;
}

struct guest* ReadGuests(struct event* Event)
{
	int EventFile = open(Event->EventName, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	struct guest *Guests = malloc(sizeof(struct guest) * (Event->NumOfGuests + 1));
	
	int i;
	for (i = 0; i < Event->NumOfGuests; ++i)
	{
		if (read(EventFile, &(Guests[i]), sizeof(struct guest)) == 0)
		{
			printf("The file of the event was probably damaged\n\n");
			return NULL;
		}
	}
	
	close(EventFile);
	return Guests;
}

int RemoveGuest(struct event *Events, int EventID, char* Email)
{
	int EventNumber = FindEventByEventID(Events, EventID);
	
	if (EventNumber == NumOfEvents)
		return 1;
	
	struct guest *Guests = ReadGuests(&(Events[EventNumber]));
	
	int GuestNumber = FindGuestByEmail(Guests, Events[EventNumber].NumOfGuests, Email);
	if (GuestNumber ==  Events[EventNumber].NumOfGuests)
		return 2;
	
	int EventFile = open(Events[EventNumber].EventName, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	int i;
	for (i = 0; i < Events[EventNumber].NumOfGuests; ++i)
	{
		if (i == GuestNumber)
			continue;
		write(EventFile, &(Guests[i]), sizeof(struct guest));
	}
	--Events[EventNumber].NumOfGuests;
	
	WriteEventsFile(Events, -1);
	
	printf("You have successfully removed %s from the %s event.\n\n", Guests[GuestNumber].Name, Events[EventNumber].EventName);
	
	return 0;
}

void SignalHandler(int signumber)
{
	if (signumber == SIGUSR1)
		printf("%-14s:\tPartner arrived\n", "King of Stands");
	else
		printf("%-14s:\tThe event ended!\n", "King of Stands");
}

int StartNextEvent(struct event *Events)
{
	if (NumOfEvents == 0)
		return 1;
	
	int pid, pipefd[2];
	if (pipe(pipefd) == -1)
		return 2;
	
	pid = fork();
	if (pid > 0)	//Parent
	{
		struct sigaction sigact;
		sigact.sa_handler = SignalHandler;
		sigemptyset(&sigact.sa_mask);
		sigact.sa_flags = 0;
		sigaction(SIGUSR1, &sigact, NULL);
		sigaction(SIGUSR2, &sigact, NULL);
		
		printf("%-14s:\tStarting to organize %s...\n", "King of Stands", Events[0].EventName);
		write(pipefd[1], Events[0].EventName, sizeof(Events[0].EventName));
		
		sigset_t sigset;
		sigfillset(&sigset);
		sigdelset(&sigset, SIGUSR1);
		sigdelset(&sigset, SIGUSR2);
		sigsuspend(&sigset);
		
		printf("%-14s:\tSending participants...\n", "King of Stands");
		char message[32];
		sprintf(message, "%d", Events[0].NumOfGuests);
		write(pipefd[1], message, sizeof(message));
		
		struct guest *Guests = ReadGuests(&(Events[0]));
		int i;
		for (i = 0; i < Events[0].NumOfGuests; ++i)
		{
			write(pipefd[1], Guests[i].Name, sizeof(Guests[i].Name));
			write(pipefd[1], Guests[i].Email, sizeof(Guests[i].Email));
		}
		
		fflush(stdout);
		close(pipefd[1]);
		sigsuspend(&sigset);
		
		read(pipefd[0], message, sizeof(message));
		int rate = atoi(message);
		printf("%-14s:\tThe event was rated %d/10\n", "King of Stands", rate);
		
		read(pipefd[0], message, sizeof(message));
		int numnotarrived = atoi(message);
		if (numnotarrived == 0)
			printf("%-14s:\tNoone missed the event\n", "King of Stands");
		else
		{
			printf("%-14s:\t%d person%s missed the event:\n", "King of Stands", numnotarrived, numnotarrived == 1 ? "" : "s");
			for (i = 0; i < numnotarrived; ++i)
			{
				read(pipefd[0], message, sizeof(message));
				printf("\t\t\t%s\n", message);
			}
			printf("%-14s:\tThey won't get their money back!\n", "King of Stands");
		}
		
		int status;
		wait(&status);
		printf("%-14s:\tMy partner killed himself :(\n", "King of Stands");
		
		if(NumOfEvents == 1)
			printf("%-14s:\tThere is no next event!\n\n", "King of Stands");
		else
			printf("%-14s:\tThe next event will be: %d %s\n\n", "King of Stands", Events[1].EventID, Events[1].EventName);
		
		DestroyEvent(Events, 0);
		WriteEventsFile(Events, 0);
	}
	
	if (pid == 0)	//Child
	{
		char message[32];
		read(pipefd[0], message, sizeof(message));
		printf("%-14s:\tTraveling to %s...\n", "Partner", message);
		sleep(1);
		kill(getppid(), SIGUSR1);
		
		read(pipefd[0], message, sizeof(message));
		int numparts = atoi(message);
		printf("%-14s:\tWaiting for %d participants...\n", "Partner", numparts);
		
		struct guest Guests[numparts];
		int numnotarrived = 0, notarrived[numparts];
		memset(notarrived, 0, sizeof(int) * numparts);
		srand(getpid());
		
		int i;
		for (i = 0; i < numparts; ++i)
		{
			read(pipefd[0], Guests[i].Name, sizeof(Guests[i].Name));
			read(pipefd[0], Guests[i].Email, sizeof(Guests[i].Email));
			if (rand() % 10 == 0)
			{
				++numnotarrived;
				++notarrived[i];
			}
		}
		
		close(pipefd[0]);
		
		printf("%-14s:\tList of the guests:\n", "Partner");
		printf("  %3s\t%-32s\t%-32s\t%s\n", "#", "Name", "E-mail address", "Arrived");
		for (i = 0; i < numparts; ++i)
		{
			printf("  %3d\t%-32s\t%-32s\t%s\n", i, Guests[i].Name, Guests[i].Email, notarrived[i] ? "No" : "Yes");
		}
		
		kill(getppid(), SIGUSR2);
		
		sprintf(message, "%d", rand() % 11);
		write(pipefd[1], message, sizeof(message));
		
		sprintf(message, "%d", numnotarrived);
		write(pipefd[1], message, sizeof(message));
		
		for (i = 0; i < numparts; ++i)
			if (notarrived[i])
				write(pipefd[1], Guests[i].Name, sizeof(Guests[i].Name));
		
		sleep(1);
		exit(0);	//Suicide so it won't return to the main program.
	}
	return 0;
}

int WriteEventsFile(struct event *Events, int nToSkip) //nToSkip <= -1 if you don't want any of them to be skipped
{
	if (Events == NULL)
	{
		return 1;
	}
	
	int skipper = 0;	//Helps to jump the i of the recently deleted event
	
	close(EventsFile);
	EventsFile = open("Events", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	
	write(EventsFile, &NumOfEvents, sizeof(NumOfEvents));
	write(EventsFile, &LastID, sizeof(LastID));
	
	int i;
	for (i = 0; i < NumOfEvents + skipper; ++i)
	{
		if (i == nToSkip)
		{
			skipper = 1;
			continue;
		}
		
		write(EventsFile, &(Events[i].EventID), sizeof(Events[i].EventID));
		write(EventsFile, &(Events[i].EventName), sizeof(char)*32);
		write(EventsFile, &(Events[i].NumOfGuests), sizeof(Events[i].NumOfGuests));
	}
	
	return 0;
}

void KillAll(struct event *Events)
{
	close(EventsFile);
	
	int i;
	for (i = 0; i < NumOfEvents; ++i)
		remove(Events[i].EventName);
	
	remove("Events");
	
	printf("T͘͝͏H̵̨̛E͜ ̡S͡À̶C̷R̡͟I̶F͏I̴͡C̡È͏͡ H̷͘A̴̧̧S͡͞ ͠B͘E̴̶͟E̴͜N͏͝ ̷͝M̷̵̨A̧D͝E̢͡!\n\n");
}