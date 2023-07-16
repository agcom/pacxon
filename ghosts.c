#ifndef GHOSTS_C_INCLUDED
#define GHOSTS_C_INCLUDED

#include "utils.c"

//ghosts type coding
#define FLOAT_GHOST 0
#define RAIL_GHOST 1
#define BRICK_ZONE_GHOST 2

//shapes
#define FLOAT_GHOST_CHAR "α"
#define RAIL_GHOST_CHAR "ß"
#define BRICK_ZONE_GHOST_CHAR "δ"

#define A_GHOST_MAX_LOCATIONS 10

#define FLOAT_GHOST_LOCATIONSS 1
#define BRICK_ZONE_GHOST_LOCATIONSS 1
#define RAIL_GHOST_LOCATIONSS 2

//a ghost
typedef struct {
	
	int type;
	int locationss;
	Location locations[A_GHOST_MAX_LOCATIONS];
	int movement;
	
} Ghost;

#endif // GHOSTS_C_INCLUDED