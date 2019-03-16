#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <wait.h>
#include <stdlib.h>

#define MAX_COUNT

int main()
{
    int m1,m2;
    int A[MAX_COUNT] = {1,3,4,6,5};
    pid_t cpid;
    
    cpid = fork();
    
    if(cpid == 0)
    {
		m1 = max(1,N/2,A);
    }
    else
    {
		m2 = max(N/2+1,N,A);
    };
    
    if(m1 < m2)
    {
		printf("The max value: %d\n",m2);
    };
    
    return 0;
};

int cprocess(int k,int l, int A[N])
{
    int m1;
    m1 = max(k,l,A);
    return m1;
};

int max(int k, int l, int A[N])
{
    m1 = cprocess(A,N/2,A);
    m2 = process(N/2+1,N,A);
    
    int i;
    for(i = 0; i < A.length; i++)
    {
	int n;
	for(n = A[i]; n < l; n++)
	{
	    if(m1 < A[i])
	    {
		m1 = A[i]
	    }
	}
    }
    
    max = m1;
    
    return max;
};