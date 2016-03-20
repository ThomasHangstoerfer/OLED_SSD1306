
#include <vector>
#include <iostream>
#include <utility>
#include <map>

#include <stdio.h>
#include <wiringPiI2C.h>

#include "window.hpp"


#include "font_16x16.xbm"


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


	char out[8];

	std::map<char, std::pair<char, char> > charMap;
	charMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(0 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('!', std::make_pair(1 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('"', std::make_pair(2 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('#', std::make_pair(3 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('$', std::make_pair(4 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('%', std::make_pair(5 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('&', std::make_pair(6 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('\'',std::make_pair(7 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('(', std::make_pair(8 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >(')', std::make_pair(9 , 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('*', std::make_pair(10, 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('#', std::make_pair(11, 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >(',', std::make_pair(12, 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('-', std::make_pair(13, 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('.', std::make_pair(14, 0)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('/', std::make_pair(15, 0)) );

	charMap.insert ( std::pair<char, std::pair<char, char> >('0', std::make_pair(0 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('1', std::make_pair(1 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('2', std::make_pair(2 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('3', std::make_pair(3 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('4', std::make_pair(4 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('5', std::make_pair(5 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('6', std::make_pair(6 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('7', std::make_pair(7 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('8', std::make_pair(8 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('9', std::make_pair(9 , 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >(':', std::make_pair(10, 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >(';', std::make_pair(11, 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('<', std::make_pair(12, 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('=', std::make_pair(13, 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('>', std::make_pair(14, 1)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('?', std::make_pair(15, 1)) );

	charMap.insert ( std::pair<char, std::pair<char, char> >('@', std::make_pair(0 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('A', std::make_pair(1 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('B', std::make_pair(2 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('C', std::make_pair(3 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('D', std::make_pair(4 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('E', std::make_pair(5 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('F', std::make_pair(6 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('G', std::make_pair(7 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('H', std::make_pair(8 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('I', std::make_pair(9 , 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('J', std::make_pair(10, 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('K', std::make_pair(11, 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('L', std::make_pair(12, 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('M', std::make_pair(13, 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('N', std::make_pair(14, 2)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('O', std::make_pair(15, 2)) );

	charMap.insert ( std::pair<char, std::pair<char, char> >('P', std::make_pair(0 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('Q', std::make_pair(1 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('R', std::make_pair(2 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('S', std::make_pair(3 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('T', std::make_pair(4 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('U', std::make_pair(5 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('V', std::make_pair(6 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('W', std::make_pair(7 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('X', std::make_pair(8 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('Y', std::make_pair(9 , 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('Z', std::make_pair(10, 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(11, 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('\\',std::make_pair(12, 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(13, 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(14, 3)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('_', std::make_pair(15, 3)) );

	charMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(0 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('a', std::make_pair(1 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('b', std::make_pair(2 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('c', std::make_pair(3 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('d', std::make_pair(4 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('e', std::make_pair(5 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('f', std::make_pair(6 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('g', std::make_pair(7 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('h', std::make_pair(8 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('i', std::make_pair(9 , 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('j', std::make_pair(10, 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('k', std::make_pair(11, 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('l', std::make_pair(12, 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('m', std::make_pair(13, 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('n', std::make_pair(14, 4)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('o', std::make_pair(15, 4)) );

	charMap.insert ( std::pair<char, std::pair<char, char> >('p', std::make_pair(0 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('q', std::make_pair(1 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('r', std::make_pair(2 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('s', std::make_pair(3 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('t', std::make_pair(4 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('u', std::make_pair(5 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('v', std::make_pair(6 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('w', std::make_pair(7 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('x', std::make_pair(8 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('y', std::make_pair(9 , 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('z', std::make_pair(10, 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('{', std::make_pair(11, 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('c', std::make_pair(12, 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('}', std::make_pair(13, 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('~', std::make_pair(14, 5)) );
	charMap.insert ( std::pair<char, std::pair<char, char> >('^', std::make_pair(15, 5)) );

  	std::pair<char, char> cPos = charMap['3'];

  	int charX = cPos.first;
  	int charY = cPos.second;

	int stride = font_16x16_char_byte_width * font_16x16_chars_per_line;
 	int startPos = (charX * font_16x16_char_byte_width) + (charY * font_16x16_char_height * stride);

 	for (int i=0; i<8; i++)
 	{
 		printf("0x%02x\n", font_16x16_data_bits[startPos + i * stride] );
 		out[i] = ~font_16x16_data_bits[startPos + i * stride];
 	}
 	render(display, out);
 	printf("\n");

 	for (int i=0; i<8; i++)
 	{
 		printf("0x%02x\n", font_16x16_data_bits[startPos+1 + i * stride] );
 		out[i] = ~font_16x16_data_bits[startPos+1 + i * stride];
 	}
 	render(display, out);
 	printf("\n");

 	for (int i=0; i<8; i++)
 	{
 		printf("0x%02x\n", font_16x16_data_bits[ (startPos+(stride*8)) + i * stride] );
 		out[i] = ~font_16x16_data_bits[(startPos+(stride*8)) + i * stride];
 	}
 	render(display, out);
 	printf("\n");

 	for (int i=0; i<8; i++)
 	{
 		printf("0x%02x\n", font_16x16_data_bits[ (startPos+(stride*8)+1) + i * stride] );
 		out[i] = ~font_16x16_data_bits[(startPos+(stride*8)+1) + i * stride];
 	}
 	render(display, out);
 	printf("\n");


/*
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
*/
	printf("window::update(3)\n");

	wiringPiI2CWriteReg8(display, 0x00, 0x21); // set column address
	wiringPiI2CWriteReg8(display, 0x00, 0x00);
	wiringPiI2CWriteReg8(display, 0x00, 127);

	wiringPiI2CWriteReg8(display, 0x00, 0x22); // set page address
	wiringPiI2CWriteReg8(display, 0x00, 0x00);
	wiringPiI2CWriteReg8(display, 0x00, 0x07);
}
