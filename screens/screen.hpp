#ifndef display_screen_hpp
#define display_screen_hpp


#include <vector>
#include <string>

#include "window.hpp"

class screen
{
public:

	screen();
	virtual ~screen(){};
	
	virtual void update();
	virtual void addWindow(std::string name, window* w);
	virtual void removeWindow(std::string name);

	virtual void setVisible(bool v);
	virtual bool isVisible() { return mVisible; }

	virtual void dump();

protected:
	bool mVisible;
	std::vector<window*> mWindows;
};

#endif