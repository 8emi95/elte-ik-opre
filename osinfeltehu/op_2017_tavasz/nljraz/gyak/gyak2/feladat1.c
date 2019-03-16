#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define New(type) ((type*)malloc(sizeof(type)))
#define NewArray(type, count) ((type*)malloc(count * sizeof(type)))

struct Data
{
	char *str;
	int val;
};

struct Data* Read(int input)
{
	struct Data *data;
	
	int length;
	if (!read(input, &length, sizeof(int)))
		return 0;
	
	int i = 0;
	char *str = NewArray(char, length + 1);
	for (; i < length; ++i)
	{
		char c;
		if (!read(input, &c, sizeof(c)))
			return 0;
		str[i] = c;
	}
	str[i] = '\0';
	
	int val;
	if (!read(input, &val, sizeof(int)))
		return 0;
	
	data = New(struct Data);
	data->str = str;
	data->val = val;
	
	return data;
}

int Write(int output, struct Data *data)
{
	int length = strlen(data->str);
	if (!write(output, &length, sizeof(int)))
		return 0;
	
	int i = 0;
	for (; i < length; ++i)
	{
		if (!write(output, &(data->str[i]), sizeof(char)))
			return 0;
	}
	
	if (!write(output, &(data->val), sizeof(int)))
		return 0;
	
	return 1;
}

int main(int argc, char **argv) 
{
	struct Data *data = New(struct Data);
	data->str = NewArray(char, 80);
	data->str = "Hello World!";
	data->val = 42;
	
	int f = open("data.dat", O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
	Write(f, data);
	close(f);
	free(data);
	
	f = open("data.dat", O_RDONLY);
	data = Read(f);
	close(f);
	printf("%s %d\n", data->str, data->val);
	free(data);
	
	return 0;
}

/*
	struct: szöveg + szám
	feltölteni
	kiírni fájlba
	beolvasni fájlból
*/