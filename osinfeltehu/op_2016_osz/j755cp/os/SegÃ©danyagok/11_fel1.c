#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

// Ez mondja meg, hogy megjött-e a szinkronizációs szignál.
// Fork után kettő is lesz belőle, csakúgy mint a szignálkezelőkből, tehát nem fog összekeveredni.
volatile sig_atomic_t _sync = 0;

struct point
{
	double x, y;
};

// Ez kezeli a szinkronizációs szignálokat (SIGUSR1-re fog lefutni).
void synchHandler(int sig)
{
	_sync = 1;
	signal(SIGUSR1, synchHandler);
}

// Ez állítja le a gyerek taskot.
// A SIGUSR2-re fog lefutni, a szülőnek ez nincs beállítva, de ő nem is fog ilyet kapni (viszont küld majd egyet a végén a gyereknek).
void endHandler(int sig)
{
	_exit(0);
}

// Ez blokkolja azt a taskot, aminek be kell várnia a másikat (megvárja, amíg megjön a szignál).
inline void synchronize(void)
{
	sigset_t mask, oldmask;
	sigemptyset (&mask);
    sigaddset (&mask, SIGUSR1);
    sigprocmask (SIG_BLOCK, &mask, &oldmask);
	if(!_sync) /**/ sigsuspend (&oldmask) /**/; // Itt van a várakozás, amíg meg nem jön a SIGUSR1, de csak ha nem jött már meg előbb, mint várnánk.
	sigprocmask (SIG_UNBLOCK, &mask, NULL);
	_sync = 0; // Vissza kell állítani, mert később is ezzel fogunk szinkronizálni.
}

int main(void)
{
	//Létrehozom a nevesített csövet
	mkfifo("myfifo", S_IRUSR|S_IWUSR);
	signal(SIGUSR1, synchHandler);
	pid_t child = fork(); //létrehozom a gyerekfolyamatot
	if(child == 0) //A gyerek
	{
		signal(SIGUSR2, endHandler);
		struct point pont1;
		struct point pont2;
		struct point pont3;

		while(1) //végtelen ciklus, az endHandler tudja lelőni
		{
			int fd = open("myfifo", O_RDONLY); //megnyitom olvasásra a csövet
				synchronize();
				read(fd, &pont1, sizeof(pont1));
				read(fd, &pont2, sizeof(pont2));
				read(fd, &pont3, sizeof(pont3));
			close(fd);
			kill(getppid(), SIGUSR1);

			int answer;
		
			if ((((pont2.y-pont1.y)/(pont2.x-pont1.x))*(pont3.x-1)-pont1.y) > pont3.y) answer = 3; //felett
			if ((((pont2.y-pont1.y)/(pont2.x-pont1.x))*(pont3.x-1)-pont1.y) == pont3.y) answer = 2; //rajta
			if ((((pont2.y-pont1.y)/(pont2.x-pont1.x))*(pont3.x-1)-pont1.y) < pont3.y) answer = 1; //alatt
		
			fd = open("myfifo", O_WRONLY);
			write(fd, &answer, sizeof(int));
			kill(getppid(), SIGUSR1);
			synchronize();
			close(fd);
		}
	}
	else //A szülő
	{
		int fd, answer = 1;
		struct point pont1;
		struct point pont2;
		struct point pont3;

		while(answer)
		{
			printf("Kerem a vonal 1. pontjanak koordinatait:\n");
			printf("x: ");
			scanf("%lf", &pont1.x);
			printf("y: ");
			scanf("%lf", &pont1.y);
			printf("Kerem a vonal 2. pontjanak koordinatait:\n");
			printf("x: ");
			scanf("%lf", &pont2.x);
			printf("y: ");
			scanf("%lf", &pont2.y);
			printf("Kerem a pont kordinatait:\n");
			printf("x: ");
			scanf("%lf", &pont3.x);
			printf("y: ");
			scanf("%lf", &pont3.y);

			fd = open("myfifo", O_WRONLY);
				write(fd, &pont1, sizeof(pont1));
				write(fd, &pont2, sizeof(pont2));
				write(fd, &pont3, sizeof(pont3));
				kill(child, SIGUSR1);
				synchronize();
			close(fd);

			fd = open("myfifo", O_RDONLY);
				synchronize();
				read(fd, &answer, sizeof(int));
				close(fd);
			kill(child, SIGUSR1);

			if(answer == 3)
			{
				printf("A pont az egyenes felett van.");
			}
			if(answer == 2)
			{
				printf("A pont az egyenesen van.");
			}
			if(answer == 1)
			{
				printf("A pont az egyenes alatt van.");
			}
			
			printf("Menjen tovább? (0 ha nem)");
			scanf("%d", &answer);
			pont3.y *= -1;
		}
		kill(child, SIGUSR2);
	}
	unlink("myfifo");
	return 0;
}

