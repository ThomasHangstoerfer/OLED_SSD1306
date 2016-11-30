#ifndef graph_screen_hpp
#define graph_screen_hpp

#include <mutex>

#include "screen.hpp"
#include "text_window.hpp"

class graph_screen : public screen
{
public:
	
	graph_screen();
	~graph_screen(){}
	
	void updateScreen(); 
	
	void setVisible(bool v) override;
	void up();
	void down();
	void left();
	void right();

	std::mutex mMutex;
private:

	int cursorX, cursorY;
};

#endif // graph_screen_hpp