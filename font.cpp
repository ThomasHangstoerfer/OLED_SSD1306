
#include "font.hpp"
#include "font_16x16.hpp"

font::font()
{

}

font::~font()
{

}

font* font::createFont(FontId fontId)
{
    return new font_16x16();
    
}



