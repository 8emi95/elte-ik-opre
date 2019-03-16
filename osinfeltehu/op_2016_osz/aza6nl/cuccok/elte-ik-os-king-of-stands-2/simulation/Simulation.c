/*
    Tárgy: Elte IK Operációs rendszerek 2. Beadandó
    Készítette: Sándor Balázs - AZA6NL
    Leadás: 2016.11.07
	Git: https://github.com/SandorBalazsHU/elte-ik-os-king-of-stands

    A szimuláció.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "./Simulation.h"
#include "../model/Events.h"
#include "../model/Event.h"
#include "../view/ConsoleIO.h"

void _signalHandler(int signumber)
{
  if(signumber == SIGTERM) simulationStateMessage("KOS(Szülő)", getpid(), "SIGTERM - szignál érkezett!");
  simulationMessage("A szervező megérkezett a helyszínre!");
}

void _sendSignal()
{
	simulationStateMessage("Partner(gyerek)", getpid(), "SIGTERM - szignál elküldve!");
	kill(getpid(), SIGTERM);
}

int simulation(Events* events, int eventID)
{
    printMessage("A szimuláció Elkezdődött!");
	pid_t parentPid = getpid();
	simulationStateMessage("KOS(Szülő)", parentPid, "Fut!");
	pid_t childPid;

	//Pipes
	char pipeBuffer[30];
	//pipeDown
    int pipeDown[2];
	if (pipe(pipeDown) == -1) 
	{
		errorMessage("Hiba a pipe nyitaskor!");
		return 1;
	}

	//pipeUp
    int pipeUp[2];
	if (pipe(pipeUp) == -1) 
	{
		errorMessage("Hiba a pipe nyitaskor!");
		return 1;
	}

	//Signal
	signal(SIGTERM,_signalHandler);

	//Fork
	childPid = fork();
	if (childPid == -1) 
	{
		errorMessage("Fork hiba!");
		return 1;
	}

	//Folyamatok
	if (childPid == 0) 
	{
		//Gyerek
		pid_t myChildPid = getpid();
		simulationStateMessage("Partner(gyerek)", myChildPid, "Fut!");

		sleep(3);
		close(pipeDown[1]);
		close(pipeUp[0]);

		//A rendezvény helyszínének fogadása a leszálló csőből.
		read(pipeDown[0], pipeBuffer, sizeof(pipeBuffer));
		simulationStateMessage("Partner(gyerek)", myChildPid, "Az olvasott üzenet:");
		simulationMessage("A rendezvény helyszíne: ");
		simulationMessage(pipeBuffer);

		//A rendezvény sikerességénekelküldése
		srand(time(NULL));
		int eventRate = 1+rand()%100;
		char eventRateStringTmp[15];
		sprintf(eventRateStringTmp, "%d", eventRate);
		_stringSerializer(eventRateStringTmp, pipeBuffer);
		write(pipeUp[1], pipeBuffer, sizeof(pipeBuffer));

		//A résztvevőlista fogadása a leszállócsőből
		//és a meg nem érkezettek visszaküldése a felszálló csövön.
		simulationMessage("A rendezvény résztvevői: ");
		read(pipeDown[0], pipeBuffer, sizeof(pipeBuffer));
		char *garbage = NULL;
		int eventSize = strtol(pipeBuffer, &garbage, 0);
		int i; for(i=0; i<eventSize; i++)
		{
			read(pipeDown[0], pipeBuffer, sizeof(pipeBuffer));
			simulationMessage(pipeBuffer);
			if((1+rand()%100) < 90) pipeBuffer[0] = '\0';
			write(pipeUp[1], pipeBuffer, sizeof(pipeBuffer));
		}
		close(pipeDown[0]);
		close(pipeUp[1]);

		//Signal
		_sendSignal();

		simulationStateMessage("Partner(gyerek)", myChildPid, "Leáll!");
		exit(EXIT_SUCCESS);
	} 
	else 
	{
		//Szülő
		Event* event = getEventFromEventsById(events, eventID);
		close(pipeDown[0]);
		close(pipeUp[1]);
		
		//A helyszín küldése a leszállócsőbe
		char* eventName = getEventFromEventsById(events, 0)->name;
		_stringSerializer(eventName, pipeBuffer);
		write(pipeDown[1], pipeBuffer, sizeof(pipeBuffer));
		fflush(NULL);

		//A résztvevők küldése a leszállócsőbe
		char eventSizeToStringTmp[15];
		sprintf(eventSizeToStringTmp, "%d", event->size);
		_stringSerializer(eventSizeToStringTmp, pipeBuffer);
		write(pipeDown[1], pipeBuffer, sizeof(pipeBuffer));
		int i; for(i=0; i<event->size; i++)
		{
			char* visitorName = getVisitorFromEventById(event, i)->name;
			_stringSerializer(visitorName, pipeBuffer);
			write(pipeDown[1], pipeBuffer, sizeof(pipeBuffer));
		}

		sleep(3);
		//A sikeresség fogadása a felszálócsőből
		read(pipeUp[0], pipeBuffer, sizeof(pipeBuffer));
		simulationMessage("Az esemény sikeressége:");
		simulationMessage(pipeBuffer);
		simulationMessage("A meg nem jelent vendégek:");
		for(i=0; i<event->size; i++)
		{
			read(pipeUp[0], pipeBuffer, sizeof(pipeBuffer));
			if(pipeBuffer[0] != '\0') simulationMessage(pipeBuffer);
		}

		close(pipeDown[1]);
		close(pipeUp[0]);
		fflush(NULL);

		//Bevár
		wait();
		simulationStateMessage("KOS(Szülő)", parentPid, "Leáll!");
        printMessage("A szimuláció végetért!");
	}
}

void _stringSerializer(char* in, char* buffer)
{
	strcpy(buffer, in);
}