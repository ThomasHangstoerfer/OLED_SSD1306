#ifndef font_hpp
#define font_hpp

class font
{
public:
	font();
	virtual ~font();

	
	enum FontId {
		FONT_16x16 = 0,
		FONT_8x8   = 1
	};
	static font* createFont(FontId fontId);

protected:
	unsigned int mHeightInPx;
};


#endif // font_hpp
