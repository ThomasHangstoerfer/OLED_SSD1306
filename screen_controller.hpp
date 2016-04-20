#ifndef screen_controller_hpp
#define screen_controller_hpp

#include <vector>
#include <map>
#include <string>

#include "screens/screen.hpp"


class screen_controller
{
public:
	screen_controller();
	void addScreen(std::string name, screen* s);
	void removeScreen(std::string name);
	void showScreen(std::string name);
	void showNext();
	void update();

	void dump();

	std::vector<std::string> list();

private:
	std::map<std::string, screen*> mScreens;
	std::map<std::string, screen*>::iterator mCurrentScreen;
};

#endif //screen_controller_hpp
