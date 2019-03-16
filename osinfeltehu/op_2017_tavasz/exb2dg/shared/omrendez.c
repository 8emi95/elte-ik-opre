#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define ELEMSZAM 50

void mem_feltolt( int osztott, int db )
{
   printf("\nEz a program tetszoleges szamu egesz szamot general, amelyeket\n");
   printf("az osztott memoriaba ir, majd rendezi oket es visszairja oda azokat.\n");
   printf("\nrendezendo elemek, amelyeket a veletlenszam generator csinal:\n");
   int i, *buffer;
   buffer = shmat ( osztott, NULL, 0 );
   srand(time(NULL));
   for(i=0; i<db; ++i)
	buffer[i] = rand()%100;
   shmdt( buffer );
}


void merge(int sh_mem_id,int tol1,int ig1,int tol2,int ig2)
{
    int *eredm,*v1,*v2,d1,d2;
    eredm = shmat(sh_mem_id,NULL,0 );

    d1 = ig1-tol1+1;
    d2 = ig2-tol2+1;
    v1 = malloc(d1*sizeof(int));
    v2 = malloc(d2*sizeof(int));

    memcpy(v1,&eredm[tol1],d1*sizeof(int));
    memcpy(v2,&eredm[tol2],d2*sizeof(int));

    int i=0;
    int j=0;
    int k=tol1;

    while( i<d1 && j<d2 )
    {
	if( v1[i]<v2[j] )
	{
	    eredm[k]=v1[i];
	    ++i; ++k;
	}

	else if( v1[i]>v2[j])
	{
	    eredm[k]=v2[j];
	    ++j; ++k;
	}

	else
	{
	    eredm[k]=v1[i];
	    eredm[k+1]=v2[j];
	    ++i; ++j; k=k+2;
	}
    }

    while( i<d1 )
    {
	eredm[k]=v1[i];
	++i;
	++k;
    }

    while( j<d2 )
    {
	eredm[k]=v2[j];
	++j;
	++k;
    }

    shmdt(eredm);
    free(v1);
    free(v2);
}


void kiir(int sh_mem_id)
{
    int *v;
    v = shmat(sh_mem_id,NULL,0 );
    int i;
    for(i=0;i<ELEMSZAM;++i)
	printf("%d\t",v[i]);
    shmdt(v);
}

int intcmp(const void *a, const void *b)
{
     return *((int*)a)>*((int*)b);
}

int main (int argc, char **argv)
{
   pid_t ch1,ch2;
   int sh_mem_id;
   int index_tol,index_ig;
   {
       sh_mem_id = shmget( ftok(argv[0],1), ELEMSZAM*sizeof(int),  0700 | IPC_CREAT );
       index_tol = 0;
       index_ig  = ELEMSZAM-1;
       mem_feltolt( sh_mem_id, ELEMSZAM );
       kiir(sh_mem_id);
   }

   if (argc > 2)
   {
       sh_mem_id = atoi(argv[0]);
       index_tol = atoi(argv[1]);
       index_ig  = atoi(argv[2]);
       if( (index_ig-index_tol)>10 )
       {
  	     ch1 = ch2 = -1;
  	     ch1 = fork ();
      	 if( ch1>0 ) ch2 = fork();
   	     if( ch1>0 && ch2>0 )
	     {
	       waitpid(ch1,NULL,-0);
	       waitpid(ch2,NULL,-0);
	       merge(sh_mem_id,index_tol,index_tol+(index_ig-index_tol)/2,1+index_tol+(index_ig-index_tol)/2,index_ig);
   	     }
	     else
         {  if( ch1 == 0 )
		    {
	         char *a[4];
	         a[0] = malloc(sizeof(int));
	         a[1] = malloc(sizeof(int));
	         a[2] = malloc(sizeof(int));
	         sprintf(a[0],"%d",sh_mem_id);
	         sprintf(a[1],"%d",index_tol);
	         sprintf(a[2],"%d",index_tol+(index_ig-index_tol)/2);
                 a[3] = NULL;
	         execv(argv[0],a);
   		    }
		    else
		    { if( ch2 == 0 )
	          {
	            char *a[4];
	            a[0] = malloc(sizeof(int));
	            a[1] = malloc(sizeof(int));
	            a[2] = malloc(sizeof(int));
	            sprintf(a[0],"%d",sh_mem_id);
	            sprintf(a[1],"%d",1+index_tol+(index_ig-index_tol)/2);
	            sprintf(a[2],"%d",index_ig);
                    a[3] = NULL;
	            execv(argv[0],a);
	            //perror("execv");
	   	      }
   		    };
         }
         }
         else
         {
            qsort( shmat(sh_mem_id,NULL,0 )+index_tol*sizeof(int),index_ig-index_tol+1,sizeof(int),intcmp);
         };
   };
   if(argc<3)
   {
        printf("\nAz elemek rendezve:\n");
        kiir(sh_mem_id);
	shmctl ( sh_mem_id, IPC_RMID, NULL );

   }
   return 0;
}
