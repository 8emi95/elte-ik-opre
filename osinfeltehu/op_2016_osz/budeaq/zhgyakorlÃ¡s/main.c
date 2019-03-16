#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdbool.h>

void handler(int signal);

int rand_int(int max){
  srand(time(NULL));
  int random_id = rand() * max;
  return random_id;
}

bool is_exist( const int* id_array, int array_size, int checkthis_id){
  int index;
  for(index; index < array_size; index++){
   if(id_array[index] == checkthis_id){
     return true;    
   }
  }
  return false;
}

int gen_id(const int* id_array, int array_size){
  int random_id = rand_int(array_size * 10);
  while(is_exist(id_array, array_size, random_id )){
   random_id = rand_int(array_size * 10);

  }
  return random_id;
}

int main(int argc, char const *argv[]){
  system("clear");
  if(argc < 2){
   printf("Nem kaptam szavazókat!\n");
   exit(1);
  }
  
  int arg_num = (int) (argv[1][0] - '0');
  printf("A szavazók száma: %d\n", arg_num);

  int pipe_1[2];
  int pipe_2[2];

  pid_t child_1 = fork();

  if(child_1 < 0){
    printf("Nem születtem meg!\n");
    exit(1);
  }
  if(child_1 > 0){
    int mypid = getpid();
    int parentpid = getppid();

    printf("PARENT  mypid: %d, parentpid: %d \n", mypid, parentpid);

    signal(SIGUSR1, handler);
    pause();
    signal(SIGUSR1, SIG_DFL);
    printf("PARENT signal érkezett. (child1)\n");

   
    int id_array[arg_num];
    int index = 0;
    for (index; index<arg_num; index++){
      id_array[index] = gen_id(id_array, sizeof(id_array));
    }

    index = 0;
    for (index; index<arg_num; index++){
        printf(" %d\n",id_array[index]);
    } 
 
    pid_t child_2 = fork();
    if(child_2 == 0){
       int mypid = getpid();
       int parentpid = getppid();
       printf("CHILD2  mypid: %d, parentpid: %d \n", mypid, parentpid);

       kill(parentpid, SIGUSR2);
     }

     signal(SIGUSR2, handler);
     pause();
     signal(SIGUSR2, SIG_DFL);
     printf("PARENT signal érkezett. (child2)\n");
          

/*  int status;
    waitpid(child_1, &status, 0);
    waitpid(child_2, &status, 0);*/
  }
  else{
    int mypid = getpid();
    int parentpid = getppid();
    printf("CHILD1  mypid: %d, parentpid: %d \n", mypid, parentpid);
    
    kill(parentpid, SIGUSR1);
  }
  return 0;
}

void handler(int signal){
   if(signal == SIGUSR1){
     printf("Készen áll a munkára a chil1.\n");
   }
   if(signal == SIGUSR2){
     printf("Készen áll a minkára a child2.\n");
   }
}

