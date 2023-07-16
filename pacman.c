#ifndef PACMAN_C_INCLUDED
#define PACMAN_C_INCLUDED

#include "utils.c"

#define PAC_MAN_CHAR "O"
#define PAC_MAN_TRACE_CHAR "░"

typedef struct {
	loc_t location;
	int movement;
	int lives;
} pac_man;

#endif // PACMAN_C_INCLUDED
