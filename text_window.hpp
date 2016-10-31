#ifndef display_text_window_hpp
#define display_text_window_hpp


#include <vector>
#include "font.hpp"
#include "window.hpp"

class text_window : public window
{

public:

	text_window();
	text_window(const text_window& w);
	text_window(int x1, int y1, int x2, int y2, bool isInverted);
	text_window(int x1, int y1, std::string content);

	void setContent(std::string content);
	void setContent(std::vector<unsigned char> content);
	void setContent(std::vector<char> content);
	void setRange(int x1, int y1, int x2, int y2 );

	void setInverted(bool inv) { mIsInverted = inv; }
	void setFontId(font::FontId fontId);

	void update();
	void dump();

	friend std::ostream& operator<< (std::ostream& stream, const text_window& w)
	{
		//stream << "window x1 = " << w.mX1;
		//stream << w.mX1;
		return stream;
	}

private:

	std::string mStringContent;
	font::FontId mFontId;
	font* mFont;
};

#endif
