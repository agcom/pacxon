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
#define FLOAT_GHOST_LOCS_SIZE 1
#define BRICK_ZONE_GHOST_LOCS_SIZE 1
#define RAIL_GHOST_LOCS_SIZE 2

#define MAX_GHOST_LOCS_SIZE 2

typedef struct {
	int type;
	int locs_size;
	loc_t locs[MAX_GHOST_LOCS_SIZE];
	int movement;
} ghost_t;

#endif // GHOSTS_C_INCLUDED