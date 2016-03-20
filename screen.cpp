

#include <iostream>

#include "screen.hpp"

screen::screen()
	: mVisible(false)
{
}

void screen::setVisible(bool v)
{
	mVisible = v;
	if ( mVisible )
		update();

}

void screen::update()
{
	if ( !mVisible )
		return;

	std::map<std::string,window>::iterator it = mWindows.begin();
  
	for (it=mWindows.begin(); it!=mWindows.end(); ++it)
	{
		//std::cout << it->first << " => " << it->second << '\n';		
		std::cout << "Updating window " << it->first  << '\n';
		it->second.dump();
		it->second.update();
	}
}

void screen::addWindow(std::string name, window w)
{
	mWindows.insert( std::pair<std::string,window>(name,w) );
}
