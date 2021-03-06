




/*


g++ display.cpp window.cpp font.c font2.c -o display -lwiringPi && ./display

Based on a user-comment on amazon from Rouven R.

convert -resize 7x13\! -font Garuda -pointsize 10 label:A A.xbm

Font in Gimp schwarz/weiss machen, zb 16x6 Zeichen
Exportieren als png
convert -monochrome 16x16_pixel_font.png font_16x16.xbm



TODO: dont poll for rotary-knob-event but use interrupts



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
#include <signal.h>

#include "window.hpp"
#include "text_window.hpp"
#include "screens/screen.hpp"
#include "screens/load_screen.hpp"
#include "screens/time_screen.hpp"
#include "screens/wifi_screen.hpp"
#include "screens/graph_screen.hpp"
#include "screen_controller.hpp"


#define  DDS_PIN1   17 // 0
#define  DDS_PIN2   18 // 1
#define  DDS_PRESS  23 //  4

extern long long int font[];
extern char font2[128][8];

const int butPin = 23; // Active-low button - Broadcom pin 17, P1 pin 11
const int ddsPressPin = 18; // Active-low button

screen_controller sc;
int display = 0;

void renderInv(int display, char *bitmap, bool inverted );
void render(int display, char *bitmap );
void clear(int display);


void button_pressed()
{
  printf("button_pressed\n");
  sc.showNext();

}

void ddsbutton_pressed()
{
  printf("ddsbutton_pressed\n");

}

void ddsbutton_released()
{
  printf("ddsbutton_released\n");

  sc.showNext();
}

void ddsbutton_event()
{
  bool pressed = !digitalRead(DDS_PRESS);
  printf("ddsbutton_event pressed = %i\n", pressed);
  static int last = !pressed;

  if ( last == pressed )
  {
    //printf("no change\n");
  }

  // TODO decouple ddsbutton-events from this ISR

  if ( last == 0 && pressed )
    ddsbutton_pressed();
  if ( last == 1 && !pressed )
    ddsbutton_released();

  last = pressed;
}

void init(int display)
{


#ifndef NO_WIRING_PI

  wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers
  /*
  pinMode(butPin, INPUT);      // Set button as INPUT
  pullUpDnControl(butPin, PUD_DOWN); // Enable pull-up resistor on button

  pinMode(ddsPressPin, INPUT);      // Set button as INPUT
  pullUpDnControl(ddsPressPin, PUD_DOWN); // Enable pull-up resistor on button


  //int wiringPiISR (int pin, int edgeType,  void (*function)(void)) ;
  wiringPiISR(butPin, INT_EDGE_RISING, button_pressed);
  wiringPiISR(ddsPressPin, INT_EDGE_RISING, ddsbutton_pressed);
  */

  pinMode(DDS_PIN1, INPUT);
  pinMode(DDS_PIN2, INPUT);
  pinMode(DDS_PRESS, INPUT);

  pullUpDnControl(DDS_PRESS, PUD_UP);

  wiringPiISR(DDS_PRESS, INT_EDGE_BOTH, ddsbutton_event);



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

void render(int display, char *bitmap)
{
  renderInv(display, bitmap, false);
}

void renderInv(int display, char *bitmap, bool inverted )
{
  //printf("render(inverted = %i)\n", inverted);
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
    wiringPiI2CWriteReg8(display, 0x40, inverted ? ~a : a);
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
  //printf("clear(%i)\n", display);
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
  //printf("clear2(%i)\n", display);
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
  //printf("printTxt(%s) size = %lu\n", t.c_str(), t.size() );
  for (unsigned int i = 0; i < t.size(); i++ )
  {
    render(display, font2[t[i]]);

  }
}

