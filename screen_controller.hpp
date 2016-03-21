#ifndef screen_controller_hpp
#define screen_controller_hpp

#include <vector>
#include <map>
#include <string>

#include "screen.hpp"


class screen_controller
{
public:
	screen_controller();
	void addScreen(std::string name, screen s);
	void removeScreen(std::string name);
	void showScreen(std::string name);

	std::vector<std::string> list();

private:
	std::map<std::string, screen> mScreens;
};

#endif //screen_controller_hpp
