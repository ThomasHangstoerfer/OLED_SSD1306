
#include <string>

#include "window.hpp"
#include "load_screen.hpp"

load_screen::load_screen() : screen()
	, wLabel(1, 1, "CPULoad")
	, wValue(1, 3, "47%")
{
	addWindow("label", &wLabel);
	addWindow("value", &wValue);
}