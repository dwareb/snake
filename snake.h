//Michael Fulton
//Header for snake game that I put together.

#ifndef SNAKE
#define SNAKE


#include<termios.h>
#include<unistd.h>
#include<iostream>
#include<ctime>

const int XMAX = 20;
const int YMAX = 15;

struct snode
{
	int x;
	int y;
	snode * next;
};

class board
{	
	public:
		board();
		~board();
		bool push(int x, int y); 
		void pop(); 
		void draw();
		void apple();
		int app[2];
		int score;
	private:
		void clear(snode *& rear);
		snode * rear;
		char screen[YMAX][XMAX];
};


class imode
{
	public:
		imode();
		~imode();

		void raw();
		void standardmode();
	
	private:
		struct termios original;
};

class sgame
{
	public:
		sgame();
		~sgame();

		void play();
	private:
		char getkey(char p);
};

#endif