void  INThandler(int sig)
{
  printf("QUIT\n");
  sc.showScreen("empty");
  clear(display);
  exit(0);
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
  display = wiringPiI2CSetup(0x3c);
#else
  printf("\n\n************\nNO_WIRING_PI\n************\n\n");
  display = 0;
#endif
  printf("display = %i\n", display);
  init(display);
  clear(display);

  signal(SIGINT, INThandler);

  //render(display, test);
  //render(display, test2);
  //printTxt(display, "              ");
  //printTxt(display, "                ");
  //printTxt(display, "H A L L O       ");
  //printTxt(display, "                ");
  //printTxt(display, "Gefaellt Dir    ");
  //printTxt(display, "mein Hut?       ");
  //return 1;
//  printTxt(display, "0123456789ABCDEF");
//  return 1;

//  screen_controller sc;



  screen s1;
  //s1.setVisible(true);
  text_window s1w1(2, 4, "21°C");
  s1w1.setFontId(font::FONT_8x8);
  text_window s1w2(0, 1, (std::string)" Thomas ");
  s1w2.setInverted(true);
  s1.addWindow("tempval", &s1w1);
  s1.addWindow("label", &s1w2);
  sc.addScreen("temperature", &s1);

  screen s2;
  text_window s2w1(6, 5, "14%");
  text_window s2w2(2, 3, (std::string)"Load");
  s2.addWindow("loadval", &s2w1);
  s2.addWindow("loadlabel", &s2w2);
  sc.addScreen("load", &s2);

  screen s3;
  text_window s3w1(1, 5, "Philipp");
  text_window s3w2(2, 3, (std::string)"Hallo");
  s3w2.setFontId(font::FONT_8x8);
  s3.addWindow("loadval", &s3w1);
  s3.addWindow("loadlabel", &s3w2);
  sc.addScreen("fips", &s3);

  sc.dump();
  printf("setup finished\n\n\n");

  load_screen ls;
  sc.addScreen("loadscreen", &ls);

  time_screen ts;
  sc.addScreen("time", &ts);

  wifi_screen ws;
  sc.addScreen("wifi", &ws);

  graph_screen gs;
  sc.addScreen("graph", &gs);

  screen empty_screen; // used to clear the screen on exit
  sc.addScreen("empty", &empty_screen); // fixed name, screen_controller checks it!

  //sc.showScreen("temperature");
  //sc.showScreen("time");
  sc.showScreen("graph");

  char in;

  int mode = 0;

  static volatile int globalCounter = 0 ;
  unsigned char flag;
  unsigned char Last_RoB_Status;
  unsigned char Current_RoB_Status;

  int pressed = 0;
  int old_pressed = 0;

  while (1) {
    break;
    printf("digitalRead(DDS_PIN1) = %i digitalRead(DDS_PIN2) = %i old_pressed = %i\n", digitalRead(DDS_PIN1), digitalRead(DDS_PIN2), old_pressed);
    Last_RoB_Status = digitalRead(DDS_PIN2);

    while (!digitalRead(DDS_PIN1)) {
      Current_RoB_Status = digitalRead(DDS_PIN2);
      flag = 1;
    }

    if (flag == 1) {
      printf("flag = 1;\n");
      flag = 0;
      if ((Last_RoB_Status == 0) && (Current_RoB_Status == 1)) {
        globalCounter ++;
        printf("right\n");
        sc.showNext();
      }
      if ((Last_RoB_Status == 1) && (Current_RoB_Status == 0)) {
        globalCounter --;
        printf("left\n");
        sc.showPrev();
      }
    }
    pressed = !digitalRead(DDS_PRESS);
    if ( pressed == 1 && old_pressed == 0 )
      printf("Pressed\n");
    if ( pressed == 0 && old_pressed == 1 )
      printf("Released\n");
    if ( pressed != old_pressed )
      old_pressed = pressed;

//    usleep(100000);
    //printf("globalCounter : %d\n",globalCounter);
  }

  while (scanf("%c", &in))
  {
    printf("command: %c\n", in);

    switch (in)
    {
    case 'C':
      clear2(display);
      break;
    case 'c':
      clear(display);
      break;
    case 'n':
      sc.showNext();
      break;
    case 'u':
      sc.update();
      break;
    case 's':
      sc.dump();
      break;
    case '8':
      gs.up();
      break;
    case '2':
      gs.down();
      break;
    case '4':
      gs.left();
      break;
    case '6':
      gs.right();
      break;
    case 'q':
      printf("clear screen and quit\n");
      clear(display);
      exit(0);
      break;
    case '?':
      printf("u - update\n");
      printf("c - clear\n");
      printf("C - clear inverted\n");
      printf("n - next screen\n");
      printf("s - show all screens\n");
      printf("q - clear screen and quit\n");
      break;
    default:
      break;
    }
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

