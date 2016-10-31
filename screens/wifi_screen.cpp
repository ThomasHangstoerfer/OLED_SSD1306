#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>  // For va_start, etc.

#include "window.hpp"
#include "font.hpp"
#include "wifi_screen.hpp"


wifi_screen* wifi_screen::instance = NULL;


void wifi_update_task(unsigned int wait)
{
	while (true)
	{
		wifi_screen::instance->mMutex.lock();
		wifi_screen::instance->updateWifiStatus();
		wifi_screen::instance->mMutex.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(wait));
	}
}

wifi_screen::wifi_screen() : screen()
	, wLabel(1, 1, "Wifi")
	, wRateValue(0, 3, "                ")
	, wQualValue(0, 5, "                ")
{
	wRateValue.setFontId(font::FONT_8x8);
	wQualValue.setFontId(font::FONT_8x8);
	addWindow("label", &wLabel);
	addWindow("rate", &wRateValue);
	addWindow("quality", &wQualValue);
	
	wifi_screen::instance = this;
}

void wifi_screen::update()
{
	wRateValue.update();
	wQualValue.update();
}

std::vector<std::string> wifi_screen::split(const char *str, char c /*= ' '*/)
{
    std::vector<std::string> result;

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        std::string s = std::string(begin, str);
		s.erase(s.find_last_not_of(" \n\r\t")+1);

        result.push_back(s);
    } while (0 != *str++);

    return result;
}


#define COMMAND_LEN 8
#define DATA_SIZE 512

void wifi_screen::updateWifiStatus()
{
	if ( !mVisible )
		return;

	printf("wifi_screen::updateWifiStatus()\n");


	//std::string command = "iwconfig wlp10s0|grep -e \"Bit Rate\" -e \"Quality\" |tr '\n' ' '|sed 's/ \\+/ /g'|cut -d' ' -f 3,4,8";
	std::string command = "iwconfig wlan0|grep -e \"Bit Rate\" -e \"Quality\" |tr '\n' ' '|sed 's/ \\+/ /g'|cut -d' ' -f 3,4,7";
	printf("command: %s\n", command.c_str() );
	char data[DATA_SIZE];

	FILE* pf = popen(command.c_str(),"r");

	if(!pf){
		fprintf(stderr, "Could not open pipe for output.\n");
		return;
	}

	fgets(data, DATA_SIZE , pf);

	pclose(pf);

	std::vector<std::string> sv = split(data);
	for (int i=0; i < sv.size(); i++)
	{
		printf("[%i] %s\n", i, sv[i].c_str() );
	}
	if ( sv.size() > 1 )
	{
		wRateValue.setContent(sv[0] + sv[1]);
	}
	if ( sv.size() > 2 )
	{
		wQualValue.setContent(sv[2]);
	}
	wRateValue.update();
	wQualValue.update();
}


void wifi_screen::setVisible(bool v)
{
	static std::thread* t1 = NULL;
	if ( v )
	{
		mMutex.unlock();
		printf("+++ wifi_screen::setVisible(%s)\n", v?"true":"false");
		if ( t1 == NULL )
			t1 = new std::thread(wifi_update_task, 1);
	}
	else
	{
		mMutex.lock();
	}
	screen::setVisible(v);
}



