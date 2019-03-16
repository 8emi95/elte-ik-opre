/*
    Tárgy: Elte IK Operációs rendszerek 2. Beadandó
    Készítette: Sándor Balázs - AZA6NL
    Leadás: 2016.11.07
	Git: https://github.com/SandorBalazsHU/elte-ik-os-king-of-stands

    A szimuláció.
*/
#ifndef SIMULATION_H
#define SIMULATION_H

    #include "../model/Events.h"

    void _stringSerializer(char*, char*);
    void _signalHandler(int);
    void _sendSignal();
    int simulation(Events*, int);

#endif