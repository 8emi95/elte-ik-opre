Init random generator:

srandom(10);
srandom(time(NULL));
srand(time(NULL));   //the starting value of random number generation


Use:

li = random();
int r = rand() % 100;   //number between 0-99