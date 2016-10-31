
#include <vector>
#include <iostream>
#include <utility>
#include <map>
#include <unistd.h>

#include <stdio.h>
#ifndef NO_WIRING_PI
#include <wiringPiI2C.h>
#endif
#include "font.hpp"
#include "font_16x16.hpp"
#include "window.hpp"



extern void render(int display, char *bitmap);
extern void renderInv(int display, char *bitmap, bool inverted);
extern char font2[128][8];
extern void printTxt(int display, std::string t);
extern int display;

/*

  std::vector<char> content;
  int mychars[] = {'A', 'B', 'C'};
  content.assign (mychars,mychars+3);   // assigning from array.
  w.setContent(content);


*/

window::window()
: mIsInverted(false)
{

}

window::window(int x1, int y1, int x2, int y2, bool isInverted)
	: mX1(x1)
	, mY1(y1)
	, mX2(x2)
	, mY2(y2)
	, mIsInverted(isInverted)
{

}

window::window(const window& w)
{
	mX1 = w.mX1;
	mY1 = w.mY1;
	mX2 = w.mX2;
	mY2 = w.mY2;
	mIsInverted = w.mIsInverted;

	// TODO gets called too often
	printf("copy contructor \n" );
}


void window::dump()
{
	printf("window.dump( mX1=%i mY1=%i mX2=%i mY2=%i mIsInverted=%i )\n", mX1, mY1, mX2, mY2, mIsInverted );
}

void window::setRange(int x1, int y1, int x2, int y2 )
{
	mX1 = x1;
	mY1 = y1;
	mX2 = x2;
	mY2 = y2;
}

void window::setContent(std::vector<unsigned char> content)
{
	printf("window::setContent<unsigned char>(size = %lu)\n", content.size() );
	mByteContent = content;

	for (std::vector<unsigned char>::iterator it = mByteContent.begin() ; it != mByteContent.end(); ++it)
		std::cout << ' ' << ((int)*it);
	std::cout << '\n';
}


void window::update()
{
	//printf("window::update()\n");

#ifndef NO_WIRING_PI

	// restrict draw-range to the configured area
	wiringPiI2CWriteReg8(display, 0x00, 0x21);    // set column address
	wiringPiI2CWriteReg8(display, 0x00, mX1*8);   // column start address


	wiringPiI2CWriteReg8(display, 0x00, 0x22); // set page address
	wiringPiI2CWriteReg8(display, 0x00, mY1);  // page start address
	wiringPiI2CWriteReg8(display, 0x00, mY2);  // page end address
	//printf("page   start=%i end=%i\n", mY1, mY2);
#endif

	if ( !mByteContent.empty() )
	{	
		for (std::vector<unsigned char>::iterator it = mByteContent.begin() ; it != mByteContent.end(); ++it)
		{
			std::cout << "mByteContent: writing " << ((int)*it) << std::endl;
			//render(display, *it);
		}
	}
	else
	{
		// no content to display
	}

#ifndef NO_WIRING_PI
	wiringPiI2CWriteReg8(display, 0x00, 0x21); // set column address
	wiringPiI2CWriteReg8(display, 0x00, 0x00);
	wiringPiI2CWriteReg8(display, 0x00, 127);

	wiringPiI2CWriteReg8(display, 0x00, 0x22); // set page address
	wiringPiI2CWriteReg8(display, 0x00, 0x00);
	wiringPiI2CWriteReg8(display, 0x00, 0x07);
#endif
}
