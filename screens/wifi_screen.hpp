#ifndef wifi_screen_hpp
#define wifi_screen_hpp

#include <mutex>
#include <vector>
#include <string>

#include "screen.hpp"

class wifi_screen : public screen
{
public:
	
	wifi_screen();
	~wifi_screen(){}
	
	void updateWifiStatus();


	void setVisible(bool v) override;

	std::mutex mMutex;

	static wifi_screen* instance;
private:

	std::vector<std::string> split(const char *str, char c = ' ');

	window wLabel;
	window wRateValue;
	window wQualValue;

};

#endif // wifi_screen_hpp