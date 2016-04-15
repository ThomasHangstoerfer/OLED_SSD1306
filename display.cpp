




/*


g++ display.cpp window.cpp font.c font2.c -o display -lwiringPi && ./display

Based on a user-comment on amazon from Rouven R.

convert -resize 7x13\! -font Garuda -pointsize 10 label:A A.xbm

Font in Gimp schwarz/weiss machen, zb 16x6 Zeichen
Exportieren als png
convert -monochrome 16x16_pixel_font.png font_16x16.xbm

*/
#ifndef NO_WIRING_PI
#include <wiringPi.h>
#include <wiringPiI2C.h>
#endif

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "window.hpp"
#include "screen.hpp"
#include "load_screen.hpp"
#include "time_screen.hpp"
#include "screen_controller.hpp"

extern long long int font[];
extern char font2[128][8];

const int butPin = 23; // Active-low button - Broadcom pin 17, P1 pin 11

  screen_controller sc;

void button_pressed()
{
  printf("button_pressed\n");
    sc.showNext();

}

void init(int display)
{


#ifndef NO_WIRING_PI

  wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers
  pinMode(butPin, INPUT);      // Set button as INPUT
  pullUpDnControl(butPin, PUD_DOWN); // Enable pull-up resistor on button


  //int wiringPiISR (int pin, int edgeType,  void (*function)(void)) ;
  wiringPiISR(butPin, INT_EDGE_RISING, button_pressed);

//  sleep(10);
  //while(1)
  //{
  //  if (digitalRead(butPin)) // Button is released if this returns 1
  //  {
  //    print("rel");
  //  }
  //  else // If digitalRead returns 0, button is pressed
  //  {
  //  }
  //}

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

#endif
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
#ifndef NO_WIRING_PI
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
#endif
}

void reset_pos(int display)
{
#ifndef NO_WIRING_PI
  wiringPiI2CWriteReg8(display, 0x00, 0xb0);
  wiringPiI2CWriteReg8(display, 0x00, 0x00);
  wiringPiI2CWriteReg8(display, 0x00, 0x10);
#endif
}

void clear(int display)
{
#ifndef NO_WIRING_PI
  reset_pos(display);
  for (int x = 0; x < 128; x++) {
    for (int y = 0; y < 8; y++) {
      wiringPiI2CWriteReg8(display, 0x40, 0x00);
    }
  }
#endif
}

void clear2(int display)
{
#ifndef NO_WIRING_PI
  reset_pos(display);
  for (int x = 0; x < 128; x++) {
    for (int y = 0; y < 8; y++) {
      wiringPiI2CWriteReg8(display, 0x40, 0xff);
    }
  }
#endif
}

void printTxt(int display, std::string t)
{
  printf("printTxt(%s) size = %lu\n", t.c_str(), t.size() );
  for (unsigned int i = 0; i < t.size(); i++ )
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

#ifndef NO_WIRING_PI
  int display = wiringPiI2CSetup(0x3c);
#else
  int display = 0;
#endif
  printf("display = %i\n", display);
  init(display);
  clear(display);

  //render(display, test);
  //render(display, test2);
  //printTxt(display, "              ");
  //printTxt(display, "                ");
  //printTxt(display, "H A L L O       ");
  //printTxt(display, "                ");
  //printTxt(display, "Gefaellt Dir    ");
  //printTxt(display, "mein Hut?       ");
  //return 1;

//  screen_controller sc;

  screen s1;
  //s1.setVisible(true);
  window s1w1(2, 1, "28°C");
  window s1w2(0, 4, (std::string)"Thomas");
  s1.addWindow("tempval", s1w1);
  s1.addWindow("label", s1w2);
  sc.addScreen("temperature", &s1);

  screen s2;
  window s2w1(6, 5, "14%");
  window s2w2(2, 3, (std::string)"Load");
  s2.addWindow("loadval", s2w1);
  s2.addWindow("loadlabel", s2w2);
  sc.addScreen("load", &s2);

  screen s3;
  window s3w1(1, 5, "Philipp");
  window s3w2(2, 3, (std::string)"Hallo");
  s3.addWindow("loadval", s3w1);
  s3.addWindow("loadlabel", s3w2);
  sc.addScreen("fips", &s3);

  sc.dump();
  printf("setup finished\n\n\n");

  load_screen ls;
  sc.addScreen("loadscreen", &ls);

  time_screen ts;
  sc.addScreen("time", &ts);

  //printf("showScreen(temperature)\n");
    sc.showScreen("temperature");


  // TODO clock-screen

//while (true)
//{
//  sleep(1);
//}

  char in;
  //scanf("%c", &in);

  int mode = 0;
  while(scanf("%c", &in))
  {
    printf("sc.showNext()\n");
    sc.showNext();
    
    /*
      if (mode == 0)
      {
        printf("showScreen(load)\n");
        clear(display);
        sc.showScreen("load");
        mode = 1;
      }
      else if (mode == 1)
      {
        printf("showScreen(temperature)\n");
        clear(display);
        sc.showScreen("temperature");
        mode = 2;
      }
      else
      {
        printf("showScreen(fips)\n");
        clear(display);
        sc.showScreen("fips");
        mode = 0;
      }
      */
  }


}

