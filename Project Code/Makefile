#
# Makefile for the disk emulator
# Author: D.M. Lyons
# Date: Spring 2017
#
# A List of all the source files that should be compiled together
#
SOURCES = diskprocess.cpp directory.cpp driver.cpp file.cpp filesystem.cpp interface.cpp gl_texture_surface.cc application.cc web_tile.cc
#
# A list of the objects that result from the compilation process
#
OBJECTS = diskprocess.o directory.o driver.o file.o filesystem.o interface.o gl_texture_surface.o application.o web_tile.o
#
# A list of include files on which theh compilation depends
#
DEFS = diskprocess.h directory.h file.h filesystem.h interface.h gl_texture_surface.h application.h web_tile.h js_delegate.h method_dispatcher.h
#
# What flags to send to the compiler when you compile source files
#
CPPFLAGS = -g
#
# Any libraries that should be included in the linkage step of compilation
LIBFLAGS = -lm -lawesomium-1-7

# SDL build flags
sdl_flags=`sdl-config --cflags --libs`

demo: $(SOURCES) $(DEFS)
	g++ -std=c++11 $(CPPFAGS) $(SOURCES) -o atos $(LIBFLAGS) $(sdl_flags) -lGLU -lGL
