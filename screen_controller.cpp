
#include <stdio.h>

#include "screen_controller.hpp"


extern void clear(int d);
extern int display;

screen_controller::screen_controller()
{
	mCurrentScreen = mScreens.end();
}

void screen_controller::addScreen(std::string name, screen* s)
{
	printf("screen_controller::addScreen(%s)\n", name.c_str() );
	mScreens.insert( std::pair<std::string, screen*>(name, s) );
	if ( mCurrentScreen == mScreens.end() )
	{
		printf("first screen\n");
		// if first screen, set currentScreen to this screen
		mCurrentScreen = mScreens.begin();
	}
}

void screen_controller::removeScreen(std::string name)
{
	printf("screen_controller::removeScreen(%s)\n", name.c_str() );
	std::map<std::string, screen*>::iterator it;
	it = mScreens.find(name);
	mScreens.erase (it);

	// TODO check if mCurrentScreen is removed
}

void screen_controller::showScreen(std::string name)
{
	printf("screen_controller::showScreen(%s)\n", name.c_str() );
	std::map<std::string, screen*>::iterator it;
	bool found = false;
	for (it = mScreens.begin(); it != mScreens.end(); ++it)
	{
		if ( it->first == name )
		{
//			it->second->setVisible(true);
			found = true;
			mCurrentScreen = it;
//mCurrentScreen->second->update();
		}
		else
		{
			if ( it->second->isVisible() )
				it->second->setVisible(false);
		}
	}
	clear(display);
	mCurrentScreen->second->setVisible(true);
	mCurrentScreen->second->dump();

	if (!found)
		printf("screen '%s' not found\n", name.c_str() );
}

void screen_controller::update()
{
	printf("screen_controller::update()\n");
	mCurrentScreen->second->update();
}

void screen_controller::showNext()
{
	printf("screen_controller::showNext()\n");

	//printf("mCurrentScreen++\n");
	if ( mCurrentScreen != mScreens.end() )
		mCurrentScreen++;

	if ( mCurrentScreen == mScreens.end() )
	{
		//printf("mCurrentScreen == mScreens.end()\n");
		mCurrentScreen = mScreens.begin();
	}
	clear(display);
	if ( mCurrentScreen->first == "empty" )
		showNext();
	else
		showScreen(mCurrentScreen->first);
}

void screen_controller::showPrev()
{
	printf("screen_controller::showPrev()\n");

	//printf("mCurrentScreen++\n");
	if ( mCurrentScreen == mScreens.begin() )
		mCurrentScreen = mScreens.end();
	mCurrentScreen--;

	clear(display);
	if ( mCurrentScreen->first == "empty" )
		showPrev();
	else
		showScreen(mCurrentScreen->first);
}

void screen_controller::dump()
{
	printf("\n\nscreen_controller::dump()\n");
	std::map<std::string, screen*>::iterator it;
	bool found = false;
	for (it = mScreens.begin(); it != mScreens.end(); ++it)
	{
		printf("%12s: \n", it->first.c_str() );
		it->second->dump();
	}
}

std::vector<std::string> screen_controller::list()
{

}
