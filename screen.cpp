

#include <stdio.h>
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

	printf("screen::update()\n");
	for (int i = 0; i < mWindows.size(); i++)
	{
		mWindows[i].update();
	}
}

void screen::addWindow(std::string name, window w)
{
	mWindows.push_back(w);
}

void screen::removeWindow(std::string name)
{
}

void screen::dump()
{
	//printf("+++++++++++++++\n");
	//printf("screen::dump()\n");
	for (int i = 0; i < mWindows.size(); i++)
	{
		printf("%10swindow[%i]: ", "", i);
		mWindows[i].dump();
	}
}

