#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>  // For va_start, etc.

#include "window.hpp"
#include "time_screen.hpp"


time_screen* instance = NULL;
void update_task(unsigned int wait)
{
	while (true)
	{
		instance->mMutex.lock();
		instance->updateTime();
		instance->mMutex.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(wait));
	}
}

time_screen::time_screen() : screen()
	, wLabel(1, 1, "TIME")
	, wValue(1, 3, getJetzt())
{
	addWindow("label", &wLabel);
	addWindow("value", &wValue);
	
	instance = this;
}

void time_screen::updateTime()
{
	if ( !mVisible )
		return;

	printf("time_screen::updateTime()\n");

	mWindows[1]->setContent(getJetzt());
	mWindows[1]->update();
}


void time_screen::setVisible(bool v)
{
	static std::thread* t1 = NULL;
	if ( v )
	{
		mMutex.unlock();
		printf("+++ time_screen::setVisible(%s)\n", v?"true":"false");
		if ( t1 == NULL )
			t1 = new std::thread(update_task, 1);
	}
	else
	{
		mMutex.lock();
	}
	screen::setVisible(v);
}



std::string time_screen::string_format(const std::string fmt, ...) {
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}

std::string time_screen::getJetzt()
{
  time_t jetzt;
  struct tm j;
  double seconds;

  time(&jetzt);
  j = *localtime(&jetzt);
  //return string_format("%02i.%02i.%i %02i:%02i", j.tm_mday, j.tm_mon + 1, j.tm_year+1900, j.tm_hour, j.tm_min);
  return string_format("%02i:%02i", j.tm_hour, j.tm_min);
}

