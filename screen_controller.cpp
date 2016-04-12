
#include <stdio.h>

#include "screen_controller.hpp"

screen_controller::screen_controller()
{
	mCurrentScreen = mScreens.end();
}

void screen_controller::addScreen(std::string name, screen s)
{
	printf("screen_controller::addScreen(%s)\n", name.c_str() );
	mScreens.insert( std::pair<std::string,screen>(name,s) );
	if ( mCurrentScreen == mScreens.end() )
	{
		printf("first screen\n");
		// if first screen, set currentScreen to this screen
		mCurrentScreen = mScreens.begin();
	}
printf("mCurrentScreen = 0x%0x\n", (int)&mCurrentScreen);
mCurrentScreen->second.dump();
	printf("mScreens.size() = %i\n", mScreens.size());
}

void screen_controller::removeScreen(std::string name)
{
	printf("screen_controller::removeScreen(%s)\n", name.c_str() );
	std::map<std::string,screen>::iterator it;
	it=mScreens.find(name);
	mScreens.erase (it);

	// TODO check if mCurrentScreen is removed
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
			mCurrentScreen = it;
		}	
		else
		{
			it->second.setVisible(false);
		}
	}
	if (!found)
		printf("screen '%s' not found\n", name.c_str() );
}

void screen_controller::showNext()
{
	printf("screen_controller::showNext()\n");

	if ( mCurrentScreen == mScreens.end() )
	{
		printf("mCurrentScreen == mScreens.end()\n");
		mCurrentScreen = mScreens.begin();
	}
	else
	{
		printf("mCurrentScreen++\n");
		mCurrentScreen++;
	}
	printf("showScreen()\n");
printf("mCurrentScreen = 0x%0x\n", (int)&mCurrentScreen);

mCurrentScreen->second.dump();
	mCurrentScreen = mScreens.begin();
//mCurrentScreen->second.setVisible(true);
	//showScreen(mCurrentScreen->first);
}

std::vector<std::string> screen_controller::list()
{

}