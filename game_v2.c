#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

#define GROUND '='
#define HOLE ' '
#define A 10 // Ground Altitude
#define P 10

#define PROBABLITY 9
#define HOLE_WIDTH 6
#define REST_WIDTH 18
#define JUMP_WIDTH 9

#define USEC 50000 // 0.05 sec

// j means jump
#define PRINT_HOBANU(j) for (i = 1; i <= 7; i++) \
	{ \
		move(LINES-(A+i), P); \
		addch(' '); \
	} \
	move(LINES-(A+3+(j)), P); \
	addch('K');\
	move(LINES-(A+2+(j)), P); \
	addch('N');\
	move(LINES-(A+1+(j)), P); \
	addch('U')

int i;
int left_hole = 0;
int left_times_to_fly_or_rest = 0;
int jump_height = 0;
int jump_width = 0;
char *land; // to render the ground
struct winsize w; // to store terminal size

void Hobanu_renderer(void); // to update Hobanu
void map_constructor(void); // to update the ground
/* https://stackoverflow.com/questions/58466603/snake-game-in-linux-source-code-c-language */
char mygetch(void);
int kbhit(void);
void checkJumpPressed(void);
/*******************************************************************************************/

// View
int main(int argc, char *argv[])
{
	int score = 0;
	int cnt = 0;
	char buffer[20];
	// Get terminal size(lenght, width)
	ioctl(0, TIOCGWINSZ, &w);
	land = malloc(sizeof(*land) * (w.ws_col+1));

	// Initialize ground
	for (i = 0; i <= w.ws_col; i++)
		land[i] = GROUND;
	land[w.ws_col] = '\0';

	srand(time(NULL)); // put in rand seed

	initscr(); // <curses.h>
	clear(); // <curses.h>

	while (1)
	{
		
		if(cnt++%50==0)// scoreup speed lower
			score++;
		// put Hobanu on the ground
		PRINT_HOBANU(jump_height);

		move(1, 1);
		addstr("Score :");
		sprintf(buffer,"%d", score);
		addstr(buffer);

		// print the ground
		move(LINES-A, 0);
		addstr(land);

		move(LINES-1, COLS-1);

		refresh(); // <curses.h>

		if (jump_height == 0 && land[P] == ' ')
			break;
        if (jump_height != 0 && land[P] == ' '){ // 홀에서 점프시 점수 추가획득
            score += 10;
        }

		Hobanu_renderer(); // update Hobanu
		map_constructor(); // update the ground

		usleep(USEC); // sleep for USEC
	}
    FILE* fp = NULL;
    fp = fopen("SCORE.txt","wt");
    fprintf(fp,"%d",score);
    if(fp!=NULL){
        fclose(fp);
    }
	sleep(5);

	endwin(); // <curses.h>

	free(land);
	return score;
}

void Hobanu_renderer(void) // update Hobanu
{
	checkJumpPressed();

	switch (jump_width)
	{
	case 9:
	case 1:
		jump_height = 1;
		break;
	case 8:
	case 2:
		jump_height = 2;
		break;
	case 7:
	case 3:
		jump_height = 3;
		break;
	case 6:
	case 5:
	case 4:
		jump_height = 4;
		break;
	default:
		jump_height = 0;
		break;
	}

	jump_width--;
}

void map_constructor(void) // update the ground
{
	// backpropagate the ground
	for (i = 0; i < w.ws_col-1; i++)
		land[i] = land[i+1];

	if (!(rand() % PROBABLITY) && left_times_to_fly_or_rest <= 0)
	// make probablity 1/DIFFICULTY
	{
		left_hole = HOLE_WIDTH;
		left_times_to_fly_or_rest = HOLE_WIDTH + REST_WIDTH;
	}

	// make a new land as a hole or ground;
	if (left_hole-- > 0)
		land[i] = HOLE;
	else
		land[i] = GROUND;

	left_times_to_fly_or_rest--;
}

// https://stackoverflow.com/questions/58466603/snake-game-in-linux-source-code-c-language
char mygetch(void)
{
	char c;

	system("stty raw");
	c = getchar();
	system("stty sane");
	
	return(c);
}

// https://stackoverflow.com/questions/58466603/snake-game-in-linux-source-code-c-language
int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if (ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

// https://stackoverflow.com/questions/58466603/snake-game-in-linux-source-code-c-language
void checkJumpPressed(void)
{
	if (kbhit()) // if a key has been pressed
		if (mygetch() == 'w')
			jump_width = JUMP_WIDTH;
}
