//Michael Fulton
//Little snake game that I put wrote to play around with.
//There's no commenting, as this is simply an easter-egg.
//In the main program to activate the game, uncomment the snake header.

#include"snake.h"


board::board():app{0},score(0),rear(NULL)
{
	for(int i = 0; i < YMAX; ++i){
		for(int j = 0; j < XMAX; ++j)
			screen[i][j] = ' ';
	}
}


board::~board()
{
	if(!rear)
		return;
	clear(rear->next);
}	

void board::clear(snode * & rear)
{
	if(rear == this->rear){
		delete rear;
		this->rear = NULL;
		return;
	}
	clear(rear->next);
	delete rear;
	return;
}


bool board::push(int x, int y)
{
	snode * current;
	if(screen[y][x] == 'x')
	{
		return false;
	}
	if(!rear){
		rear = new snode;
		rear->x = x;
		rear->y = y;
		rear->next = rear;
	}
	current = new snode;
	current->x = x;
	current->y = y;
	current->next = rear->next;
	rear->next = current;
	rear = current;
	screen[y][x] = 'x';
	return true;
}

void board::pop()
{
	snode * temp;
	if(!rear)
		return;
	if(rear == rear->next)
	{
		screen[rear->y][rear->x] = ' ';
		delete rear;
		rear = NULL;
		return;
	}
	temp = rear->next;
	rear->next = temp->next;
	screen[temp->y][temp->x] = ' ';
	delete temp;
	return;
}

void board::apple()
{
	bool placed = false;
	do{
		app[0] = rand()%YMAX;
		app[1] = rand()%XMAX;
		if(screen[app[0]][app[1]] == ' ')
		{
			screen[app[0]][app[1]] = 'o';
			placed = true;
		}
	}while(!placed);
	return;

}

void board::draw()
{
	system("clear");
	std::cout << "SCORE: ";
	std::cout.width(3);
	std::cout.fill('0');
	std::cout << score << std::endl;
	std::cout << "\u2554";
	for(int i = 0; i < (XMAX * 2); ++i)
	{
		std::cout << "\u2550";
	}
	std::cout << "\u2557" << std::endl;
	for(int i = 0; i < YMAX; ++i){
		std::cout << "\u2551";
		for(int j = 0; j < XMAX; ++j){
			if(screen[i][j] == 'x')
				std::cout << "\u2588\u2588";
			else if (screen[i][j] == 'o')
				std::cout << "\u25d6\u25d7";
			else
				std::cout << "  ";
		}
		std::cout << "\u2551" << std::endl;

	}
	std::cout << "\u255a";
	for(int i = 0; i < (XMAX * 2); ++i)
	{
		std::cout << "\u2550";
	}
	std::cout << "\u255d" << std::endl;
	return;
}

sgame::sgame(){}

sgame::~sgame(){}

void sgame::play()
{
	board snake;
	bool game_over = false;
	int x = 2;
	int y = 0;
	char dir = 'd';
	char pdir = 'd';
	char ndir = 'a';
	int slp = 150000;

	srand(time(NULL));
	snake.push(0,0);
	snake.push(1,0);
	snake.push(2,0);
	snake.apple();
	snake.draw();
	std::cout << "Use arrow keys, or wasd to control. PRESS ENTER TO START";
	std::cin.ignore(1000, '\n');
	imode change;
	change.raw();
	do{
		dir = getkey(pdir);
		if(dir == ndir || (dir != 'w' && dir != 'a' && dir != 's' && dir != 'd' && dir != 'q'))
			dir = pdir;
		if(dir == 'w'){
			--y;
			pdir = 'w';
			ndir = 's';
		}else if(dir == 's'){
			++y;
			pdir = 's';
			ndir = 'w';
		}else if(dir == 'a'){
			--x;
			pdir = 'a';
			ndir = 'd';
		}else if(dir == 'd'){
			++x;
			pdir = 'd';
			ndir = 'a';
		}else if(dir == 'q'){
			game_over = true;
		}
		if((x == -1 || x == XMAX || y == -1 || y == YMAX) && !game_over)
			game_over = true;
		else{
			if(snake.push(x,y)){
				if(x == snake.app[1] && y == snake.app[0]){
					++snake.score;
					snake.apple();
					if(slp > 50000)
						slp-=1000;
				}
				else
					snake.pop();
			}else{
				game_over = true;
			}
		}
		snake.draw();
		if(game_over)
			std::cout << "GAME OVER" << std::endl;
		usleep(slp);
		
	}while(dir != 'q' && !game_over);
	change.standardmode();
	std::cout << "Press enter to continue" << std::endl;
	std::cin.ignore(1000, '\n');
	return;
}

char sgame::getkey(char p)
{
	char press;
	char arrows[2];
	if(read(STDIN_FILENO, &press, 1) != 1)
		return p;
	if(press == '\x1b'){
		if(read(STDIN_FILENO, &arrows[0], 1) != 1) return p;
		if(read(STDIN_FILENO, &arrows[1], 1) != 1) return p;
		if(arrows[0] == '['){
			switch(arrows[1]){
				case 'A': return 'w';
				case 'B': return 's';
				case 'C': return 'd';
				case 'D': return 'a';
			}
		}
		return p;
	}
	tcflush(STDIN_FILENO, TCIFLUSH);
	return press;
}

imode::imode()
{
	tcgetattr(STDIN_FILENO, & original);
	return;
}

imode::~imode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, & original);
	return;
}

void imode::raw()
{
	struct termios raw = original;
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	return;
}

void imode::standardmode()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, & original);
	return;
}
