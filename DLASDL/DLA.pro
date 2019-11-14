TARGET=DLA

CONFIG -=qt
CONFIG+=c++14
cache()
CONFIG-=app_bundle
DESTDIR=./
SOURCES=DLA.cpp
HEADERS=RGBA.h
QMAKE_CXXFLAGS+=-fopenmp
QMAKE_LFLAGS+= -fopenmp -lpthread

QMAKE_CXXFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

LIBS+=$$system(sdl2-config  --libs)
message(output from sdl2-config --libs added to LIB=$$LIBS)
