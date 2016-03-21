
#include <stdio.h>

#include "screen_controller.hpp"

screen_controller::screen_controller()
{

}

void screen_controller::addScreen(std::string name, screen s)
{
	printf("screen_controller::addScreen(%s)\n", name.c_str() );
	mScreens.insert( std::pair<std::string,screen>(name,s) );
}

void screen_controller::removeScreen(std::string name)
{
	printf("screen_controller::removeScreen(%s)\n", name.c_str() );
	std::map<std::string,screen>::iterator it;
	it=mScreens.find(name);
	mScreens.erase (it);
}

void screen_controller::showScreen(std::string name)
{
	printf("screen_controller::showScreen(%s)\n", name.c_str() );
	std::map<std::string,screen>::iterator it;
	bool found = false;
	for (it=mScreens.begin(); it!=mScreens.end(); ++it)
	{
		if ( it->first == name )
		{
			it->second.setVisible(true);
			found = true;
		}	
		else
		{
			it->second.setVisible(false);
		}
	}
	if (!found)
		printf("screen '%s' not found\n", name.c_str() );
}

std::vector<std::string> screen_controller::list()
{

}