// copies the content of the file given in argv[1] to the one given in
// argv[2] replacing the pattern given in argv[3] to argv[4]

#include <fcntl.h>
#include <string.h>
#include <stdio.h>

void knuth_morris_pratt_replace(int input, int output, char pattern[], char replace_with[]);
void kmp_init(int next[], char pattern[]);

int main(int argc, char** argv)
{
	if (argc != 5)
	{
		printf("Please use this program with four arguments.\n");
		return 1;
	}
	
	// file descriptors
	int f = open(argv[1], O_RDONLY);
	int g = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	knuth_morris_pratt_replace(f, g, argv[3], argv[4]);
	close(f);
	close(g);
	return 0;
}

void knuth_morris_pratt_replace(int input, int output, char pattern[], char replace_with[])
{
	int pattern_length = strlen(pattern);
	int next[pattern_length];
	kmp_init(next, pattern);
	int i = 0, j = 0;
	char c;
	int eof = read(input, &c, sizeof(c));
	while (!eof)
	{
		if (c == pattern[j])
		{
			write(output, &c, sizeof(c));
			eof = read(input, &c, sizeof(c));
			++j;
			if (j == pattern_length)
			{
				write(output, &replace_with, sizeof(replace_with));
				j = next[j];
			}
		}
		else
		{
			if (j = 0)
			{
				write(output, &c, sizeof(c));
				eof = read(input, &c, sizeof(c));
			}
			else
			{
				j = next[j];
			}
		}
	}
}

// For each "j" index of the pattern the array next should give how long prefix of
// the pattern is a suffix of the sequence of the first "j" characters of the 
// pattern.
void kmp_init(int next[], char pattern[])
{
	next[0] = 0;
	int i = -1;
	int j = 0;
	int pattern_length = strlen(pattern);
	while (j < pattern_length)
	{
		if (pattern[i + 1] == pattern[j + 1])
		{
			++i;
			++j;
			next[j] = i;
		}
		else
		{
			if (i == 0)
			{
				++j;
				next[j] = 0;
			}
			else
			{
				i = next[i];
			}
		}
	}
}
