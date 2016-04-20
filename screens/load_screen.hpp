#ifndef load_screen_hpp
#define load_screen_hpp

#include "screen.hpp"

class load_screen : public screen
{
public:
	
	load_screen();

private:
	window wLabel;
	window wValue;
};

#endif // load_screen_hpp