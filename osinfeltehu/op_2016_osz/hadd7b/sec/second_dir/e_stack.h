#include <sys/mman.h>
#include <string.h>

#define ES 256

void* _malloc(size_t size)
{
	return 
	mmap(NULL, size,
		PROT_READ|PROT_WRITE|PROT_EXEC,
		MAP_PRIVATE|MAP_ANONYMOUS,
		-1, 0);
}

typedef struct _str_stack
{
	int m;
	int t;
	char** strs;
} str_stack;

str_stack* init(int top)
{
	str_stack* es;

	es = _malloc(sizeof(str_stack));
	es->strs = _malloc(sizeof(es->strs[0])*(top-1));
	for(int i=0; i<top; ++i)
	{
		es->strs[i] = _malloc(sizeof(es->strs[0])*ES);
	}
	es->m = 0;
	es->t = top;

	return es;
}
		
int push(str_stack* st, char* elem)
{
	if(st->m < st->t)
	{
		strcpy(st->strs[st->m], elem);
	
		++st->m;

		return strlen(elem);
	}
	else
	{
		return 0;
	}
}

char* pop(str_stack* st)
{
	if(st->m > 0)
	{
		char* eptr;
		eptr = _malloc(sizeof(char)*ES);

		char* crnt = st->strs[st->m-1];

		strcpy(eptr, crnt);
		memset(crnt, 0, strlen(crnt));

		--st->m;

		return eptr;
	}
	else
	{
		return 0;
	}
}

int top(str_stack* st)
{
	return st->m;
}
