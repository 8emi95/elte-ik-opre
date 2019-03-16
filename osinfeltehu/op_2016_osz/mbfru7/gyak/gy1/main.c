#include <stdio.h>
#include <stdlib.h>

int length (char* str) {
	int result = 0;
	while (*str != 0) {
		result++;
		*str++;
	}
	return result;
}

int compare (char* a, char*b) {
	int result = 1;
	while (*a != 0 && *b != 0 && result) {
		if (*a != *b) {
			result = 0;
		}
		*a++;
		*b++;
	}
	return result;
}

int main (int argc, char ** argv) {
	// feladat 1
	//
	// char* s = "hello";
	// printf("%s length: %i\n", s, length(s));
	
	// feladat 2
	//
	// char* a = "kutya";
	// char* b = "cica";
	// char* c = "kutya";
	//
	// if (compare(a,b)) {
	//     printf("%s & %s are identical.\n", a, b);
	// } else {
	//     printf("%s & %s aren't identical.\n", a, b);
	// }

	// feladat 3
	// ???
	
	// feladat 4
	//
	// printf("Number of command line arguments are: %i\n", argc - 1);
	// if (argc - 1) {
	//     int i;
	//     for (i = 1; i < argc; i++) {
	//         printf("%i. argument is %s\n", i, argv[i]);
	//     }
	// }
	
	
	
	return 0;
}