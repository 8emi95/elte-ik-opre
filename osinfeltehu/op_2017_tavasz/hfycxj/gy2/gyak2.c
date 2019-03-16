//szöveg hossz
int hossz(char* s)
{
	int i = 0;
	while(s[i++]);	// (*s++ != 0)
	return i;
}

//karaktercsere
void* karakter_csere(char* miben, char* mit, char* mire)
{
	char* start = (char*)miben;
	
	while(*start)
	{
		if(*strat == *mit)
		{
			*start = *mire;
		}
		*start++;
	}
}

//karakter számlálás
int karakter_szamlalas(char* miben, char* mit)
{
	char* start = (char*)miben;
	int sum = 0;
	while(*start)
	{
		if(*start == *mit)
		{
			sum++;
		}
		*start++;
	}
	return sum;
}

//szövegkeresés

////////////////////////////////////////////////////////

struct coords
{
	char x[5];
	char y[5];
};

int main()
{
	int f = open("coords.txt");
	O_RDWR(O_CREAT, S_IRUSR);
	
	struct coords c;
	printf("coords merete: %i\n", sizeof(c));
	
	
	strcpy(c.x, "12345");
	strcpy(c.y, "67890");
	
	write(f, &c, sizeof(c));
	close(f);
	
	int fRead = open("coords.txt", O_RDONLY,S_IRUSR);
	
	char cRead;
	
	while(read(fRead, &cRead, 1)))
	{
		printf("%c", cRead);
	}
	
	close(fRead);
	return 0;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}