#ifndef KBHIT_C_INCLUDED
#define KBHIT_C_INCLUDED

#include <curses.h>

int _kbhit() {
	const int c = getch();
	if (c != ERR) {
		ungetch(c);
		return 1;
	} else
		return 0;
}

#endif // KBHIT_C_INCLUDED