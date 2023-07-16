#ifndef UTILS_C_INCLUDED
#define UTILS_C_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "kbhit.c"

// Directions' masks
#define VERTICAL 0b0001
#define HORIZONTAL 0b0010
#define LEFT 0b0100
#define UP 0b1000

// AKC = Arrow Key Code
#define LEFT_AKC KEY_LEFT
#define RIGHT_AKC KEY_RIGHT
#define UP_AKC KEY_UP
#define DOWN_AKC KEY_DOWN

#define SHOULD_NOT_REACH_HERE (-1)

// x = row & y = column
typedef struct {
	int x, y;
} loc_t;

// Clock-wise order
const int directions_all[8] = {
		VERTICAL | UP, // UP
		HORIZONTAL | VERTICAL | UP | !LEFT, // Up Right
		HORIZONTAL | !LEFT, // Right
		HORIZONTAL | VERTICAL | !UP | !LEFT, // Down Rigth
		VERTICAL | !UP, // Down
		HORIZONTAL | VERTICAL | !UP | LEFT, // Down Left
		HORIZONTAL | LEFT, // Left
		HORIZONTAL | VERTICAL | UP | LEFT  // Up Left
};

bool rand_bool() {
	return rand() % 2;
}

void rand_loc(loc_t *p, const int min_x, const int max_x, const int min_y, const int max_y) {
	p->x = min_x + rand() % (max_x - min_x + 1);
	p->y = min_y + rand() % (max_y - min_y + 1);
}

void print_2d_str_arr(const int rows, const int columns, const char *arr[rows][columns]) {
	int i;
	for (i = 0; i < rows; i++) {
		int j;
		for (j = 0; j < columns; j++) {
			printf("%s", arr[i][j]);
		}
		printf("\r\n");
	}
}

void print_int_arr(const int size, const int arr[size]) {
	int i;
	for (i = 0; i < size; i++) {
		printf("%d", arr[i]);
		if (i != size - 1) printf(", ");
	}
}

void print_dir(const int dir) {
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

void print_dir_arr(const int size, const int dirs[size]) {
	int i;
	for (i = 0; i < size; i++) {
		print_dir(dirs[i]);
		if (i != size - 1) printf(", ");
	}
}

int replace_2d_int_arr(const int rows, const int columns, int arr[rows][columns], const int key, const int replace) {
	int replaceds = 0;
	
	int i;
	for (i = 0; i < rows; i++) {
		int j;
		for (j = 0; j < columns; j++) {
			if (arr[i][j] == key) {
				arr[i][j] = replace;
				replaceds++;
			}
		}
	}
	
	return replaceds;
}

int linear_int_arr_search(const int size, const int arr[size], const int key) {
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == key) return i;
	}
	
	return -1;
}

void clear_screen() {
#ifdef WINDOWS
	system("cls");
#else
	system("clear");
#endif
}

void next_loc(loc_t *next, const int dir) {
	if (dir & VERTICAL) {
		if (dir & UP) next->x--;
		else next->x++;
	}
	
	if (dir & HORIZONTAL) {
		if (dir & LEFT) next->y--;
		else next->y++;
	}
}

int reverse_dir(int dir) {
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

void purge_keys() {
	while (kbhit()) getch();
}

bool are_equal_locs(const loc_t p1, const loc_t p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

bool are_neighbour_locs(const loc_t p1, const loc_t p2) {
	if (are_equal_locs(p1, p2)) return false;
	
	int i;
	for (i = 0; i < 8; i++) {
		loc_t t = p2;
		next_loc(&t, directions_all[i]);
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

int find_common_elements_of_int_arrs(
		const int res_size, const int arr1_size, const int arr2_size, int res[res_size],
		const int arr1[arr1_size], const int arr2[arr2_size]
) {
	
	int rai = 0; // Result Append Index
	bool has_space = res_size != 0;
	
	// Set the smaller length array as base
	if (arr1_size < arr2_size) {
		int i;
		for (i = 0; i < arr1_size; i++) {
			if (linear_int_arr_search(arr2_size, arr2, arr1[i]) != -1) { // Found a common int
				if (has_space) res[rai] = arr1[i];
				rai++;
				
				if (rai == res_size) has_space = false;
			}
		}
	} else {
		int i;
		for (i = 0; i < arr2_size; i++) {
			if (linear_int_arr_search(arr1_size, arr1, arr2[i]) != -1) { // Found a common int
				if (has_space) res[rai] = arr2[i];
				rai++;
				
				if (rai == res_size) has_space = false;
			}
		}
	}
	
	return rai;
}

void insertion_sort_int_arr(int size, int arr[size]) {
	int i;
	for (i = 1; i < size; i++) {
		int j;
		for (j = i; arr[j] < arr[j - 1] && j > 0; j--) {
			const int t = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = t;
		}
	}
}

bool shift_left_int_arr(const int size, int arr[size], const int remove_index) {
	
	if (remove_index < 0 || remove_index >= size) return false;
	else if (remove_index == size - 1) return true;
	
	bool executed = false;
	
	int i;
	for (i = remove_index + 1; i < size; i++) {
		const int t = arr[i];
		arr[i] = arr[i - 1];
		arr[i - 1] = t;
		
		if (!executed) executed = true;
	}
	
	return executed;
}

void sleep_ms(const long ms) {
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	
	nanosleep(&ts, NULL);
}

void bug_alert(const char *msg) {
	printf("Bug: %s", msg);
	sleep_ms(1000);
}

int turn_dir(int dir, const int clockwise, const int steps) {
	
	// Find its index at directions
	int index;
	if ((index = linear_int_arr_search(8, directions_all, dir)) == -1)
		return SHOULD_NOT_REACH_HERE; // Not a direction
	
	int i;
	for (i = 0; i < steps; i++) {
		if (clockwise) dir = directions_all[index == 7 ? 0 : index + 1];
		else dir = directions_all[index == 0 ? 7 : index - 1];
		
	}
	
	return dir;
}

#endif // UTILS_C_INCLUDED