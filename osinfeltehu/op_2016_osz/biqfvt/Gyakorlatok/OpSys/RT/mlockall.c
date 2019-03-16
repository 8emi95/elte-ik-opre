#include <stdio.h>

#include <stdlib.h>

#include <sched.h>

#include <sys/mman.h>

#include <sys/resource.h>

 

#define SIZE      500

 

int main(void)

{

 

   struct record {

      char name[128];

      int count;

      unsigned char dummy_array[1024000];

      struct record *next;

   };

 

   struct record *a[SIZE];

   struct sched_param param;

   int i;

   struct rusage usage1, usage2, usage3, usage4;

 

 

   getrusage(RUSAGE_SELF, &usage1);

   /*

    * Part 1:  Lock down memory mappsed to the process address space

    */

   if(mlockall(MCL_CURRENT) < 0) {

      perror("mlockall");

      return 1;

   }

 

   /*

    * Collect stats that show the effect of mlockall on paging

    */

   getrusage(RUSAGE_SELF, &usage2);

 

   /*

    *  Part 2:  Dynamically allocate memory for records

    */

   for(i = 0; i < SIZE; i++) {

      if ((a[i] = (struct record *)malloc(sizeof(struct record))) == NULL) {

         perror("malloc failed");

         return 1;

      }

   }

 

   /*

    *  Collect stats that show the effect of dynamic memory allocation

    */

   getrusage(RUSAGE_SELF, &usage3);

 

   /*

    *  Part 3:  Initialize allocated records

    */

   for(i = 0; i < SIZE; i++) {

      (a[i])->count = 0;

      (a[i])->dummy_array[0] = 0;

      (a[i])->next = NULL;

   }

 

   /*

    *  Collect stats that show the effect of initialing newly allocated memory.

    */

   getrusage(RUSAGE_SELF, &usage4);

 

   /* Print out some statistics */

   printf("Before mlockall\n");

   printf("minflt %d\tmajflt %d\n\n", usage1.ru_minflt, usage1.ru_majflt);

   printf("After mlockall\n");

   printf("minflt %d\tmajflt %d\n\n", usage2.ru_minflt, usage2.ru_majflt);

   printf("After dynamic memory allocation with malloc\n");

   printf("minflt %d\tmajflt %d\n\n", usage3.ru_minflt, usage3.ru_majflt);

   printf("After initializing all allocated memory\n");

   printf("minflt %d\tmajflt %d\n\n", usage4.ru_minflt, usage4.ru_majflt);

 

   /*

    *  Cleanup. Free all memory allocations

    */

   for(i = 0; i < SIZE; i++) {

      free(a[i]);

   }  

 

   return 0;

}