#include <pdcurses.h>

#ifndef SCREEN_H
#define SCREEN_H

class Screen{
	int _height, _width;
public:
	Screen();

	~Screen();

	void add(const char *message);

	int height();
	int width();
};

#endif
