




/*


g++ display.cpp window.cpp font.c font2.c -o display -lwiringPi && ./display

Based on a user-comment on amazon from Rouven R.

convert -resize 7x13\! -font Garuda -pointsize 10 label:A A.xbm

Font in Gimp schwarz/weiss machen, zb 16x6 Zeichen
Exportieren als png
convert -monochrome 16x16_pixel_font.png font_16x16.xbm

*/

#include <wiringPiI2C.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include "window.hpp"
#include "screen.hpp"


extern long long int font[];
extern char font2[128][8];

void init(int display)
{
  wiringPiI2CWriteReg8(display, 0x00, 0xae); // display off
  wiringPiI2CWriteReg8(display, 0x00, 0xd5); // clockdiv
  wiringPiI2CWriteReg8(display, 0x00, 0x80);
  wiringPiI2CWriteReg8(display, 0x00, 0xa8); // multiplex
  wiringPiI2CWriteReg8(display, 0x00, 0x3f);
  wiringPiI2CWriteReg8(display, 0x00, 0xd3); // offset
  wiringPiI2CWriteReg8(display, 0x00, 0x00);
  wiringPiI2CWriteReg8(display, 0x00, 0x40); // startline
  wiringPiI2CWriteReg8(display, 0x00, 0x8d); // charge pump
  wiringPiI2CWriteReg8(display, 0x00, 0x14);
  wiringPiI2CWriteReg8(display, 0x00, 0x20); // memory mode
  wiringPiI2CWriteReg8(display, 0x00, 0x00);
//wiringPiI2CWriteReg8(display,0x00,0x01);
  wiringPiI2CWriteReg8(display, 0x00, 0xa1); // segregmap
  wiringPiI2CWriteReg8(display, 0x00, 0xc8); // comscandec
  wiringPiI2CWriteReg8(display, 0x00, 0xda); // set com pins
  wiringPiI2CWriteReg8(display, 0x00, 0x12);
  wiringPiI2CWriteReg8(display, 0x00, 0x81); // contrast
  wiringPiI2CWriteReg8(display, 0x00, 0xcf);
  wiringPiI2CWriteReg8(display, 0x00, 0xd9); // precharge
  wiringPiI2CWriteReg8(display, 0x00, 0xf1);
  wiringPiI2CWriteReg8(display, 0x00, 0xdb); // vcom detect
  wiringPiI2CWriteReg8(display, 0x00, 0x40);
  wiringPiI2CWriteReg8(display, 0x00, 0xa4); // resume
  wiringPiI2CWriteReg8(display, 0x00, 0xa6); // normal (not inverted)
  wiringPiI2CWriteReg8(display, 0x00, 0xaf); // display on
}


const char *byte_to_binary(int x)
{
  static char b[9];
  b[0] = '\0';

  int z;
  for (z = 128; z > 0; z >>= 1)
  {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

void render(int display, char *bitmap) {
  char m[8], n[8] = {0};
  memcpy(&m, bitmap, 8);
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
//choose one:
      n[y] |= (m[x] & (1 << (7 - y))) >> (7 - y) << x ; // 270 grad drehen
    }
  }
  for (int x = 7; x >= 0; x--) {
    int a = (int)n[x];
    wiringPiI2CWriteReg8(display, 0x40, a);
  }
}

void reset_pos(int display)
{
  wiringPiI2CWriteReg8(display, 0x00, 0xb0);
  wiringPiI2CWriteReg8(display, 0x00, 0x00);
  wiringPiI2CWriteReg8(display, 0x00, 0x10);
}

void clear(int display)
{
  reset_pos(display);
  for (int x = 0; x < 128; x++) {
    for (int y = 0; y < 8; y++) {
      wiringPiI2CWriteReg8(display, 0x40, 0x00);
    }
  }
}

void clear2(int display)
{
  reset_pos(display);
  for (int x = 0; x < 128; x++) {
    for (int y = 0; y < 8; y++) {
      wiringPiI2CWriteReg8(display, 0x40, 0xff);
    }
  }
}

void printTxt(int display, std::string t)
{
  printf("printTxt(%s) size = %i\n", t.c_str(), t.size() );
  for (int i = 0; i < t.size(); i++ )
  {
    render(display, font2[t[i]]);

  }
}


int main(int argc, char *argv[])
{
  char test[8] = {
    0b00011000,
    0b00111100,
    0b01111110,
    0b11111111,
    0b01100110,
    0b01100110,
    0b01100110,
    0b00000000,
  };
  char test2[8] = {
    0b00111000,
    0b11111111,
    0b01111110,
    0b11111111,
    0b00111100,
    0b00111100,
    0b00111100,
    0b01000010,
  };

  int display = wiringPiI2CSetup(0x3c);
  printf("display = %i\n", display);
  init(display);
  clear(display);
  render(display, test);
  render(display, test2);
  printTxt(display, "               ");
  printTxt(display, "     TEST      ");

/*
  wiringPiI2CWriteReg8(display, 0x00, 0xd3); // offset
  wiringPiI2CWriteReg8(display, 0x00, 0x01);
  usleep(1000000);
  wiringPiI2CWriteReg8(display, 0x00, 0xd3); // offset
  wiringPiI2CWriteReg8(display, 0x00, 0x02);
  usleep(1000000);
  wiringPiI2CWriteReg8(display, 0x00, 0xd3); // offset
  wiringPiI2CWriteReg8(display, 0x00, 0x03);
  usleep(1000000);
  wiringPiI2CWriteReg8(display, 0x00, 0xd3); // offset
  wiringPiI2CWriteReg8(display, 0x00, 0x02);
  usleep(1000000);
  wiringPiI2CWriteReg8(display, 0x00, 0xd3); // offset
  wiringPiI2CWriteReg8(display, 0x00, 0x01);
  usleep(1000000);
  wiringPiI2CWriteReg8(display, 0x00, 0xd3); // offset
  wiringPiI2CWriteReg8(display, 0x00, 0x00);
*/


  window w(4, 2, 6, 4);
  std::vector<char> content;

  int mychars[] = {'A', 'B', 'C'};
  content.assign (mychars,mychars+3);   // assigning from array.

  w.setContent(content);


  screen s;
  s.addWindow("days", w);
  s.update();


    printTxt(display, "THOMAS          ");

  /*
      for (int z = 0; z < 70; z++)
      {
                    render(display, font2[(z%10)+48]);
        sleep(1);
      }
  */

/*
  for (int z = 0; z < test_height; z++)
  {
    if ( z % 2 == 0 )
      printf("\n");
    printf("%s", byte_to_binary(test_bits[z]));

  }
  render(display, test_bits);
*/
}

