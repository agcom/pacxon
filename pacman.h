#ifndef PACMAN_H_INCLUDED
#define PACMAN_H_INCLUDED

#include "utils.h"

#define PAC_MAN_CHAR 'O'
#define PAC_MAN_TRACE_CHAR 176

typedef struct {

    Location location;
    int movement;
    int lives;

} PacMan;

#endif // PACMAN_H_INCLUDED
