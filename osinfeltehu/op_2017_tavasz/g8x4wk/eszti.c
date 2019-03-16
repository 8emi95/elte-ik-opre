#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

struct messageBuffer
{
	long messageType;
	int messageText;
};

void menu();

void newDestination();
void showDestinations();
void deleteDestination();
void newPassenger();
void updatePassenger();
void deletePassenger();
void destinationInfo();
void trip();

int searchFirstData(char targetFileName[50], char target[50]);
int searchFirstAndSecondData(char targetFileName[50], char firstTarget[50], char secondTarget[50]);
void deleteData(char targetFileName[50], int lineNumber);
void signalHandler(int s) { _exit(0); }

int main()
{
	int choice = 1;
	int hiba = 0;
	int end = 0;
	
	printf("\n*** BEST OF UBER ***\n");
	
	while(choice != 9 && !end)
	{
		if(!hiba) { menu(); }
		
		while(!scanf("%d", &choice))
		{
			scanf("%s");
			printf("\nA megadott parancs nem ismerheto fel! Ujra: ");
		}
		getchar();
		
		hiba = 0;
		
		switch(choice)
		{
			case 1:
				showDestinations();
				break;
			case 2:
				newDestination();
				break;
			case 3:
				deleteDestination();
				break;
			case 4:
				newPassenger();
				break;
			case 5:
				updatePassenger();
				break;
			case 6:
				deletePassenger();
				break;
			case 7:
				destinationInfo();
				break;
			case 8:
				trip();
				end = 1;
				break;
			case 9: 
				printf("\nViszont latasra! A megadott adatok mentesre kerultek!\n");
				break;
			default:
				printf("\nA megadott parancs nem ismerheto fel! Ujra: ");
				hiba = 1;
				break;
		}
	}
}

void menu()
{
	printf("\nKerem valasszon az alabbi menupontokbol!\n");
	printf("\n1. Celallomasok megtekintese\n");
	printf("2. Uj celallomas felvetele\n");
	printf("3. Celallomas torlese\n");
	printf("4. Uj jelentkezes felvetele\n");
	printf("5. Jelentkezo telefonszamanak modositasa\n");
	printf("6. Jelentkezes torlese\n");
	printf("7. Jaratinformacio utaslistaval\n");
	printf("8. Jarat inditasa\n");
	printf("9. Kilepes a programbol\n");
	printf("\nParancs: ");
}

void newDestination()
{
	FILE* schedule;
	schedule = fopen("schedule.txt", "a+");
	
	char city[50];
	char departure[18];
	
	printf("\nAz uj celallomas neve: ");
	fgets(city, 50, stdin);
	city[strlen(city)-1] = '\0';
	
	if(!(searchFirstData("schedule.txt", city)))
	{
		printf("A jarat indulasi napja (ev.honap.nap. ora:perc formaban): ");
		fgets(departure, 18, stdin);
		fprintf(schedule, "%s, %s\n", city, departure);
	}
	else
	{
		printf("\nA megadott celallomas mar letezik!\n");
	}
	
	fclose(schedule);
}

void showDestinations()
{
	FILE* schedule;
	int count = 0;
	
	if(schedule = fopen("schedule.txt", "r"))
	{
		char line[50];
		
		while(fgets(line, 50, schedule) != NULL)
		{
			count++;
			
			if(count == 1)
			{
				printf("\nA lehetseges celallomasok: \n");
			}
			
			char act[50];
			int i = 0;
			
			while(line[i] != ',')
			{
				act[i] = line[i];
				i++;
			}
			
			act[i] = '\0';
			printf("%s\n", act);
		}
		
		if(count == 0)
		{
			printf("\nNincs megjelenitheto celallomas!\n");
		}
		
		fclose(schedule);
	}
	else
	{	
		printf("\nNincs megjelenitheto celallomas!\n");
	}
}

void deleteDestination()
{
	char target[50];
	
	printf("\nA torolni kivant celallomas: ");
	fgets(target, 50, stdin);
	
	int lineNumber = searchFirstData("schedule.txt", target);
	
	if(!lineNumber)
	{
		printf("A megadott celallomas nem szerepel az adatbazisban!\n");
	}
	else
	{
		if(!(searchFirstData("passengers.txt", target)))
		{
			deleteData("schedule.txt", lineNumber);
			printf("A megadott celallomas torlesre kerult!\n");
		}
		else
		{
			printf("A megadott jarat nem torolhet, mert vannak ra jelentkezok!\n");
		}
		
	}
}

