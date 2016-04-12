
#include <string>

#include "window.hpp"
#include "load_screen.hpp"

load_screen::load_screen() : screen()
{
	window wLabel(1, 1, "Load");
	window wValue(1, 3, "14%");
	addWindow("label", wLabel);
	addWindow("value", wValue);
}