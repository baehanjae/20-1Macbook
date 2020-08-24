#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include <unistd.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

#include "Console.h"

// use this structure to send parameter to the child threads
typedef struct {
	int width, height;		// screen size
	int x, y;				// current coordinate of the ball
	int dx, dy;				// velocity of the ball
} ThreadParam;

// initialize the parameter for the thread function
void InitParam(ThreadParam *param, int width, int height);

// thread function to move a ball
void* MoveBall(void *vparam);

int repeat = TRUE;

int main(int argc, char *argv[])
{
	// get <# of balls> from command line parameter
	int no_thread = 0;
	if(argc > 1)
		no_thread = atoi(argv[1]);
	if(no_thread == 0)				// for safety
		no_thread = 5;				// default value of <# of threads>

	srand(time(NULL));

	int width = getWindowWidth();
	int height = getWindowHeight() - 1;
	int t = 0;						// thread index

	EnableCursor(0);				// hide cursor

	clrscr();
	printf("screen size = %d x %d\n", width, height);
	printf("Press ESC to quit!\n");

//	initialize location and velocity of ball
	// int x = rand() % width + 1;
	// int y = rand() % height + 1;
	// int dx = rand() % 7 - 3;		// dx in [-3, +3]
	// int dy = rand() % 5 - 2;		// dy in [-2, +2]

// TO DO: modify the above code to represent multiple balls
//	1. Declare an array of ThreadParam whose length is no_thread.
//		ex) ThreadParam param[no_thread];
//  2. Initialize each of param[t] by calling InitParam()

	ThreadParam* param;
	param = (ThreadParam*)malloc(sizeof(ThreadParam)*no_thread);

	for(t=0;t<no_thread;t++)
	{
		InitParam(param+t,width,height);
	}

	// animate the bouncing ball
	// while(repeat){
	// 	// if the user presses ESC, terminate
	// 	// DO NOT copy the next 4 lines to MoveBall()
	// 	if(kbhit()){			// check if the a key was pressed
	// 		if(getch() == 27)	// 27: ASCII code of ESC
	// 			break;
	// 	}
	//
	// 	// save current coordinate
	// 	int oldx = x;
	// 	int oldy = y;
	//
	// 	// update horizontal coordinate
	// 	x += dx;
	//
	// 	// horizontal bouncing
	// 	if(x <= 0){
	// 		x = 1 + (1 - x);
	// 		dx = -dx;
	// 	} else if(x > width){
	// 		x = width - (x - width) - 1;
	// 		dx = -dx;
	// 	}
	//
	// 	// update vertical coordinate
	// 	y += dy;
	//
	// 	// vertical bouncing
	// 	if(y <= 0){
	// 		y = 1 + (1 - y);
	// 		dy = -dy;
	// 	} else if(y > height){
	// 		y = height - (y - height) - 1;
	// 		dy = -dy;
	// 	}
	//
	// 	// delete previous ball
	// 	PrintXY(oldx, oldy, " ");
	//
	// 	// draw new ball
	// 	PrintXY(x, y, "*");
	//
	// 	// delay
	// 	MySleep(50);
	// }

// TO DO: extend the above animation code to animate multiple balls using threads

//	1. Move the above while-loop to the 'void* MoveBall(void *vparam);' below.

//	2. Launch threads using MoveBall() function passing &param[t]

	pthread_t* tid;
	pthread_attr_t* attr;

	tid = (pthread_t*)malloc(sizeof(pthread_t)*no_thread);
	attr = (pthread_attr_t*)malloc(sizeof(pthread_attr_t)*no_thread);

	for(t=0;t<no_thread;t++)
	{
		pthread_attr_init(attr+t);
		pthread_create(tid+t,attr+t,MoveBall,param+t);
	}

//	3. Wait for ESC key
//		while(getch() != 27)
//			MySleep(1000);

	while(getch() != 27){
		MySleep(1000);
}
//	4. Terminate the child threads by setting repeat to FALSE (0)

	repeat = FALSE;

//  5. Wait for the child threads to terminate (call pthread_join())
	for(t=0;t<no_thread;t++) pthread_join(tid[t],NULL);

	clrscr();
	gotoxy(1, 1);
	printf("Bye!\n");

	EnableCursor(1);			// enable cursor

	free(param);
	free(tid);
	free(attr);

	return 0;
}

void InitParam(ThreadParam *param, int width, int height)
{
	// TO DO: implement this function to initialize param

	// fill param->width, param->height using the parameters
	param->width = width;
	param->height = height;

	// set x to a random number in [1, width]
	// set y to a random number in [1, height]
	// set dx to a random number in [-3, +3]
	// set dy to a random number in [-2, +2]

	// srand(time(NULL));
	param->x = rand() % width + 1;
	param->y = rand() % height + 1;
	param->dx = rand() % 7 - 3;		// dx in [-3, +3]
	param->dy = rand() % 5 - 2;		// dy in [-2, +2]
}

void* MoveBall(void *vparam)
{
	

	ThreadParam *param = (ThreadParam*) vparam;




	// TO DO: implement this function
	//	move the animation code (the while-loop in the main()) here and modify it to work correctly
	//	delete the if(kbhit()) part

	int x = param->x;
	int y = param->y;
	int dx = param->dx;
	int dy = param->dy;
	int width = param->width;
	int height = param->height;

	while(repeat){
		// if the user presses ESC, terminate
		// DO NOT copy the next 4 lines to MoveBall()
		// if(kbhit()){			// check if the a key was pressed
		// 	if(getch() == 27)	// 27: ASCII code of ESC
		// 		break;
		// }

		// save current coordinate
		int oldx = x;
		int oldy = y;

		// update horizontal coordinate
		x += dx;

		// horizontal bouncing
		if(x <= 0){
			x = 1 + (1 - x);
			dx = -dx;
		} else if(x > width){
			x = width - (x - width) - 1;
			dx = -dx;
		}

		// update vertical coordinate
		y += dy;

		// vertical bouncing
		if(y <= 0){
			y = 1 + (1 - y);
			dy = -dy;
		} else if(y > height){
			y = height - (y - height) - 1;
			dy = -dy;
		}

		// delete previous ball
		PrintXY(oldx, oldy, " ");

		// draw new ball
		PrintXY(x, y, "*");

		// delay
		MySleep(50);
	}

	pthread_exit(0);

	return NULL;
}
