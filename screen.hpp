#ifndef display_screen_hpp
#define display_screen_hpp


#include <map>
#include <string>

#include "window.hpp"

class screen
{
public:

	screen();
	void update();
	void addWindow(std::string name, window w);

	void setVisible(bool v);

private:
	bool mVisible;
	std::map<std::string, window> mWindows;

};

#endif