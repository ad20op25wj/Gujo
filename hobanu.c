#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAIN_MENU "\n< Run, HOBANU!!! >\n1. Game Start\n2. Score\n3. Quit\n>> "

int main(int argc, char *argv[])
{
	int input;
	pid_t pid;

	while(1)
	{
		printf(MAIN_MENU);
		scanf("%d", &input);

		switch (input) {
		case 1:
			if ((pid = fork()) < 0)
			{
				perror("fork error");
				exit(1);
			}
			else if (pid == 0) // Child
			{
				system("./game");
				return 0;
			}
			else // Parent
				wait(NULL);
			break;
		case 2:
			puts("Ranking");
			break;
		case 3:
			return 0;
		default:
			puts("Enter the number \'1 - 3\'");
			break;
		}
	}
}
