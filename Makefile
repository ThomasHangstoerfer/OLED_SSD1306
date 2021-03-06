#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = g++

# define any compile-time flags
#CFLAGS = -Wall -g -DNO_WIRING_PI
CFLAGS = -g -pthread -std=c++11 -DNO_WIRING_PI -Wno-narrowing

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -lwiringPi
LIBS = -lm

# define any directories containing header files other than /usr/include
#
INCLUDES = -I.

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#LFLAGS = -L/home/newhall/lib  -L../lib

# define the C source files
SRCS =  display.cpp \
	window.cpp \
	text_window.cpp \
	screens/screen.cpp \
	font.cpp \
	font_old.c \
	font2.c \
	font_16x16.cpp \
	screen_controller.cpp \
	screens/load_screen.cpp \
	screens/time_screen.cpp \
	screens/graph_screen.cpp \
	screens/wifi_screen.cpp
HDRS = window.hpp text_window.hpp screen.hpp screen_controller.hpp font.hpp font_16x16.hpp load_screen.hpp time_screen.hpp wifi_screen.hpp graph_screen.hpp

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c$=.o)
OBJS += $(SRCS:.cpp=.o)

# define the executable file 
MAIN = display

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
#		@echo  Simple compiler named mycc has been compiled

$(MAIN): $(OBJS) 
		$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@
.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN) screens/*.o

depend: $(SRCS) $(HDRS)
		makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
