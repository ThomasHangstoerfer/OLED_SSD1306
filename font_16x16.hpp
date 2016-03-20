#ifndef font_16x16_hpp
#define font_16x16_hpp

#include <utility>
#include <map>


class font_16x16 
{
public:

	font_16x16();
	std::pair<char, char> getCharPos(const char c);
	unsigned char getCharId(const char c);

	void getPatch(const char id, const char patchno, char* patch);

	static const char char_byte_width;
	static const char char_height;
	static const char chars_per_line;
	static const unsigned int data_width;
	static const char data_height;


private:
	std::map<char, std::pair<char, char> > mCharMap;



};

#endif // font_16x16_hpp