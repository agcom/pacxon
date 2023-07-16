#ifndef GHOSTS_C_INCLUDED
#define GHOSTS_C_INCLUDED

#include "utils.c"

// Ghosts type coding
enum {
	FLOAT_GHOST, RAIL_GHOST, BRICK_ZONE_GHOST
};

// Shapes
#define FLOAT_GHOST_CHAR "α"
#define RAIL_GHOST_CHAR "ß"
#define BRICK_ZONE_GHOST_CHAR "δ"

// Locations needed sizes
#define FLOAT_GHOST_LOCATIONS_SIZE 1
#define BRICK_ZONE_GHOST_LOCATIONS_SIZE 1
#define RAIL_GHOST_LOCATIONS_SIZE 2

#define MAX_GHOST_LOCATIONS_SIZE 2

typedef struct {
	int type;
	int locations_size;
	Location locations[MAX_GHOST_LOCATIONS_SIZE];
	int movement;
} Ghost;

#endif // GHOSTS_C_INCLUDED