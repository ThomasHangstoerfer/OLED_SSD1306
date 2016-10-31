#ifndef load_screen_hpp
#define load_screen_hpp

#include "screen.hpp"
#include "text_window.hpp"

class load_screen : public screen
{
public:
	
	load_screen();

private:
	text_window wLabel;
	text_window wValue;
};

#endif // load_screen_hpp