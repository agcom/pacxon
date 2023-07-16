#ifndef DIR_C_INCLUDED
#define DIR_C_INCLUDED

#include <stdbool.h>
#include "utils.c"

// Directions' masks
#define VERTICAL 0b0001
#define HORIZONTAL 0b0010
#define LEFT 0b0100
#define UP 0b1000

typedef int dir_t;

// Clock-wise order
const dir_t all_directions[8] = {
		VERTICAL | UP, // UP
		HORIZONTAL | VERTICAL | UP | !LEFT, // Up Right
		HORIZONTAL | !LEFT, // Right
		HORIZONTAL | VERTICAL | !UP | !LEFT, // Down Right
		VERTICAL | !UP, // Down
		HORIZONTAL | VERTICAL | !UP | LEFT, // Down Left
		HORIZONTAL | LEFT, // Left
		HORIZONTAL | VERTICAL | UP | LEFT  // Up Left
};

void print_dir(const dir_t dir) {
	bool is_vertical_printed = false;
	
	if (dir & VERTICAL) {
		if (dir & UP) printf("Up");
		else printf("Down");
		
		is_vertical_printed = true;
	}
	
	if (dir & HORIZONTAL) {
		if (is_vertical_printed) printf(" ");
		
		if (dir & LEFT) printf("Left");
		else printf("Right");
	}
}

void print_dir_arr(const int size, const dir_t dirs[size]) {
	int i;
	for (i = 0; i < size; i++) {
		print_dir(dirs[i]);
		if (i != size - 1) printf(", ");
	}
}

int turn_dir(dir_t dir, const bool clockwise, const int steps) {
	
	// Find its index at directions
	int index;
	if ((index = linear_int_arr_search(8, all_directions, dir)) == -1)
		return SHOULD_NOT_REACH_HERE; // Not a direction
	
	int i;
	for (i = 0; i < steps; i++) {
		if (clockwise) dir = all_directions[index == 7 ? 0 : index + 1];
		else dir = all_directions[index == 0 ? 7 : index - 1];
	}
	
	return dir;
}

#endif // DIR_C_INCLUDED