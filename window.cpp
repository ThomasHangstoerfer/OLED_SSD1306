
#include <vector>
#include <iostream>

#include <stdio.h>
#include <wiringPiI2C.h>

#include "window.hpp"


extern void render(int display, char *bitmap);
extern char font2[128][8];
extern void printTxt(int display, std::string t);


window::window()
{

}

window::window(int x1, int y1, int x2, int y2)
	: mX1(x1)
	, mY1(y1)
	, mX2(x2)
	, mY2(y2)
{

}

window::window(const window& w)
{
	mX1 = w.mX1;
	mY1 = w.mY1;
	mX2 = w.mX2;
	mY2 = w.mY2;
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
	printf("window::setContent<unsigned char>(size = %i)\n", content.size() );
	mCharContent.clear();
	mByteContent = content;

	for (std::vector<unsigned char>::iterator it = mByteContent.begin() ; it != mByteContent.end(); ++it)
		std::cout << ' ' << ((int)*it);
	std::cout << '\n';
}

void window::setContent(std::vector<char> content)
{
	printf("window::setContent<char>(size = %i)\n", content.size() );
	mByteContent.clear();
	mCharContent = content;

	for (std::vector<char>::iterator it = mCharContent.begin() ; it != mCharContent.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';
}

void window::update()
{
	printf("window::update()\n");
	int display = 0x3;

	wiringPiI2CWriteReg8(display, 0x00, 0x21); // set column address
	wiringPiI2CWriteReg8(display, 0x00, 0x20);
	wiringPiI2CWriteReg8(display, 0x00, 0x20+8+8-1);

	wiringPiI2CWriteReg8(display, 0x00, 0x22); // set page address
	wiringPiI2CWriteReg8(display, 0x00, 0x04);
	wiringPiI2CWriteReg8(display, 0x00, 0x05);

	printf("window::update(2) mCharContent.size() = %i\n", mCharContent.size() );


	if ( !mCharContent.empty() )
	{	
		for (std::vector<char>::iterator it = mCharContent.begin() ; it != mCharContent.end(); ++it)
		{
			std::cout << "mCharContent: writing " << *it << std::endl;
		    render(display, font2[*it]);
		}
	}
	else if ( !mByteContent.empty() )
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

	printf("window::update(3)\n");

	wiringPiI2CWriteReg8(display, 0x00, 0x21); // set column address
	wiringPiI2CWriteReg8(display, 0x00, 0x00);
	wiringPiI2CWriteReg8(display, 0x00, 127);

	wiringPiI2CWriteReg8(display, 0x00, 0x22); // set page address
	wiringPiI2CWriteReg8(display, 0x00, 0x00);
	wiringPiI2CWriteReg8(display, 0x00, 0x07);
}
