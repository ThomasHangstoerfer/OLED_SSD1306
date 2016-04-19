
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
, mFontId(font::FONT_16x16)
, mFont(NULL)
{

}

window::window(int x1, int y1, int x2, int y2, bool isInverted)
	: mX1(x1)
	, mY1(y1)
	, mX2(x2)
	, mY2(y2)
	, mIsInverted(isInverted)
	, mFontId(font::FONT_16x16)
	, mFont(NULL)
{

}

window::window(const window& w)
{
	mX1 = w.mX1;
	mY1 = w.mY1;
	mX2 = w.mX2;
	mY2 = w.mY2;
	mStringContent = w.mStringContent;
	mIsInverted = w.mIsInverted;
	mFontId = w.mFontId;
	mFont = font::createFont(w.mFontId);

	// TODO gets called too often
	//printf("copy contructor: %s -> %s\n", w.mStringContent.c_str(), mStringContent.c_str());
}

window::window(int x1, int y1, std::string content)
	: mX1(x1)
	, mY1(y1)
	, mIsInverted(false)
	, mFontId(font::FONT_16x16)
	, mFont(NULL)
{
	// TODO check params for validity
	mStringContent = content;
	mX2 = content.size();
	mY2 = mY1 + /*font_hight_in_pixel/8 - 1  */ 1;
	printf("window( mX1=%i mY1=%i mX2=%i mY2=%i '%s')\n", mX1, mY1, mX2, mY2, mStringContent.c_str() );
}

void window::setFontId(font::FontId fontId)
{
	mFontId = fontId;
	if ( mFontId == font::FONT_16x16)
		mY2 = mY1 + /*font_hight_in_pixel/8 - 1  */ 1;
	else if ( mFontId == font::FONT_8x8 )
	{
		mY2 = mY1;//+1;
		//mX2 += 2;
	}
}

void window::dump()
{
	printf("window.dump( mX1=%i mY1=%i mX2=%i mY2=%i mIsInverted=%i mFontId=%i '%s')\n", mX1, mY1, mX2, mY2, mIsInverted, mFontId, mStringContent.c_str() );
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
	mCharContent.clear();
	mStringContent.clear();
	mByteContent = content;

	for (std::vector<unsigned char>::iterator it = mByteContent.begin() ; it != mByteContent.end(); ++it)
		std::cout << ' ' << ((int)*it);
	std::cout << '\n';
}

void window::setContent(std::vector<char> content)
{
	printf("window::setContent<char>(size = %lu)\n", content.size() );
	mByteContent.clear();
	mStringContent.clear();
	mCharContent = content;

	for (std::vector<char>::iterator it = mCharContent.begin() ; it != mCharContent.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';
}

void window::setContent(std::string content)
{
	printf("window::setContent<string>('%s', size = %lu)\n", content.c_str(), content.size() );
	mByteContent.clear();
	mCharContent.clear();
	mStringContent = content;
}


void window::update()
{
	//printf("window::update()\n");

#ifndef NO_WIRING_PI

	// restrict draw-range to the configured area
	wiringPiI2CWriteReg8(display, 0x00, 0x21);    // set column address
	wiringPiI2CWriteReg8(display, 0x00, mX1*8);   // column start address

	if ( mFontId == font::FONT_16x16)
	{
		wiringPiI2CWriteReg8(display, 0x00, mX1*8 + mX2*8*2-1); // column end address
		//printf("column start=%i end=%i\n", mX1*8, mX1*8 + mX2*8*2-1);
	}
	else //if ( mFontId == font::FONT_8x8 )
	{
		wiringPiI2CWriteReg8(display, 0x00, mX1*8 + mX2*8-1); // column end address
		printf("column start=%i end=%i\n", mX1*8, mX1*8 + mX2*8-1);
	}

	wiringPiI2CWriteReg8(display, 0x00, 0x22); // set page address
	wiringPiI2CWriteReg8(display, 0x00, mY1);  // page start address
	wiringPiI2CWriteReg8(display, 0x00, mY2);  // page end address
	//printf("page   start=%i end=%i\n", mY1, mY2);
#endif

	if ( !mStringContent.empty() )
	{
		//printf("window::update() mStringContent = %s\n", mStringContent.c_str() );
		font_16x16 f16;
		char out[8];
		unsigned char patchno = 0;

		if ( mFontId == font::FONT_16x16 )
		{
			for (int line = 0; line < 2; line++)
			{
				for (int x = 0; x < mStringContent.size(); x++)
				{
					unsigned char id = f16.getCharId(mStringContent[x]);

					f16.getPatch(id, patchno, out);
					char out1[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
					renderInv(display, out, mIsInverted);

					f16.getPatch(id, patchno+1, out);
					renderInv(display, out, mIsInverted);
				}
				patchno+=2;
			}
		}
		else
		{
			for (int x = 0; x < mStringContent.size(); x++)
			{
				render(display, font2[mStringContent[x]]);
			}
		}
	}
	else if ( !mCharContent.empty() )
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

#ifndef NO_WIRING_PI
	wiringPiI2CWriteReg8(display, 0x00, 0x21); // set column address
	wiringPiI2CWriteReg8(display, 0x00, 0x00);
	wiringPiI2CWriteReg8(display, 0x00, 127);

	wiringPiI2CWriteReg8(display, 0x00, 0x22); // set page address
	wiringPiI2CWriteReg8(display, 0x00, 0x00);
	wiringPiI2CWriteReg8(display, 0x00, 0x07);
#endif
}
