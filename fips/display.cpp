


/*


g++ display.cpp font.c font2.c -o display -lwiringPi && ./display

Based on a user-comment on amazon from Rouven R.


*/

#include <wiringPiI2C.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr

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



std::string string_format(const std::string fmt, ...) {
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}

std::string getJetzt()
{
  time_t jetzt;
  struct tm j;
  double seconds;

  time(&jetzt);
  j = *localtime(&jetzt);
  return string_format("%02i.%02i.%i %02i:%02i", j.tm_mday, j.tm_mon, j.tm_year+1900, j.tm_hour, j.tm_min);
}

int tage_bis_Ostern()
{


  time_t jetzt;
  struct tm ostern;
  double seconds;

  time(&jetzt);

  ostern = *localtime(&jetzt);

  ostern.tm_hour = 0;
  ostern.tm_min = 0;
  ostern.tm_sec = 0;
  ostern.tm_mday = 27;
  ostern.tm_mon = 3 - 1;
  ostern.tm_year = 2016 - 1900;

  time_t o  = mktime(&ostern);

  seconds = o - jetzt;
  printf("%s", asctime (&ostern) );

  printf("heute %i\n", jetzt);
  printf("ostern %i\n", o);
  float tage = ((float)((o - jetzt)/3600))/24;
  printf("bis ostern sinds %i tage\n", (int)tage);

  return (int)tage;
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

        init(display);
        


        while ( tage_bis_Ostern() > 0 )
        {
          clear(display);

          printTxt(display, "Es sind noch    ");
          printTxt(display, "                " );

          std::ostringstream s;
          s << "       " << tage_bis_Ostern() << "        ";
          const std::string tageString(s.str());

          printTxt(display, tageString );

          printTxt(display, "                " );
          printTxt(display, "Tage bis Ostern!");

          printTxt(display, "                " );
          printTxt(display, "                " );

          printTxt(display, getJetzt() );

          //sleep(60*60);
          sleep(60);
        }



        clear(display);

        printTxt(display, "                " );
        printTxt(display, "                " );
        printTxt(display, "                " );

        printTxt(display, "Heute ist Ostern");

        printTxt(display, "                " );
        printTxt(display, "                " );
        printTxt(display, "                " );
}


/*
1.1.1970 - 00:00:00

1458330886

24 h

1458330886 / 86400

1 Tag hat 86400 Sekunden (60*60*24)

*/
