#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>  // For va_start, etc.
#include <math.h>

#include "window.hpp"
#include "graph_screen.hpp"


#define PI 3.14159265
#define sgn(x) ((x<0)?-1:((x>0)?1:0))

extern int display;

graph_screen* graph_instance = NULL;

static unsigned char b[128][64];


void graph_update_task(unsigned int wait_ms)
{
	while (true)
	{
		//graph_instance->mMutex.lock();
		graph_instance->updateScreen();
		//graph_instance->mMutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
	}
}

graph_screen::graph_screen() : screen()
, cursorX(0)
, cursorY(0)
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
	// http://www.brackeen.com/vga/source/djgpp20/lines.c.html
	int dx,dy,sdx,sdy,px,py,dxabs,dyabs,i;
	float slope;

	dx=x2-x1;      /* the horizontal distance of the line */
	dy=y2-y1;      /* the vertical distance of the line */
	dxabs=abs(dx);
	dyabs=abs(dy);
	sdx=sgn(dx);
	sdy=sgn(dy);
	if (dxabs>=dyabs) /* the line is more horizontal than vertical */
	{
		slope=(float)dy / (float)dx;
		for(i=0;i!=dx;i+=sdx)
		{
			px=i+x1;
			py=slope*i+y1;
			setpixel(px,py);
		}
	}
	else /* the line is more vertical than horizontal */
	{
		slope=(float)dx / (float)dy;
		for(i=0;i!=dy;i+=sdy)
		{
			px=slope*i+x1;
			py=i+y1;
			setpixel(px,py);
		}
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

	static int offset = 0;
	const double factor = PI / 180;
	double sinus = 0, cosinus = 0, angle = 0;
	int x = 0, y = 0;
	int radius = 25;
	int origin_x = 64 + offset;
	int origin_y = 31 + offset;

	initBuffer();

	drawCircle(origin_x, origin_y, radius, false);
	drawCircle(origin_x, origin_y, 3, true);
	setpixel(origin_x, origin_y);

	setpixel(origin_x+radius-1, origin_y); // 3
	setpixel(origin_x+radius-2, origin_y); // 3
	setpixel(origin_x+radius-1, origin_y-1); // 3
	setpixel(origin_x+radius-2, origin_y+1); // 3

	setpixel(origin_x, origin_y+radius-1); // 6
	setpixel(origin_x, origin_y+radius-2); // 6
	setpixel(origin_x-1, origin_y+radius-1); // 6
	setpixel(origin_x+1, origin_y+radius-2); // 6

	setpixel(origin_x-radius+1, origin_y); // 9
	setpixel(origin_x-radius+2, origin_y); // 9
	setpixel(origin_x-radius+1, origin_y-1); // 9
	setpixel(origin_x-radius+2, origin_y+1); // 9

	setpixel(origin_x, origin_y-radius+1); // 12
	setpixel(origin_x, origin_y-radius+2); // 12
	setpixel(origin_x-1, origin_y-radius+1); // 12
	setpixel(origin_x+1, origin_y-radius+2); // 12

	time_t jetzt;
	struct tm j;
	time(&jetzt);
	j = *localtime(&jetzt);
	printf("%02i:%02i:%02i\n", j.tm_hour, j.tm_min, j.tm_sec);

	angle = (360.0/60*j.tm_sec)-180;
	sincos((angle*-1)*factor, &sinus, &cosinus);
	x = origin_x+sinus*radius;
	y = origin_y+cosinus*radius;
	drawLine(origin_x, origin_y, x, y);

	angle = (360.0/60*j.tm_min)-180;
	sincos((angle*-1)*factor, &sinus, &cosinus);
	x = origin_x+sinus*radius*0.85;
	y = origin_y+cosinus*radius*0.85;
	drawLine(origin_x, origin_y, x, y);

	angle = 360.0/(12.0)*((j.tm_hour%12))+j.tm_min/2;
	angle -= 180;
	sincos((angle*-1)*factor, &sinus, &cosinus);
	x = origin_x+sinus*radius*0.7;
	y = origin_y+cosinus*radius*0.7;
	drawLine(origin_x, origin_y, x, y);

	//printBuffer();
	drawBuffer();

	static int lastMin = j.tm_min;
	if (j.tm_min != lastMin )
	{
		offset = (offset+1)%4;
		lastMin = j.tm_min;
	}

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
			t1 = new std::thread(graph_update_task, 100);
	}
	else
	{
		mMutex.lock();
	}
	screen::setVisible(v);
}

