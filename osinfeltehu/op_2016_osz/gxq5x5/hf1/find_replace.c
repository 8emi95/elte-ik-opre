// copies the content of the file given in argv[1] to the one given in
// argv[2] replacing the pattern given in argv[3] to argv[4]

#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	if (argc != 5)
	{
		printf("Please use this program with four arguments.\n");
		return 1;
	}
	
	int f = open(argv[1], O_RDONLY);
	if (f < 0)
	{
		printf("Error opening the input file.";
		return 1;
	}
	int g = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (g < 0)
	{
		printf("Error opening the output file.");
		return 1;
	}
	
	char pattern[] = argv[3];
	const int pattern_length = strlen(pattern);
	char buffer[pattern_length];
	if (read(f, &buffer, sizeof(buffer)) != pattern_length)
	{
		printf("Replacing cannot be started. (Is the pattern longer than the input?)");
		return 1;
	}
	int eof;
	do
	{
		int i;
		for (i = 0; i < pattern_length && buffer[i] == pattern[i]; ++i);		
		if (i < pattern_length)
		{
			write(g, &buffer[0], sizeof(buffer[0]));
			int j;
			for (j = 0; j < pattern_length - 1; ++j)
				buffer[j] = buffer[j + 1];
			if (eof = !read(f, &buffer[pattern_length - 1], sizeof(buffer[pattern_length - 1])))
				write(g, buffer, sizeof(buffer) - 1);
		}
		else
		{
			write(g, argv[4], sizeof(argv[4]));
			if (eof = read(f, buffer, sizeof(buffer)) != sizeof(buffer))
				write(g, buffer, eof * sizeof(buffer[1]));
		}
	}
	while (!eof);
	
	close(f);
	close(g);
}
