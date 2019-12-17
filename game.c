#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <sys/ioctl.h>

#define GROUND '='
#define HOLE ' '

#define DIFFICULTY 10
#define HOLE_WIDTH 10
#define REST_WIDTH 30
#define USEC 50000 // 0.05 sec

int i;
int left_hole = 0;
int left_times_to_jump_or_rest = 0;
char *land; // to render the ground
struct winsize w; // to store terminal size

void map_constructor(void); // to update the ground

// View
int main(int argc, char *argv[])
{
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
		// print the ground
		move(LINES-10, 0);
		addstr(land);

		move(LINES-1, COLS-1);

		refresh(); // <curses.h>

		map_constructor(); // update the ground

		usleep(USEC); // sleep for USEC
	}

	endwin(); // <curses.h>

	free(land);

	return 0;
}

void map_constructor(void) // update the ground
{
	// backpropagate the ground
	for (i = 0; i < w.ws_col-1; i++)
		land[i] = land[i+1];

	if (!(rand() % DIFFICULTY) && left_times_to_jump_or_rest <= 0)
	// make probablity 1/DIFFICULTY
	{
		left_hole = HOLE_WIDTH;
		left_times_to_jump_or_rest = HOLE_WIDTH + REST_WIDTH;
	}

	// make a new land as a hole or ground;
	if (left_hole-- > 0)
		land[i] = HOLE;
	else
		land[i] = GROUND;

	left_times_to_jump_or_rest--;
}
