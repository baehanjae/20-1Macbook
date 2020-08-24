// If the comments are hard to read because of the color,
// type ':set background=dark'

/***

	This program moves bars left and right until the user types 'q' key.
	Usage: $ ./bar <no_bar>

	Currently, it moves only one bar.
	Extend this program to move multiple bars using multiple threads.
	- Launch multiple threads, each of which moves a bar.
	- The vertical interval between bars should be height / (no_bar + 1).

	Compile with Console.c with -pthread -D_LINUX flag
		Ex) gcc bar.c Console.c -pthread -D_LINUX


***/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pthread.h>

#include "Console.h"

#define TRUE 1
#define FALSE 0

typedef struct {
	int width, height;		// screen size
	char len;
	int x, y;				// current coordinate of the ball
	int dx;				// velocity of the ball
	int idx;
} ThreadParam;

int repeat = TRUE;

void DrawBar(int x, int y, int len, char c);

//func for threads
void* MoveBall(void *vparam);
void InitParam(ThreadParam *param, int width, int height,int no_bar,int idx);

int main(int argc, char *argv[])
{
	int no_bar = 1;

	if(argc > 1)
		no_bar =atoi(argv[1]);
	if(no_bar == 0)
		no_bar = 5;

	srand(time(NULL));
	int width = getWindowWidth();
	int height = getWindowHeight() - 1;
	int t = 0;						// thread index

	// char len = rand() % 6 + 5;	// a random number in [5, 10]
	// int step = rand() % 4 + 2;	// a random number in [2, 5]
	// int x = rand() % (width - step - 1) + 1;
	//
	// // vertical interval between bars = height / (no_bar + 1)
	// float interval = height / (float)(no_bar + 1);
	// int y = (int) interval;
	//
	// int dx = step;
	// if(rand() % 2 == 0)
	// 	dx = -dx;

	clrscr();
	gotoxy(1, 1);
	printf("Press 'q' to quit.\n");

//  Do not enable the next line until you complete the program.
//	EnableCursor(FALSE);
	EnableCursor(0);				// hide cursor

	ThreadParam* param;
	param = (ThreadParam*)malloc(sizeof(ThreadParam)*no_bar);

	for(t=0;t<no_bar;t++)
	{
		InitParam(param+t,width,height,no_bar,t);
	}

	pthread_t* tid;
	pthread_attr_t* attr;

	tid = (pthread_t*)malloc(sizeof(pthread_t)*no_bar);
	attr = (pthread_attr_t*)malloc(sizeof(pthread_attr_t)*no_bar);

	for(t=0;t<no_bar;t++)
	{
		pthread_attr_init(attr+t);
		pthread_create(tid+t,attr+t,MoveBall,param+t);
	}

	// char c = 'A' + 0;
	// DrawBar(x, y, len, 1);


	// TO DO: Wait for 'q' key. (use getch() and while statement)
	while(getch() != 'q'){
		MySleep(1000);
}

	// TO DO: Stop threads by setting repeat to FALSE

	repeat = FALSE;
	// TO DO: Wait for the child threads
	for(t=0;t<no_bar;t++) pthread_join(tid[t],NULL);

	clrscr();
	gotoxy(1, 1);
	printf("Bye!\n");

	free(param);
	free(tid);
	free(attr);

//  Do not enable the next line until you complete the program.
//	EnableCursor(TRUE);
	EnableCursor(1);				// hide cursor

	return 0;
}

void DrawBar(int x, int y, int len, char c)
{
	pthread_mutex_lock(&display_mutex);

	gotoxy(x, y);
	for(int i = 0; i < len; i++)
		putchar(c);
	fflush(stdout);

	pthread_mutex_unlock(&display_mutex);
}

void InitParam(ThreadParam *param, int width, int height,int no_bar,int idx)
{
	param->width = width;
	param->height = height;

	param->len = rand() % 6 + 5;	// a random number in [5, 10]
	int step = rand() % 4 + 2;	// a random number in [2, 5]
	param->x = rand() % (width - step - 1) + 1;


	// vertical interval between bars = height / (no_bar + 1)
	float interval = height / (float)(no_bar + 1);
	param->y = (int) interval*(idx+1);

	param->dx = step;
	if(rand() % 2 == 0)
		param->dx = -(param->dx);

	param->idx = idx;
}

void* MoveBall(void *vparam)
{

	ThreadParam *param = (ThreadParam*) vparam;

	int x = param->x;
	int y = param->y;
	int dx = param->dx;
	char len = param->len;
	int width = param->width;
	int height = param->height;
	int idx = param->idx;

	while(repeat){
		int oldx = x;

		// remove the next two lines in the thread function
		// if(kbhit() && getch() == 'q')
		// 	break;

		x += dx;
		if(x <= 0){
			dx = -dx;
			x = 1 - x;
		}

		if(x > width - len + 1){
			dx = -dx;
			x = width - len + 1 - (x - width + len - 1);
		}

		char c = 'A' + idx;

		DrawBar(oldx, y, len, ' ');
		DrawBar(x, y, len, c);

		MySleep(50);
	}

}
