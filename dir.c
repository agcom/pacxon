#ifndef DIR_C_INCLUDED
#define DIR_C_INCLUDED

// Directions' masks
#define VERTICAL 0b0001
#define HORIZONTAL 0b0010
#define LEFT 0b0100
#define UP 0b1000

// Clock-wise order
const int all_directions[8] = {
		VERTICAL | UP, // UP
		HORIZONTAL | VERTICAL | UP | !LEFT, // Up Right
		HORIZONTAL | !LEFT, // Right
		HORIZONTAL | VERTICAL | !UP | !LEFT, // Down Right
		VERTICAL | !UP, // Down
		HORIZONTAL | VERTICAL | !UP | LEFT, // Down Left
		HORIZONTAL | LEFT, // Left
		HORIZONTAL | VERTICAL | UP | LEFT  // Up Left
};

#endif // DIR_C_INCLUDED