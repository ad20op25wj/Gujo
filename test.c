#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	char string[10];

	string[0] = 'a';
	string[1] = 'b';
	string[2] = 'c';
	string[3] = '\0';

	puts(string);

	return 0;
}
