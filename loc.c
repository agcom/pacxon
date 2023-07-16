#ifndef LOC_C_INCLUDED
#define LOC_C_INCLUDED

#include <stdlib.h>
#include "dir.c"
#include "utils.c"

// x = row & y = column
typedef struct {
	int x, y;
} loc_t;

void rand_loc(loc_t *p, const int min_x, const int max_x, const int min_y, const int max_y) {
	p->x = min_x + rand() % (max_x - min_x + 1);
	p->y = min_y + rand() % (max_y - min_y + 1);
}

void next_loc(loc_t *next, const dir_t dir) {
	if (dir & VERTICAL) {
		if (dir & UP) next->x--;
		else next->x++;
	}
	
	if (dir & HORIZONTAL) {
		if (dir & LEFT) next->y--;
		else next->y++;
	}
}

int reverse_dir(dir_t dir) {
	if (dir & HORIZONTAL) dir ^= LEFT;
	if (dir & VERTICAL) dir ^= UP;
	
	return dir;
}

int rand_dir(const bool is_diagonal) {
	if (is_diagonal) return VERTICAL | HORIZONTAL | (rand_bool() ? LEFT : !LEFT) | (rand_bool() ? UP : !UP);
	else {
		const int rnd = rand() % 4;
		return rnd == 0 ? (HORIZONTAL | LEFT) :
		       rnd == 1 ? (VERTICAL | UP) :
		       rnd == 2 ? (HORIZONTAL | !LEFT) :
		       rnd == 3 ? (VERTICAL | !UP) : SHOULD_NOT_REACH_HERE;
	}
	
}

bool are_equal_locs(const loc_t p1, const loc_t p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

bool are_neighbour_locs(const loc_t p1, const loc_t p2) {
	if (are_equal_locs(p1, p2)) return false;
	
	int i;
	for (i = 0; i < 8; i++) {
		loc_t t = p2;
		next_loc(&t, all_directions[i]);
		if (are_equal_locs(p1, t)) return true;
	}
	
	return false;
}

void locs_diff_vector(const loc_t p1, const loc_t p2, loc_t *res) {
	res->x = p2.x - p1.x;
	res->y = p2.y - p1.y;
}

int loc_vector_to_dir(const loc_t vector) {
	if (vector.x < 0 && vector.y < 0) return VERTICAL | HORIZONTAL | LEFT | UP; // Up Left
	else if (vector.x < 0 && vector.y == 0) return VERTICAL | UP; // Up
	else if (vector.x < 0 && vector.y > 0) return VERTICAL | HORIZONTAL | !LEFT | UP; // Up Right
	else if (vector.x == 0 && vector.y < 0) return HORIZONTAL | LEFT; // Left
	else if (vector.x == 0 && vector.y > 0) return HORIZONTAL | !LEFT; // Right
	else if (vector.x > 0 && vector.y < 0) return HORIZONTAL | VERTICAL | !UP | LEFT; // Down Left
	else if (vector.x > 0 && vector.y == 0) return VERTICAL | !UP; // Down
	else if (vector.x > 0 && vector.y > 0) return VERTICAL | HORIZONTAL | !UP | !LEFT; // Down Right
	else return SHOULD_NOT_REACH_HERE; // Zero vector
}

bool are_size_by_size_locs(const loc_t l1, const loc_t l2, const bool check_being_neighbors) {
	if (check_being_neighbors && !are_neighbour_locs(l1, l2)) return false;
	
	loc_t vector;
	locs_diff_vector(l1, l2, &vector);
	
	const int diff_dir = loc_vector_to_dir(vector);
	if (diff_dir & VERTICAL && diff_dir & HORIZONTAL) return false;
	else return true;
}

void print_loc(const loc_t p) {
	printf("(%d, %d)", p.x, p.y);
}

void print_loc_arr(const int size, const loc_t locs[size]) {
	int i;
	for (i = 0; i < size; i++) {
		print_loc(locs[i]);
		printf(" ");
	}
}

#endif // LOC_C_INCLUDED