void newPassenger()
{
	FILE* passengers;
	passengers = fopen("passengers.txt", "a+");
	
	char name[50];
	char destination[50];
	char phone[20];
	int year;
	int month;
	int day;
	int hour;
	int minute;
	
	printf("Jelentkezo uticelja: ");
	fgets(destination, 50, stdin);
	
	if(!(searchFirstData("schedule.txt", destination)))
	{
		printf("\nA megadott uticel nem szerepel a lehetseges celallomasok kozott!\n");
	}
	else
	{
		printf("Jelentkezo neve: ");
		fgets(name, 50, stdin);
		name[strlen(name)-1] = 0;
		destination[strlen(destination)-1] = '\0';
		
		if(searchFirstAndSecondData("passengers.txt", destination, name))
		{
			printf("\nA megadott szemely mar jelentkezett erre a jaratra!\n");
		}
		else
		{
			printf("Jelentkezo telefonszama: ");
			fgets(phone, 18, stdin);
			phone[strlen(phone)-1] = 0;
			
			
			time_t rawtime;
			struct tm *timeInfo;
			time (&rawtime);
			timeInfo = localtime(&rawtime);
			year = timeInfo -> tm_year+1900;
			month = timeInfo -> tm_mon+1;
			day = timeInfo -> tm_mday;
			hour = timeInfo -> tm_hour;
			minute = timeInfo -> tm_min;
			
			fprintf(passengers, "%s, %s, %s, %d.%d.%d %d:%d\n", destination, name, phone, year, month, day, hour, minute);
		}
	}
	
	fclose(passengers);
}

void updatePassenger()
{
	char name[50];
	char destination[50];
	char date[18];
	char newPhone[20];
	
	printf("A jelentkezo uticelja: ");
	fgets(destination, 50, stdin);
	
	if(!(searchFirstData("schedule.txt", destination)))
	{
		printf("\nA megadott uticel nem szerepel a lehetseges celallomasok kozott!\n");
	}
	else
	{
		printf("Jelentkezo neve: ");
		fgets(name, 50, stdin);
		name[strlen(name)-1] = 0;
		destination[strlen(destination)-1] = '\0';
		
		int index = searchFirstAndSecondData("passengers.txt", destination, name);
		
		if(index)
		{
			FILE* passengers;
			passengers = fopen("passengers.txt", "a+");
			
			int count = 0;
			char line[140];
			
			while(fgets(line, 140, passengers) != NULL && count < index - 1) { count++; }
			
			int i = 0;
			int j = 0;
			int k = 0;
			
			while(k <= strlen(line))
			{
				if(line[k] == ',')
				{ 
					i++; 
					j = 0;
					k++;
				}
				
				if(i == 3) 
				{ 
					date[j] = line[k];
					j++;
				}
				
				k++;
			}
			
			fclose(passengers);
			
			printf("A jelentkezo uj telefonszama: ");
			fgets(newPhone, 20, stdin);
			newPhone[strlen(newPhone)-1] = '\0';
			date[strlen(date)-1] = '\0';
			
			deleteData("passengers.txt", index);
			passengers = fopen("passengers.txt", "a+");
			fprintf(passengers, "%s, %s, %s, %s\n", destination, name, newPhone, date);
			fclose(passengers);
			printf("\nA jelentkezo telefonszamanak modositasa megtortent\n");
			
			
		}
		else
		{
			printf("\nAz adatbazisban nem szerepel a megadottaknak megfelelo adat!\n");
		}
	}
}

void deletePassenger()
{
	char name[50];
	char destination[50];
	
	printf("\nJelentkezo uticelja: ");
	fgets(destination, 50, stdin);
	
	if(!(searchFirstData("schedule.txt", destination)))
	{
		printf("\nA megadott uticel nem szerepel a lehetseges celallomasok kozott!\n");
	}
	else
	{
		printf("Jelentkezo neve: ");
		fgets(name, 50, stdin);
		name[strlen(name)-1] = 0;
		destination[strlen(destination)-1] = '\0';
		
		int index = searchFirstAndSecondData("passengers.txt", destination, name);
		
		if(index)
		{
			deleteData("passengers.txt", index);
			printf("\nA megadott jelentkezes torlesre kerult!\n");
		}
		else
		{
			printf("\n Az adatbazisban nem szerepel a megadottaknak megfelelo adat!\n");
		}
	}
}

