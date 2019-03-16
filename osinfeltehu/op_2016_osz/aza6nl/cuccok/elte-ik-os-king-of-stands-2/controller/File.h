/*
    Tárgy: Elte IK Operációs rendszerek 1. Beadandó
    Készítette: Sándor Balázs - AZA6NL
	Git: https://github.com/SandorBalazsHU/elte-ik-os-1

    A fájlkezelésért felelős függvények
*/

#ifndef FILE_H
#define FILE_H
    #include <stdio.h>
    #include "../model/Events.h"
    #include "../model/Event.h"
    #include "../model/Visitor.h"

    static const char _dataFileName[] = "kos.data";
    void eventsWriterForBinFiles(Events*);
    void eventWriterForBinFiles(Event*, FILE*);
    void visitorWriterForBinFiles(Visitor*, FILE*);
    Events* eventsReaderForBinFiles();
    Event* eventReaderForBinFiles(FILE*);
    Visitor* visitorReaderForBinFiles(FILE*);
    FILE* openBinFileForRead(const char*);
    FILE* openBinFileForWrite(const char*);
    int getFileSize(FILE*);

#endif