#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for pipe()
#include <string.h>
#include <time.h> //ctime

// Tengely Alex MFXX5I
// OPREG 2. beadando
// 2016.12.11.

struct Event {
  int time;
  char *spot;
  int number_guests;
  char *guests[100];
};

void initalize_events(struct Event *events) { // rendezvenyek statikus adatai
  time_t t = time(0);

  events[0].time = t + 4;
  events[0].spot = "Budapest";
  events[0].number_guests = 8;
  events[0].guests[0] = "Bret Kull";
  events[0].guests[1] = "Genaro Keeble";
  events[0].guests[2] = "Ricky Grindle";
  events[0].guests[3] = "Sandi Engelking";
  events[0].guests[4] = "Flavia Matsuo";
  events[0].guests[5] = "Almeda Mudge";
  events[0].guests[6] = "Ji Peterman";
  events[0].guests[7] = "Gaston Hanus";

  events[1].time = t + 38;
  events[1].spot = "Miskolc";
  events[1].number_guests = 7;
  events[1].guests[0] = "Ricky Grindle";
  events[1].guests[1] = "Bret Kull";
  events[1].guests[2] = "Genaro Keeble";
  events[1].guests[3] = "Flavia Matsuo";
  events[1].guests[4] = "Gaston Hanus";
  events[1].guests[5] = "Deidra Geter";
  events[1].guests[6] = "Mathilde Gagliardi";

  events[2].time = t + 21;
  events[2].spot = "Esztergom";
  events[2].number_guests = 6;
  events[2].guests[0] = "Almeda Mudge";
  events[2].guests[1] = "Gaston Hanus";
  events[2].guests[2] = "Genaro Keeble";
  events[2].guests[3] = "Sandi Engelking";
  events[2].guests[4] = "Deidra Geter";
  events[2].guests[5] = "Ricky Grindle";
}

int choose_next_event(struct Event *events,
                      const int number_events) { // kovetkezo rendezmeny
                                                 // kivalasztasa idopont alapjan
  time_t t = time(0);
  int i = 0;
  int next_event_time = 0;
  int next_event_number = -1;

  while (i < number_events) {
    if (events[i].time > t &&
        (next_event_time == 0 || next_event_time > events[i].time)) {
      next_event_time = events[i].time;
      next_event_number = i;
    }
    i = i + 1;
  }

  return next_event_number;
}

void handler(int signumber) {
  printf("Partner jelezte, hogy a helyszinen van! (%i-es szignal)\n\n",
         signumber);
}