void destinationInfo()
{
	char destination[50];
	printf("\nA kivant celallomas: ");
	fgets(destination, 50, stdin);
	
	int index = searchFirstData("schedule.txt", destination);
	
	if(!index)
	{
		printf("\nA megadott celallomas nem szerepel az adatbazisban!\n");
	}
	else
	{
		FILE* schedule;
		schedule = fopen("schedule.txt", "r");
		char sline[80];
		int count = 0;
		
		while(fgets(sline, 80, schedule) != NULL && count < index)
		{
			count++;
			
			if(count == index) { printf("\n%s", sline); }
		}
	
		fclose(schedule);
		
		FILE* passengers;
		passengers = fopen("passengers.txt", "r");
		char line[140];
		count = 0;
		destination[strlen(destination)-1] = '\0'; 
		
		printf("\nUtaslista:\n");
		
		while(fgets(line, 140, passengers) != NULL)
		{
			char actFirst[50];
			int i = 0;
			
			while(line[i] != ',')
			{
				actFirst[i] = line[i];
				i++;
			}
			
			actFirst[i] = '\0';
			
			if(strlen(actFirst) == strlen(destination))
			{
				int j = 0;
				int countSecond = 0;
				while(actFirst[j] == destination[j] && j<strlen(actFirst)) { j++; }
				if(j == strlen(actFirst)) 
				{	
					count++;
					char actSecond[50];
					int k = i+2;
					int m = 0;
					
					while(line[k] != ',')
					{
						actSecond[m] = line[k];
						k++;
						m++;
					}
					
					actSecond[m] = '\0';
					
					printf("%s\n", actSecond);
				}
			}
		}
		
		if(!count){ printf("Nincs megjelenitheto utas!\n"); }
		
		fclose(passengers);
	}
}

int searchFirstData(char targetFileName[50], char target[50])
{
	int lineNumber = 0;
	FILE* targetFile;
	
	if(targetFile = fopen(targetFileName, "r"))
	{
		char line[50];
		int count = 0;
	
		while(fgets(line, 50, targetFile) != NULL && lineNumber == 0)
		{
			count++;
			char act[50];
			int i = 0;
			
			while(line[i] != ',')
			{
				act[i] = line[i];
				i++;
			}
			
			act[i] = '\0';
			
			if(strlen(act) == strlen(target)-1)
			{
				int j = 0;
				while(act[j] == target[j]) { j++; }
				if(j == strlen(act)) 
				{ 
					lineNumber = count; 
				}
			}
		}
		
		fclose(targetFile);
	}
	
	return lineNumber;
}

int searchFirstAndSecondData(char targetFileName[50], char firstTarget[50], char secondTarget[50])
{
	int lineNumber = 0;
	FILE* targetFile;
	
	if(targetFile = fopen(targetFileName, "r"));
	{
		char line[140];
		int count = 0;
	
		while(fgets(line, 140, targetFile) != NULL && lineNumber == 0)
		{
			count++;
			char actFirst[50];
			int i = 0;
			
			while(line[i] != ',')
			{
				actFirst[i] = line[i];
				i++;
			}
			
			actFirst[i] = '\0';
			
			if(strlen(actFirst) == strlen(firstTarget))
			{
				int j = 0;
				int countSecond = 0;
				while(actFirst[j] == firstTarget[j] && j<strlen(actFirst)) { j++; }
				if(j == strlen(actFirst)) 
				{	
					char actSecond[50];
					int k = i+2;
					int m = 0;
					
					while(line[k] != ',')
					{
						actSecond[m] = line[k];
						k++;
						m++;
					}
					
					actSecond[m] = '\0';
					
					if(strlen(actSecond) == strlen(secondTarget))
					{
						int l = 0;
						while(actSecond[l] == secondTarget[l] && l<strlen(actSecond)) { l++; }
						if(l == strlen(actSecond)) 
						{ 
							lineNumber = count;
						}
					}
				}
			}
		}
		
		fclose(targetFile);
	}
	
	return lineNumber;
}

void deleteData(char targetFileName[50], int lineNumber)
{
	int count = 0;
	char line[80];
	FILE* targetFile;
	targetFile = fopen(targetFileName, "r");
	char copyFileName[50] = "help.txt";
	FILE* copyFile;
	copyFile = fopen(copyFileName, "w");
		
	while(fgets(line, 80, targetFile) != NULL)
	{
		count++;
		if(count != lineNumber)
		{
			fprintf(copyFile, "%s", line);
		}
	}
		
	fclose(copyFile);
	fclose(targetFile);
	remove(targetFileName);
	rename(copyFileName, targetFileName);
}

