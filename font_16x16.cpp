


#include <stdio.h>

#include "font_16x16.hpp"
#include "font_16x16.xbm"


const char font_16x16::char_byte_width =   2;
const char font_16x16::char_height     =  16;
const char font_16x16::chars_per_line  =  16;
const unsigned int font_16x16::data_width = 256;
const char font_16x16::data_height     =  96;



void font_16x16::getPatch(const char id, const char patchno, char* patch)
{
	// TOOD: check id and patchno for validity

 	int charX = id % chars_per_line;
  	int charY = id / chars_per_line;
  	//printf("getPatch(id=%i, patchno=%i) charX = %i charY = %i\n", id, patchno, charX, charY);
	
	int stride = char_byte_width * chars_per_line;
 	int startPos = (charX * char_byte_width) + (charY * char_height * stride);
 	int dataoffset = 0;

 	// TODO remove this ugly hack
 	switch(patchno)
 	{
 		case 0: dataoffset = 0; break;
 		case 1: dataoffset = 1; break;
 		case 2: dataoffset = stride*8; break;
 		case 3: dataoffset = stride*8+1; break;
 	}
 	//printf("stride = %i startPos = %i\n", stride, startPos);

 	for (int i=0; i<8; i++)
 	{
 		//printf("0x%02x\n", font_16x16_data_bits[startPos + dataoffset + i * stride] );
 		patch[i] = ~font_16x16_data_bits[startPos + dataoffset + i * stride];
 	}
}


unsigned char font_16x16::getCharId(const char c)
{
	unsigned char ret = mCharMap[c].first + mCharMap[c].second * chars_per_line;
	//printf("getCharId('%c') = %i\n", c, ret);
	return ret;
}

font_16x16::font_16x16()
{
	mCharMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(0 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('!', std::make_pair(1 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('"', std::make_pair(2 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('#', std::make_pair(3 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('$', std::make_pair(4 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('%', std::make_pair(5 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('&', std::make_pair(6 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('\'',std::make_pair(7 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('(', std::make_pair(8 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >(')', std::make_pair(9 , 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('*', std::make_pair(10, 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('#', std::make_pair(11, 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >(',', std::make_pair(12, 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('-', std::make_pair(13, 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('.', std::make_pair(14, 0)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('/', std::make_pair(15, 0)) );

	mCharMap.insert ( std::pair<char, std::pair<char, char> >('0', std::make_pair(0 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('1', std::make_pair(1 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('2', std::make_pair(2 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('3', std::make_pair(3 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('4', std::make_pair(4 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('5', std::make_pair(5 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('6', std::make_pair(6 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('7', std::make_pair(7 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('8', std::make_pair(8 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('9', std::make_pair(9 , 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >(':', std::make_pair(10, 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >(';', std::make_pair(11, 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('<', std::make_pair(12, 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('=', std::make_pair(13, 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('>', std::make_pair(14, 1)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('?', std::make_pair(15, 1)) );

	mCharMap.insert ( std::pair<char, std::pair<char, char> >('@', std::make_pair(0 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('A', std::make_pair(1 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('B', std::make_pair(2 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('C', std::make_pair(3 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('D', std::make_pair(4 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('E', std::make_pair(5 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('F', std::make_pair(6 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('G', std::make_pair(7 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('H', std::make_pair(8 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('I', std::make_pair(9 , 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('J', std::make_pair(10, 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('K', std::make_pair(11, 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('L', std::make_pair(12, 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('M', std::make_pair(13, 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('N', std::make_pair(14, 2)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('O', std::make_pair(15, 2)) );

	mCharMap.insert ( std::pair<char, std::pair<char, char> >('P', std::make_pair(0 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('Q', std::make_pair(1 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('R', std::make_pair(2 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('S', std::make_pair(3 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('T', std::make_pair(4 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('U', std::make_pair(5 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('V', std::make_pair(6 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('W', std::make_pair(7 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('X', std::make_pair(8 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('Y', std::make_pair(9 , 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('Z', std::make_pair(10, 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(11, 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('\\',std::make_pair(12, 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(13, 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(14, 3)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('_', std::make_pair(15, 3)) );

	mCharMap.insert ( std::pair<char, std::pair<char, char> >(' ', std::make_pair(0 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('a', std::make_pair(1 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('b', std::make_pair(2 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('c', std::make_pair(3 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('d', std::make_pair(4 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('e', std::make_pair(5 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('f', std::make_pair(6 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('g', std::make_pair(7 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('h', std::make_pair(8 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('i', std::make_pair(9 , 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('j', std::make_pair(10, 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('k', std::make_pair(11, 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('l', std::make_pair(12, 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('m', std::make_pair(13, 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('n', std::make_pair(14, 4)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('o', std::make_pair(15, 4)) );

	mCharMap.insert ( std::pair<char, std::pair<char, char> >('p', std::make_pair(0 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('q', std::make_pair(1 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('r', std::make_pair(2 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('s', std::make_pair(3 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('t', std::make_pair(4 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('u', std::make_pair(5 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('v', std::make_pair(6 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('w', std::make_pair(7 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('x', std::make_pair(8 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('y', std::make_pair(9 , 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('z', std::make_pair(10, 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('{', std::make_pair(11, 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('c', std::make_pair(12, 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('}', std::make_pair(13, 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('~', std::make_pair(14, 5)) );
	mCharMap.insert ( std::pair<char, std::pair<char, char> >('^', std::make_pair(15, 5)) );
}

std::pair<char, char> font_16x16::getCharPos(const char c)
{
	return mCharMap[c];
}

