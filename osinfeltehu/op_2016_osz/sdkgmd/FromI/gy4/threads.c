#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 

     int _my_value = 0; 
     pthread_mutex_t mutex_my_value = PTHREAD_MUTEX_INITIALIZER; 
     pthread_cond_t cond_my_value = PTHREAD_COND_INITIALIZER; 
     int thid[3] = { 0, 1, 2 }; 
     
     void inc_my_value (int *id) { 
          int i;  printf ("\nSTART inc_my_value %d\n", *id); 
          for (i = 0; i < 7; i++) { 
               sleep(random() % 3); 
               pthread_mutex_lock (&mutex_my_value); 
               _my_value ++; 
               printf("\n inc_my_value: thread %d old _my_value %d new _my_value %d", *id, _my_value - 1, _my_value); 
               if (_my_value == 12) 
                    pthread_cond_signal (&cond_my_value); 
               pthread_mutex_unlock (&mutex_my_value); 
          } 
          printf ("\nSTOP inc_my_value %d\n", *id); 
     } 
     
     void verify_my_value (int *id) { 
          printf ("\nSTART verify_my_value \n"); 
          pthread_mutex_lock (&mutex_my_value); 
          while (_my_value <= 12) { 
               pthread_cond_wait (&cond_my_value, &mutex_my_value); 
               printf ("\n verify_my_value: thread %d _my_value %d", *id, 
                    _my_value); 
               break; 
          } 
          pthread_mutex_unlock (&mutex_my_value); 
          printf ("\nSTOP verify_my_value \n"); 
     } 
     
     main () { 
          pthread_t th[3]; 
          int i; 
          // A három szál létrehozása. 
          pthread_create ((pthread_t *) & th[0], NULL, (void *) 
               verify_my_value, &thid[0]); 
          pthread_create ((pthread_t *) & th[1], NULL, (void *) 
               inc_my_value, &thid[1]); 
          pthread_create ((pthread_t *) & th[2], NULL, (void *) 
               inc_my_value, &thid[2]); 
          // Várakozás a szálak befejezésére: 
          for (i = 0; i < 3; i++) 
               pthread_join (th[i], NULL); 
     }