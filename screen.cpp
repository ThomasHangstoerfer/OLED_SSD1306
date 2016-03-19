

#include <iostream>

#include "screen.hpp"

screen::screen()
{

}

void screen::update()
{
	std::map<std::string,window>::iterator it = mWindows.begin();
  
	for (it=mWindows.begin(); it!=mWindows.end(); ++it)
	{
		//std::cout << it->first << " => " << it->second << '\n';		
		std::cout << "Updating window " << it->first  << '\n';
		it->second.update();
	}
}

void screen::addWindow(std::string name, window w)
{
	mWindows.insert( std::pair<std::string,window>(name,w) );
}
