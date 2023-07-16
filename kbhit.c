#ifndef KBHIT_C_INCLUDED
#define KBHIT_C_INCLUDED

#include <curses.h>

int _kbhit() {
	const char c = getch();
	ungetch(c);
	return c != ERR;
}

#endif // KBHIT_C_INCLUDED