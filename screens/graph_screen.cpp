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
void graph_update_task(unsigned int wait)
{
	while (true)
	{
		graph_instance->mMutex.lock();
		graph_instance->updateScreen();
		graph_instance->mMutex.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(wait));
	}
}

graph_screen::graph_screen() : screen()
{
	graph_instance = this;
}

static unsigned char b[128][64];
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
	for (int i = 0; i < 128; i++)
		for (int j = 0; j < 64; j++ )
		{
			printf("%s", b[i][j] ? "0" : " ");
			if ( j % 64 == 0 )
				printf("\n");
		}

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
					cur_pix = b[l*8+u][c*8+v];
					s[u] = s[u] + (cur_pix<<v);
					//printf("l=%i c=%i u=%i v=%i s[%i]=%i\n", l, c, u, v, u, s[u]);
				}
			}
			render(display, s);
		}
	}
}

void drawCircle(int center_x, int center_y, int radius)
{
	for(int y=-radius; y<=radius; y++)
	{
		for(int x=-radius; x<=radius; x++)
		{
			if(x*x+y*y <= radius*radius)
				setpixel(center_x+x, center_y+y);
		}
	}
}

void drawLine(int x1, int y1, int x2, int y2)
{

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
	}

}

void graph_screen::updateScreen()
{
	printf("graph_screen::updateScreen()\n");
	int radius = 20;
	int origin_x = 25;
	int origin_y = 25;

	initBuffer();

	drawCircle(origin_x, origin_y, radius);

	drawLine(25, 25, 0, 0);

    printBuffer();
	drawBuffer();


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

