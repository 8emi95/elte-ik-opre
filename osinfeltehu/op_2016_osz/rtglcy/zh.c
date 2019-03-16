#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


int main(int argc, char *argv[])
{ 
	char feladat[1000] = "ajvfhLIBEF WSFLS FLWef ehafzsj fariuelk gsig ltrgsl evbairug aeérgva elrugaqepérvg aergaeiubgrae va";
	char sorszam[1000] = "8";
	char result[1000];
	
	int MyPipeFiel[2];
	char buffer[300];

	int MyPipe = pipe(MyPipeFiel);
	if (MyPipe < 0)
	{
		perror("pipe(MyPipeFiel) error!");
		exit(-1);
	}


	pid_t child = fork();
	if (child < 0) 
	{
		perror ("fork() error!");
		exit(-1);
	}
	
	/* parent process */
	if (0 != child)	
	{	
		
		strcpy(result, "");
		strcat(result, feladat);
		strcat(result, "_");
		
		strcat(result, argv[1]);
		strcat(result, "_");
		
		strcat(result, sorszam);

		/* printf("%s", result); */		

		write(MyPipeFiel[1], result, strlen(result) + 1);

		wait(NULL);

		read(MyPipeFiel[0], buffer, 300);
		printf("\n%s\n", buffer);
	}
	/* child process */
	else
	{
		read(MyPipeFiel[0], buffer, 999);
		/* printf("\n%s\n\n", buffer); */
		char** tokens;
		tokens = str_split(buffer, '_');
		
		printf("Feladat: %s\n", *(tokens + 0));
		printf("Nev: %s\n", *(tokens + 1));
		printf("Sorszam: %s\n", *(tokens + 2));
		
		char r[100];
		if (rand() % 2 == 1) {
			strcpy(r, "Gratulalok sikerult a feladat!");
		} else {
			strcpy(r, "Sajnos nem sikerult a feladat");
		}

		write(MyPipeFiel[1], r, strlen(r) + 1);
	}


	return 0;
}


