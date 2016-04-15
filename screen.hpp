#ifndef display_screen_hpp
#define display_screen_hpp


#include <vector>
#include <string>

#include "window.hpp"

class screen
{
public:

	screen();
	void update();
	void addWindow(std::string name, window w);
	void removeWindow(std::string name);

	void setVisible(bool v);

	void dump();

private:
	bool mVisible;
	std::vector<window> mWindows;
};

#endif