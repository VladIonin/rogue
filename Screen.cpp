#include "Screen.h"

Screen::Screen(){
	initscr();
	resize_term(20,60);
	clear();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);
	getmaxyx(stdscr, _height, _width);
}

Screen::~Screen(){
	endwin();
}

void Screen::add(const char *message){
	printw(message);
}

int Screen::height(){
	return _height;
}

int Screen::width(){
	return _width;
}
