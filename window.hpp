#ifndef display_window_hpp
#define display_window_hpp


#include <vector>
#include "font.hpp"

class window
{

public:

	window();
	window(const window& w);
	window(int x1, int y1, int x2, int y2, bool isInverted);
	window(int x1, int y1, std::string content);

	void setContent(std::string content);
	void setContent(std::vector<unsigned char> content);
	void setContent(std::vector<char> content);
	void setRange(int x1, int y1, int x2, int y2 );

	void setInverted(bool inv) { mIsInverted = inv; }
	void setFontId(font::FontId fontId);

	void update();
	void dump();

	friend std::ostream& operator<< (std::ostream& stream, const window& w)
	{
		//stream << "window x1 = " << w.mX1;
		//stream << w.mX1;
		return stream;
	}

private:

	int mX1;
	int mY1;
	int mX2;
	int mY2;

	std::vector<unsigned char> mByteContent;
	std::vector<char> mCharContent;
	std::string mStringContent;
	bool mIsInverted;
	font::FontId mFontId;
	font* mFont;
};

#endif
