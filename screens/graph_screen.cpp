#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>  // For va_start, etc.

#include "window.hpp"
#include "graph_screen.hpp"


extern int display;

graph_screen* graph_instance = NULL;

static unsigned char b[128][64];


void graph_update_task(unsigned int wait)
{
	while (true)
	{
		//graph_instance->mMutex.lock();
		graph_instance->updateScreen();
		//graph_instance->mMutex.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(wait));
	}
}

graph_screen::graph_screen() : screen()
, cursorX(1)
, cursorY(1)
{
	graph_instance = this;
}

void initBuffer()
{
	for (int i = 0; i < 128; i++)
		for (int j = 0; j < 64; j++ )
			b[i][j] = 0;
}
void setpixel(unsigned char x, unsigned char y)
{
	b[x][y] = 1;
}

void flippixel(unsigned char x, unsigned char y)
{
	//printf("flippixel(x = %i y = %i) oldValue=%i\n", x, y, b[x][y]);
	b[x][y] = b[x][y]?0:1;
}

void printBuffer()
{
	printf(" ");
	for (int i = 0; i < 128; i++)
		printf("_");
	printf("\n");
	for (int j = 0; j < 64; j++ )
	{
		printf("|");
		for (int i = 0; i < 128; i++)
		{
			printf("%s", b[i][j] ? "0" : " ");
			//if ( j % 64 == 0 )
		}
		printf("|\n");
	}
	printf("|");
	for (int i = 0; i < 128; i++)
		printf("_");
	printf("|\n");
}

extern void render(int display, char *bitmap);
void drawBuffer()
{
	unsigned char cur_pix = 0;
	for (int l = 0; l < 8; l++ )
	{
		for (int c = 0; c < 16; c++ )
		{
			char s[8] = {
				0b00000000,
				0b00000000,
				0b00000000,
				0b00000000,
				0b00000000,
				0b00000000,
				0b00000000,
				0b00000000,
			};
			for (int u = 0; u < 8; u++ )
			{
				for (int v = 0; v < 8; v++ )
				{
					cur_pix = b[c*8+v][l*8+u];
					s[u] = s[u] + (cur_pix<<v);
					//printf("l=%i c=%i u=%i v=%i s[%i]=%i b[%03i|%03i]\n", l, c, u, v, u, s[u], c*8+v, l*8+u);
				}
			}
			render(display, s);
		}
	}
}


void drawCircle(int center_x, int center_y, int radius, bool filled)
{
	for(int y=-radius; y<=radius; y++)
	{
		for(int x=-radius; x<=radius; x++)
		{
			if ( filled )
			{
				if(x*x+y*y <= radius*radius)
					setpixel(center_x+x, center_y+y);
			}
			else
			{
				int diff = x*x+y*y - radius*radius;
				if( abs(diff) <= 25 )
					setpixel(center_x+x, center_y+y);
			}
		}
	}
}

void drawLine(int x1, int y1, int x2, int y2)
{

	//printf("drawLine(x1 = %i, y1 = %i, x2 = %i, y2 = %i)\n", x1, y1, x2, y2);

	int dx, dy, p, end;
	int x, y;

	dx = abs(x1 - x2);
	dy = abs(y1 - y2);
	p = 2 * dy - dx;
	
	if (x1 > x2)
	{
		x = x2;
		y = y2;
		end = x1;
	}
	else
	{
		x = x1;
		y = y1;
		end = x2;
	}
	flippixel(x, y);
	while (x < end)
	{
		x = x + 1;
		if (p < 0)
		{
			p = p + 2 * dy;
		}
		else
		{
			y = y + 1;
			p = p + 2 * (dy - dx);
		}
		flippixel(x, y);
		//printf("(%03i|%03i)\n", x, y);
	}

}

void graph_screen::up()
{
	setpixel(cursorX, cursorY);
	cursorY -= 1;
	setpixel(cursorX, cursorY);
	updateScreen();
}
void graph_screen::down()
{
	setpixel(cursorX, cursorY);
	cursorY += 1;
	setpixel(cursorX, cursorY);
	updateScreen();
}
void graph_screen::left()
{
	setpixel(cursorX, cursorY);
	cursorX -= 1;
	setpixel(cursorX, cursorY);
	updateScreen();
}
void graph_screen::right()
{
	setpixel(cursorX, cursorY);
	cursorX += 1;
	setpixel(cursorX, cursorY);
	updateScreen();
}

void graph_screen::updateScreen()
{
	printf("graph_screen::updateScreen()\n");
	graph_instance->mMutex.lock();

	int radius = 30;
	int origin_x = 64;
	int origin_y = 31;

	initBuffer();

	drawCircle(origin_x, origin_y, radius, false);
	setpixel(origin_x, origin_y);
	drawLine(origin_x, origin_y, origin_x+radius, origin_y+radius);

//	drawLine(25, 25, 127, 63);
//	drawLine(1, 1, 2, 13);
	printf("(%03i|%03i)\n", cursorX, cursorY);
	setpixel(cursorX, cursorY);

//    printBuffer();
	drawBuffer();


	graph_instance->mMutex.unlock();
}

void graph_screen::setVisible(bool v)
{
	static std::thread* t1 = NULL;
	if ( v )
	{
		mMutex.unlock();
		//printf("+++ graph_screen::setVisible(%s)\n", v?"true":"false");
		if ( t1 == NULL )
			t1 = new std::thread(graph_update_task, 1);
	}
	else
	{
		mMutex.lock();
	}
	screen::setVisible(v);
}

