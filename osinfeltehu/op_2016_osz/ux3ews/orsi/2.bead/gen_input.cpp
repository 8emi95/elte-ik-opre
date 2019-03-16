#include <cstdio>
#include <cstdlib>
#include <random>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Size of input should be given in first parameter");
		exit(0);
	}

	long N = atoi(argv[1]);

	if (N <= 0)
	{
		printf("Wrong parameter given");
		exit(0);
	}

	long order = (argc > 2) ? atoi(argv[2]) : 0;

	FILE* f = fopen("input.txt", "w");

	if (f == NULL)
	{
		perror("File opening error\n");
		exit(1);
	}

	fprintf(f, "%ld\n", N);

	switch (order)
	{
	case 1:
		for (long int i = 1; i <= N; i++)
		{
			fprintf(f, "%06ld\n", i);
		}
		break;

	case 2:
		while (N)
		{
			fprintf(f, "%06ld\n", N--);
		}

	default:
	{
		std::default_random_engine generator;
		std::uniform_int_distribution<long> distribution(1, 999999);

		while (N--)
		{
			fprintf(f, "%06d\n", distribution(generator));
		}
		break;
	}
	}

	fclose(f);
}