void trip()
{
	char listOfPassengers[5000] = "";
	int numberOfPassengers = 0;
	int passengersPosition[50];

	char destination[50];
	printf("\nA kivant celallomas: ");
	fgets(destination, 50, stdin);
	
	int index = searchFirstData("schedule.txt", destination);
	int lineNumber = index;

	if(!index)
	{
		printf("\nA megadott celallomas nem szerepel az adatbazisban!\n");
	}
	else
	{
		//utaslista kinyerése		
		FILE* schedule;
		schedule = fopen("schedule.txt", "r");
		char sline[80];
		int count = 0;
		int lineIndex = 0;
		
		while(fgets(sline, 80, schedule) != NULL && count < index)
		{
			count++;
			
			if(count == index) { printf("\n%s\n", sline); }
		}
	
		fclose(schedule);
		
		FILE* passengers;
		passengers = fopen("passengers.txt", "r");
		char line[140];
		count = 0;
		destination[strlen(destination)-1] = '\0'; 
		
		while(fgets(line, 140, passengers) != NULL)
		{
			lineIndex++;
			char actFirst[50];
			int i = 0;
			
			while(line[i] != ',')
			{
				actFirst[i] = line[i];
				i++;
			}
			
			actFirst[i] = '\0';
			
			if(strlen(actFirst) == strlen(destination))
			{
				int j = 0;
				int countSecond = 0;
				while(actFirst[j] == destination[j] && j<strlen(actFirst)) { j++; }

				if(j == strlen(actFirst)) 
				{	
					count++;
					char actSecond[50];
					int k = i+2;
					int m = 0;
					
					while(line[k] != ',')
					{
						actSecond[m] = line[k];
						k++;
						m++;
					}
					
					actSecond[m] = '\n';

					strcat(listOfPassengers, actSecond);
					passengersPosition[numberOfPassengers] = lineIndex;	
					numberOfPassengers++;	
				}
			}	
		}
		
		if(!count) { printf("Nincs megjelenitheto utas!\n"); }
		
		fclose(passengers);
	}
	
	if(index)
	{
		int n=-1;
		int pfd[2];
		pid_t child;
		char receivedPassengers[5000]; 

		key_t key = 2012;
		int messageID;
		int status;
		struct messageBuffer messageQueue;

	    	messageID = msgget(key, IPC_CREAT | 0600);

		if (messageID < 0)
		{
			perror("\nUzenetsor: A kirandulas az idegenvezeto betegsege miatt elmarad!\n");
			exit(1);
		}

		if (pipe(pfd) < 0)
		{
	    	perror("\nCsovezetek: A kirandulas az idegenvezeto betegsege miatt elmarad!\n");
			exit(1);
		}

		child = fork();
		if (child < 0) 
		{
	    	perror("\nFork: A kirandulas az idegenvezeto betegsege miatt elmarad!\n");
			exit(1);

		} 
		else
		{
			if(child == 0) 
			{
				//jelzés
				signal(SIGUSR1, signalHandler);
				
				//csõvezeték
				close(pfd[1]); 
				n = read(pfd[0], receivedPassengers, 5000);
				close(pfd[0]);

				printf("\nAz idegenvezeto megkapta az utaslistat!\n");
			
				int opinions[numberOfPassengers];

				messageQueue.messageType = 100;

				srand(time(NULL));

				int i = 0;
				for(i=0; i<numberOfPassengers; i++)
				{
					int r = rand() % 5 + 1;
					messageQueue.messageText = r;

					status = msgsnd(messageID, &messageQueue, sizeof(messageQueue.messageText), 0);
					if(status < 0)
					{
						perror("\nHiba a velemenyek kuldese soran!\n");
					}
				}

				printf("\nAz idegenvezeto elkuldte a tarsasagnak a velemenyeket!\n");		
				
				wait(NULL);
				printf("Az idegenvezeto megkapta a jelzest, hogy befejezheti a tevekenyseget!");
			}
			else
			{
				//csõvezeték
				close(pfd[0]);
				write(pfd[1], listOfPassengers, sizeof(listOfPassengers));
				close(pfd[1]);

				printf("A tarsasag elkuldte az idegenvezetonek az utaslistat!\n");

				waitpid(child, &status, 0);
				
				//üzenetsor
				int i=0;	
				int receivedOpinions[numberOfPassengers];		
				for(i=0; i<numberOfPassengers; i++)
				{
					status = msgrcv(messageID, &messageQueue, sizeof(int), 100, 0);
					receivedOpinions[i] = messageQueue.messageText;
				}

				printf("\nA tarsasag megkapta az idegenvezetotol a velemenyeket! Eredmenyek:\n");

				//eredmények kiírása
				int length = strlen(listOfPassengers);
				i = 0;
				index = 0;

				while(index != numberOfPassengers)
				{
					char act[50];
					int j = 0;
			
					while(listOfPassengers[i] != '\n')
					{
						act[j] = listOfPassengers[i];
						j++;
						i++;
						
					}
			
					act[j] = '\0';
					i++;
			
					printf("%s %d\n", act, receivedOpinions[index]);
					index++;

				}

				//jelzés
				kill(child, SIGUSR1);
				printf("\nA tarsasag elkuldte az idegenvezetonek a jelzest, hogy befejezheti a tevekenyseget!\n\n");
				exit(0);
			}
		}

		int close(int pfd);

		deleteData("schedule.txt", lineNumber);

		int i=0;
		for(i=0; i<numberOfPassengers; i++)
		{
			passengersPosition[i] -= i;
			deleteData("passengers.txt", passengersPosition[i]);
		}
	}
}
