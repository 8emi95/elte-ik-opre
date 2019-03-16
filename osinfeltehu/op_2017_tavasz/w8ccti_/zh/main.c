#include "all.h"

struct request
{
  long mtype;
  pid_t phoneNumber;
  char address[256];
};

struct acknowledgement
{
  long mtype;
  char message[256];
};

char *arrivalFIFO = "arrivalFIFO";
int messageQueue;

// Parent and Car

int passengerReplied = 0;

void passengerReplyHandler()
{
  passengerReplied = 1;
}

void car()
{
  sleep(1);

  struct request rq;
  int RQstatus;
  int RQsize = sizeof(struct request) - sizeof(float);
  RQstatus = msgrcv(messageQueue, &rq, RQsize, 1, 0);

  struct acknowledgement ack = {2, "I have departed."};
  int ACKstatus;
  int ACKsize = sizeof(struct acknowledgement) - sizeof(float);
  ACKstatus = msgsnd(messageQueue, &ack, ACKsize, 0);

  sleep(1);

  passengerReplied = 0;
  kill(rq.phoneNumber, SIGUSR1);

  printf("notified passenger\n");

  signal(SIGUSR2, passengerReplyHandler);
  // while (passengerReplied == 0) { }
  
  int distance = rand() % 20 + 1;
  int price = distance * 300 + 700;
  printf("The price: %i\n", price);

  exit(0);
}

void parent()
{
  while (1)
  {
    char destinationAddress[256];
    pid_t phoneNumber;

    printf("Waiting for passenger...\n");

    int arrivalPipe = open(arrivalFIFO, O_RDONLY);
    read(arrivalPipe, &phoneNumber, sizeof(phoneNumber));
    read(arrivalPipe, destinationAddress, sizeof(destinationAddress));

    printf(
        "Passenger arrived with phone number: %d\n\t    and destination address: %s\n",
        phoneNumber, destinationAddress);

    pid_t carPID = fork();
    if (carPID < 0)
    {
      perror("Cannot create car process.");
      exit(1);
    }

    if (carPID == 0)
    {
      car();
    }

    write(arrivalPipe, &carPID, sizeof(carPID));

    struct request rq = {1, phoneNumber};
    strcpy(rq.address, destinationAddress);
    int RQstatus;
    int RQsize = sizeof(struct request) - sizeof(float);
    RQstatus = msgsnd(messageQueue, &rq, RQsize, 0);

    printf("Sent request.\n");

    struct acknowledgement ack;
    int ACKstatus;
    int ACKsize = sizeof(struct acknowledgement) - sizeof(float);
    ACKstatus = msgrcv(messageQueue, &ack, ACKsize, 2, 0);

    printf("Received acknowledgement: %s\n", ack.message);

    int status;
    waitpid(carPID, &status, 0);

    printf("Client arrived.\n\n", ack.message);

    sleep(1);
  }
}

// Passenger and PassengerGenerator

int carArrived = 0;

void carArrivedHandler()
{
  carArrived = 1;
}

void passenger(char destinationAddress[])
{
  pid_t phoneNumber = getpid();

  sleep(2);

  int arrivalPipe = open(arrivalFIFO, O_WRONLY);
  write(arrivalPipe, &phoneNumber, sizeof(phoneNumber));
  write(arrivalPipe, destinationAddress, sizeof(destinationAddress));

  pid_t carPID;
  read(arrivalPipe, &carPID, sizeof(carPID));

  carArrived = 0;
  signal(SIGUSR1, carArrivedHandler);
  while (carArrived == 0) { }
  kill(carPID, SIGUSR2);

  exit(0);
}

void passengerGenerator(int passengerCount, char *passengerDestinations[])
{
  int i;
  for (i = 1; i < passengerCount; i++)
  {
    pid_t passengerPID = fork();
    if (passengerPID < 0)
    {
      perror("Cannot create passenger process.");
      exit(1);
    }

    if (passengerPID == 0)
    {
      passenger(passengerDestinations[i]);
    }

    int status;
    waitpid(passengerPID, &status, 0);

    sleep(3);
  }

  exit(0);
}

// Main

int main(int argc, char *argv[])
{
  srand(time(NULL));

  mkfifo(arrivalFIFO, 0777);

  key_t key = ftok(argv[0], 1);
  messageQueue = msgget(key, 0777 | IPC_CREAT);
  if (messageQueue < 0)
  {
    perror("Cannot create message queue.");
    exit(1);
  }

  pid_t passengerGeneratorPID = fork();
  if (passengerGeneratorPID < 0)
  {
    perror("Cannot create passengerGenerator process.");
    exit(1);
  }

  if (passengerGeneratorPID == 0)
  {
    passengerGenerator(argc, argv);
  }
  else
  {
    parent();
  }

  int status;
  waitpid(passengerGeneratorPID, &status, 0);

  unlink(arrivalFIFO);
  return 0;
}