int main(int argc, char *argv[]) {
  struct sigaction sigact;
  sigact.sa_handler = handler;
  sigemptyset(&sigact.sa_mask);
  sigact.sa_flags = 0;
  sigaction(SIGUSR1, &sigact, NULL);

  int pipe_parent[2]; // a szulo ezen keresztul fog atcsovezni adatot
  int pipe_child[2];  // a gyerek ezen keresztul fog atcsovezni adatot
  pid_t pid;

  if (pipe(pipe_parent) == -1 || pipe(pipe_child) == -1) {
    perror("Hiba a pipe nyitaskor!");
    exit(EXIT_FAILURE);
  }

  pid = fork(); // creating parent-child processes
  if (pid == -1) {
    perror("Fork hiba");
    exit(EXIT_FAILURE);
  }

  const int number_events = 3;

  if (pid == 0) {

    // child process //

    close(pipe_parent[1]);
    close(pipe_child[0]);

    printf("Partner elkezdte!\n\n");
    char received_message[250]; // char array for reading from pipe

    while (1) {
      read(pipe_parent[0], received_message, sizeof(received_message));

      if (!strcmp("STOP", received_message)) {
        printf("Partner befejezte!\n\n");
        close(pipe_parent[0]);
        close(pipe_child[1]);
        exit(EXIT_SUCCESS);
      }

      // Helyszin olvasasa //

      read(pipe_parent[0], received_message, sizeof(received_message));
      printf("Partner: Megkaptam a helyszint! (%s)\n\n", received_message);
      sleep(2);

      // Jelzes kuldese King of Standsnek helyszinre erkezeskor! //

      kill(getppid(), SIGUSR1);

      // Vendeglista olvasasa //

      read(pipe_parent[0], received_message, sizeof(received_message));

      char *str_split;
      str_split = strtok(received_message, ",");
      const int number_guests = atoi(str_split);

      int has_arrived[number_guests]; // tomb annak tarolasara, hogy a vendegek
                                      // megerkeztek-e
      char **guests =
          malloc(sizeof(char *) *
                 number_guests); // tomb a vendegek neveinek tarolasara

      printf("Partner: Megkaptam a vendeglistat!\nVendegek szama: %i fo\n",
             number_guests);

      // Vendegek megvizsgalasa, hogy ki nem erkezett meg //

      int i = 0;
      while (1) {
        str_split = strtok(NULL, ",");
        if (str_split == '\0') { // ha mar az osszes vendeget vegigneztuk
          break;
        } else {

          // Megerkezett-e a vendeg? //

          sleep(1);
          srand(time(NULL)); // the starting value of random number generation
          int p = rand() % 101; // szam 0-100 kozott
          if (p < 11) {
            has_arrived[i] = 0;
          } else {
            has_arrived[i] = 1;
          }

          // Kiirjuk a vendeg nevet sorszamozva //

          guests[i] = str_split;
          printf("%i. %s\n", i + 1, guests[i]);
          i = i + 1;
        }
      }
      printf("\n");

      // Rendezveny sikeressege: veletlenszam! //

      srand(time(NULL)); // the starting value of random number generation
      int event_success = rand() % 11; // szam 0-10 kozott

      // Jelentes kuldese King of Stands reszere //

      char report[250];
      strcpy(report, "A rendezveny sikeressege: ");
      sprintf(report + strlen(report), "%i", event_success);
      strcat(report, "/10\nA kovetkezo vendegek nem erkeztek meg!\n");

      int everybody_arrived = 1;
      i = 0;

      // Megnezzuk, ki nem erkezett meg //

      while (i < number_guests) {
        if (!has_arrived[i]) {
          strcat(report, "\t");
          strcat(report, guests[i]);
          strcat(report, "\n");
          everybody_arrived = 0;
        }
        i = i + 1;
      }

      if (everybody_arrived) {
        strcat(report, "\tMindenki megerkezett!\n");
      }

      // Jelentes elcsovezese King of Stands reszere! //

      sleep(1);
      write(pipe_child[1], report, strlen(report) + 1);
    }
  } else {

    // parent process //

    close(pipe_parent[0]);
    close(pipe_child[1]);

    printf("King of Stands elkezdte!\n\n");
    char received_message[250]; // char array for reading from pipe

    // Rendezvenyek adatainak letrehozasa statikus adatokkal //

    struct Event *events = malloc(sizeof(struct Event *) * number_events);
    initalize_events(events);

    // Rendezvenyek kilistazasa! //

    printf("A rendezvenyek listaja!\n");
    int i = 0;
    while (i < number_events) {
      printf("%i. %s (timestamp: %i)\n", i + 1, events[i].spot, events[i].time);
      i = i + 1;
    }
    printf("\n");

    // Addig ismetlunk, amig van hatralevo rendezveny //

    while (1) {

      // Kivalasztjuk a kovetkezo rendezvenyt az idopontja alapjan //

      const int next_event_number = choose_next_event(events, number_events);

      // Ha nincs tobb rendezveny, befejezzuk //

      if (next_event_number == -1) {
        printf("King of Stands: Nincs tobb rendezveny!\n\n");
        write(pipe_parent[1], "STOP",
              strlen("STOP") +
                  1); // jelzes partnernek, hogy nincs tobb rendezveny
        printf("King of Stands befejezte!\n");
        close(pipe_parent[1]);
        close(pipe_child[0]);
        exit(EXIT_SUCCESS);
      } else {
        write(pipe_parent[1], "NEXT",
              strlen("NEXT") + 1); // jelzes partnernek, hogy van rendezveny
      }

      // Ha van rendezveny, megvarjuk a kezdodeset! //

      printf("King of Stands: Kovetkezo rendezveny %i masodperc mulva "
             "kezdodik!\n\n",
             (unsigned int)(events[next_event_number].time - time(0)));
      sleep(events[next_event_number].time - time(0));

      // Helyszin elkuldese partnernek //

      write(pipe_parent[1], events[next_event_number].spot,
            strlen(events[next_event_number].spot) + 1);

      // Varakozas a partner helyszinre erkezesere //

      pause();

      // Vendeglista elkuldese partnernek //

      char guest_list[250];
      sprintf(guest_list, "%i", events[next_event_number].number_guests);
      strcat(guest_list, ",");
      int i = 0;
      while (i < events[next_event_number].number_guests) {
        strcat(guest_list, ",");
        strcat(guest_list, events[next_event_number].guests[i]);
        i = i + 1;
      }
      write(pipe_parent[1], guest_list, strlen(guest_list) + 1);

      // Jelentes olvasasa partnertol! //

      read(pipe_child[0], received_message, sizeof(received_message) + 1);
      printf("King of Stands: Megkaptam a "
             "jelentest!\n%s\n\n------------------------------\n",
             received_message);
    }
  }

  exit(EXIT_SUCCESS);
}
