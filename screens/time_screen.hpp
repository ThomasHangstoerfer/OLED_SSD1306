#ifndef time_screen_hpp
#define time_screen_hpp

#include <mutex>

#include "screen.hpp"
#include "window.hpp"

class time_screen : public screen
{
public:
	
	time_screen();
	~time_screen(){}
	
	void updateTime();


	void setVisible(bool v) override;

	std::mutex mMutex;
private:
	std::string getJetzt();
	std::string string_format(const std::string fmt, ...);

	window wLabel;
	window wValue;

};

#endif // time_screen_hpp