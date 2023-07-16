/**
to dos :
    - brick zone ghost creation : appears after 5%
    - brick zone ghost move algorithm
    - power ups
    - progress != 100% bug
    - score
    - collisions...
    - restart
    - save and load
    - levels
    - menu
    - reflect_dir method (int reflect_dir(const int dir, const int mirror_pos)
    - colored char struct
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "utils.c"
#include "ghosts.c"
#include "pacman.c"
#include "kbhit.c"
#include "dir.c"
#include "loc.c"

#define MAX_GHOSTS 100 // Maximum capacity of the ghosts array

#define REFRESH_RATE 30 // In Hzs; affects velocity of things in game (pac-man and ghosts).

// Shapes
#define BRICK_CHAR "▓"

// The whole game board dimensions
#define ROWS 30
#define COLUMNS 70

#define WIN_PROGRESS 80 // Win percentage

// Ghosts quantity
#define FLOAT_GHOSTS_COUNT 2
#define RAIL_GHOSTS_COUNT 1
#define BRICK_ZONE_GHOSTS_COUNT 0
#define GHOSTS_SIZE FLOAT_GHOSTS_COUNT + RAIL_GHOSTS_COUNT + BRICK_ZONE_GHOSTS_COUNT // Count of active ghosts
#define GHOSTS_SPEED_RATIO 3 // Ghosts' speed = pac-man's speed / this ratio

ghost_t ghosts[MAX_GHOSTS]; // Ghosts pool

int bricks[ROWS][COLUMNS]; // -1 for bricks and >=0 for zones

pac_man pm; // The hero

char *picture[ROWS][COLUMNS]; // The whole things view except the header
bool is_view_update = false; // The drawn picture is the current state or not

int score = 0;
int progress = 0; // Current progress; from 100.

bool is_brick_there(const loc_t loc) {
	// Check location to be in bounds
	if (loc.x < 0 || loc.x > ROWS - 1 || loc.y < 0 || loc.y > COLUMNS - 1) return false; // Out of bounds location
	return bricks[loc.x][loc.y] < 0;
}

// For initGhosts method
void add_ghost(const int type, const int index) {
	ghosts[index].type = type;
	
	switch (type) {
		case FLOAT_GHOST : {
			ghosts[index].locs_size = 1;
			rand_loc(&(ghosts[index].locs[0]), 1, ROWS - 2, 1, COLUMNS - 2);
			ghosts[index].movement = rand_dir(1);
			
			break;
		}
		case RAIL_GHOST : {
			ghosts[index].locs_size = 2;
			
			const int bricks_count = (ROWS * 2 + COLUMNS * 2) - 4; // Count of bricks at the start of the game
			const int rand_brick = rand() % bricks_count;
			
			// rand_room to location
			int i, counter = 0;
			for (i = 0; i < ROWS; i++) {
				int j;
				for (j = 0; j < COLUMNS; j++) {
					if (bricks[i][j] == -1) {
						if (counter == rand_brick) {
							ghosts[index].locs[0].x = i;
							ghosts[index].locs[0].y = j;
							
							// Break both loops
							i = ROWS;
							break;
						}
						counter++;
					}
				}
			}
			
			// Choose a random possible direction
			int possibilities[2];
			int possibilities_append_index = 0;
			for (i = 0; i < 8; i++) {
				loc_t t = ghosts[index].locs[0];
				next_loc(&t, all_directions[i]);
				
				if (is_brick_there(t)) {
					possibilities[possibilities_append_index++] = all_directions[i];
				}
			}
			
			ghosts[index].movement = possibilities[rand() % 2];
			ghosts[index].locs[1] = ghosts[index].locs[0];
			
			if (ghosts[index].movement & VERTICAL) { // Vertical: Left or Right
				if (ghosts[index].locs[0].y == 0) { // Right
					next_loc(&ghosts[index].locs[1], HORIZONTAL | !LEFT);
				} else { // Left
					next_loc(&ghosts[index].locs[1], HORIZONTAL | LEFT);
				}
			} else { // Horizontal: Up or Down
				if (ghosts[index].locs[0].x == 0) { // Down
					next_loc(&ghosts[index].locs[1], VERTICAL | !UP);
				} else { // Up
					next_loc(&ghosts[index].locs[1], VERTICAL | UP);
				}
			}
			
			break;
		}
		
		case BRICK_ZONE_GHOST : {
			// Should initialize the location after the first zone captured
			ghosts[index].locs_size = 1;
			ghosts[index].locs[0].x = -1;
			ghosts[index].locs[0].y = -1;
			ghosts[index].movement = rand_dir(1);
			
			break;
		}
		
	}
}

void init_ghosts() {
	int append_index = 0;
	
	int i;
	for (i = 0; i < FLOAT_GHOSTS_COUNT; i++) {
		add_ghost(FLOAT_GHOST, append_index);
		append_index++;
	}
	
	for (i = 0; i < RAIL_GHOSTS_COUNT; i++) {
		add_ghost(RAIL_GHOST, append_index);
		append_index++;
	}
	
	for (i = 0; i < BRICK_ZONE_GHOSTS_COUNT; i++) {
		add_ghost(BRICK_ZONE_GHOST, append_index);
		append_index++;
	}
}

void init_pac_man() {
	pm.location.x = 0;
	pm.location.y = 0;
	
	pm.movement = -1;
	pm.lives = 3;
}

// Initial bricks layout
void init_bricks() {
	int i;
	for (i = 0; i < ROWS; i++) {
		int j;
		for (j = 0; j < COLUMNS; j++) {
			if (i == 0 || i == ROWS - 1 || j == 0 || j == COLUMNS - 1) bricks[i][j] = -1;
			else bricks[i][j] = 0; // First zone
		}
	}
}

void update_picture() {
	int i;
	for (i = 0; i < ROWS; i++) {
		int j;
		for (j = 0; j < COLUMNS; j++) {
			if (bricks[i][j] == -1) picture[i][j] = BRICK_CHAR;
			else if (bricks[i][j] == -2) picture[i][j] = PAC_MAN_TRACE_CHAR;
			else picture[i][j] = " "; // Empty
		}
	}
	
	// Add pac-man
	picture[pm.location.x][pm.location.y] = PAC_MAN_CHAR;
	
	// Add ghosts
	for (i = 0; i < GHOSTS_SIZE; i++) {
		int j;
		for (j = 0; j < ghosts[i].locs_size; j++) {
			picture[ghosts[i].locs[j].x][ghosts[i].locs[j].y] = (
					ghosts[i].type == FLOAT_GHOST ? FLOAT_GHOST_CHAR :
					ghosts[i].type == RAIL_GHOST ? RAIL_GHOST_CHAR :
					ghosts[i].type == BRICK_ZONE_GHOST ? BRICK_ZONE_GHOST_CHAR :
					SHOULD_NOT_REACH_HERE
			);
		}
	}
	
	is_view_update = true;
}

void draw_header() {
	printf("Lives: %d\tScore: %d\tProgress: %d/%d%%\r\n", pm.lives, score, progress, WIN_PROGRESS);
}

// Draws everything
void draw_game() {
	if (is_view_update) return;
	
	update_picture();
	clear_screen();
	draw_header();
	print_2d_str_arr(ROWS, COLUMNS, picture);
}

void init() {
	init_bricks();
	init_ghosts();
	init_pac_man();
}

void move_ghost(const int index) {
	switch (ghosts[index].type) {
		case FLOAT_GHOST : {
			if (ghosts[index].movement == -1) break;
			
			loc_t next = ghosts[index].locs[0];
			next_loc(&next, ghosts[index].movement);
			
			if (!is_brick_there(next)) { // Execute the move
				ghosts[index].locs[0] = next;
				is_view_update = 0;
			} else { // Opposite the vertical movement
				ghosts[index].movement = ghosts[index].movement ^ UP; // Opposite it
				
				// Reset the point
				next = ghosts[index].locs[0];
				next_loc(&next, ghosts[index].movement);
				
				if (!is_brick_there(next)) { // Execute the move
					ghosts[index].locs[0] = next;
					is_view_update = 0;
				} else { // Opposite the horizontal movement
					ghosts[index].movement = ghosts[index].movement ^ UP; // Undo vertical changes
					ghosts[index].movement = ghosts[index].movement ^ LEFT; // Opposite it
					
					// Reset the point
					next = ghosts[index].locs[0];
					next_loc(&next, ghosts[index].movement);
					
					if (!is_brick_there(next)) { // Execute the move
						ghosts[index].locs[0] = next;
						is_view_update = 0;
					} else { // Reverse both movements
						ghosts[index].movement = ghosts[index].movement ^ UP;
						
						// Reset the point
						next = ghosts[index].locs[0];
						next_loc(&next, ghosts[index].movement);
						
						if (!is_brick_there(next)) { // Execute the move
							ghosts[index].locs[0] = next;
							is_view_update = 0;
						} else { // Stuck in a rail; find possible moving directions.
							
							int pdai = 0; // Possible Directions Append Index
							int possible_dirs[4];
							
							int j;
							for (j = 0; j < 8; j++) {
								if (all_directions[j] & VERTICAL && all_directions[j] & HORIZONTAL)
									continue; // Skip 2D directions
								
								loc_t t = ghosts[index].locs[0];
								next_loc(&t, all_directions[j]);
								
								if (!is_brick_there(t)) { // Add
									possible_dirs[pdai++] = all_directions[j];
								}
							}
							
							if (pdai == 0); // Actually stuck
							else {
								// Undo changes
								ghosts[index].movement = reverse_dir(ghosts[index].movement);
								
								// Find nearest direction to current moving direction from possible_dirs
								int nearest_dir_index = -1;
								int nearest_dir_changes = -1;
								
								// Clock-wise and non
								for (j = 0; j <= 1; j++) {
									int k;
									for (k = 0; k < pdai; k++) {
										int changes = 0;
										int direction = ghosts[index].movement;
										
										while (direction != possible_dirs[k]) {
											direction = turn_dir(direction, j, 1);
											changes++;
										}
										
										if (nearest_dir_index == -1 || changes < nearest_dir_changes) {
											nearest_dir_index = k;
											nearest_dir_changes = changes;
										}
									}
								}
								
								// Update direction on this algorithm
								
								if (pdai == 1) {
									ghosts[index].movement = reverse_dir(ghosts[index].movement);
								} else {
									if (possible_dirs[nearest_dir_index] & HORIZONTAL) {
										ghosts[index].movement ^= UP;
									} else { // Vertical
										ghosts[index].movement ^= LEFT;
									}
								}
								
								// Move on nearest direction
								next_loc(&ghosts[index].locs[0], possible_dirs[nearest_dir_index]);
								is_view_update = 0;
							}
						}
					}
				}
			}
			
			break;
		}
		case RAIL_GHOST : {
			// First and second locations should be sync
			int fpai = 0; // First Possibilities Append Index
			int first_possibilities[4];
			
			int spai = 0; // Second Possibilities Append Index
			int second_possibilities[4];
			
			// Check first location moving possibilities
			int j;
			for (j = 0; j < 8; j++) {
				if (all_directions[j] & VERTICAL && all_directions[j] & HORIZONTAL) continue; //skip 2D directions
				
				loc_t next = ghosts[index].locs[0];
				next_loc(&next, all_directions[j]);
				
				if (is_brick_there(next)) { //possible
					first_possibilities[fpai++] = all_directions[j];
				}
			}
			
			// Check second location moving possibilities
			for (j = 0; j < 8; j++) {
				if (all_directions[j] & VERTICAL && all_directions[j] & HORIZONTAL) continue; // Skip 2D directions
				
				loc_t next = ghosts[index].locs[1];
				next_loc(&next, all_directions[j]);
				
				if (!is_brick_there(next)) { // Possible
					second_possibilities[spai++] = all_directions[j];
				}
			}
			
			// Current moving direction possibility
			int first_possibility = linear_int_arr_search(fpai, first_possibilities, ghosts[index].movement) != -1;
			int second_possibility = linear_int_arr_search(spai, second_possibilities, ghosts[index].movement) != -1;
			
			int should_change_first_direction = 0;
			int should_change_second_direction = 0;
			
			if (first_possibility) {
				loc_t next = ghosts[index].locs[0];
				next_loc(&next, ghosts[index].movement);
				
				// Check to stay neighbors
				if (are_neighbour_locs(next, ghosts[index].locs[1])) { // Execute it
					ghosts[index].locs[0] = next;
					is_view_update = 0;
				} else should_change_first_direction = 1;
			} else should_change_first_direction = 1;
			if (second_possibility) {
				loc_t next = ghosts[index].locs[1];
				next_loc(&next, ghosts[index].movement);
				
				// Check to stay neighbors
				if (are_neighbour_locs(next, ghosts[index].locs[0])) { // Execute it
					ghosts[index].locs[1] = next;
					is_view_update = 0;
				} else should_change_second_direction = 1;
			} else should_change_second_direction = 1;
			
			if (should_change_first_direction && should_change_second_direction) {
				// Find common possible directions then remove the reverse of current direction
				int commons[2];
				int cai = find_common_elements_of_int_arrs(
						2, fpai, spai, commons,
						first_possibilities, second_possibilities
				); // Commons' append index
				
				if (cai != 0) { // Found some commons
					// Remove the reverse of current direction
					if (shift_left_int_arr(
							cai, commons,
							linear_int_arr_search(cai, commons, reverse_dir(ghosts[index].movement))
					)) {
						cai--;
					} else bug_alert("Reverse of current direction not removed!\n");
					
					if (cai != 0) {
						ghosts[index].movement = commons[0];
						// If ghosts are not side by side, reach that, else normal move.
						if (!are_size_by_size_locs(
								ghosts[index].locs[1], ghosts[index].locs[0],
								0
						)) { // Are not side by size
							loc_t next = ghosts[index].locs[0];
							next_loc(&next, ghosts[index].movement);
							
							// Check if now side by side
							if (!are_size_by_size_locs(ghosts[index].locs[1], next, 0)) { //are not side by side
								next = ghosts[index].locs[1];
								next_loc(&next, ghosts[index].movement);
								
								// Check if now side by side
								if (!are_size_by_size_locs(next, ghosts[index].locs[0], 0)) { // Stuck
									// WTF
								} else { // Execute
									ghosts[index].locs[1] = next;
									is_view_update = 0;
								}
							} else { // Are side by side; execute the move.
								ghosts[index].locs[0] = next;
								is_view_update = 0;
							}
						} else { // Are side by side; do a normal move.
							move_ghost(index);
						}
					} else { // No commons after removing reverse direction of current moving direction
						
						// Find a direction based on being side by side but not reverse of current direction
						for (j = 0; j < 8; j++) {
							if ((all_directions[j] & VERTICAL && all_directions[j] & HORIZONTAL) ||
							    all_directions[j] == reverse_dir(ghosts[index].movement))
								continue; // Skip 2D directions and reverse direction of current direction
							
							loc_t next = ghosts[index].locs[0];
							next_loc(&next, all_directions[j]);
							
							if (is_brick_there(next)) { // Check to be side by side
								if (!are_size_by_size_locs(ghosts[index].locs[1], next, 0)) {
									continue; // Not side by side
								} else { // Found it; execute the move.
									ghosts[index].movement = all_directions[j];
									move_ghost(index);
									break;
								}
							}
						}
					}
				} else { // Stuck
					
					// Find a direction based on being side by side but not reverse of current direction
					for (j = 0; j < 8; j++) {
						if ((all_directions[j] & VERTICAL && all_directions[j] & HORIZONTAL) ||
						    all_directions[j] == reverse_dir(ghosts[index].movement))
							continue; // Skip 2D directions and reverse direction of current direction
						
						loc_t next = ghosts[index].locs[0];
						next_loc(&next, all_directions[j]);
						
						if (is_brick_there(next)) {
							if (!are_size_by_size_locs(ghosts[index].locs[1], next, 0)) continue; // Not side by side
							else { // Found it; execute the move
								ghosts[index].movement = all_directions[j];
								move_ghost(index);
								break;
							}
						}
					}
				}
			}
			
			break;
		}
		case BRICK_ZONE_GHOST : {
			// TODO
			break;
		}
	}
}

// Controls ghosts movement
void move_ghosts() {
	static int called_times = 0;
	called_times++;
	
	if (called_times == GHOSTS_SPEED_RATIO) {
		int i;
		for (i = 0; i < GHOSTS_SIZE; i++) {
			move_ghost(i);
		}
		
		called_times = 0;
	}
}

bool can_pac_man_move(const int dir) {
	if (dir & HORIZONTAL) {
		if (dir & LEFT) return pm.location.y - 1 >= 0; // Left
		else return pm.location.y + 1 < COLUMNS; // Right
	} else if (dir & VERTICAL) {
		if (dir & UP) return pm.location.x - 1 >= 0; // Up
		else return pm.location.x + 1 < ROWS; // Down
	} else return SHOULD_NOT_REACH_HERE;
}

void update_zones() {
	int current_color = 0;
	int current_color_use_time = 0;
	
	// Replace pac-man trace with bricks
	replace_2d_int_arr(ROWS, COLUMNS, bricks, -2, -1);
	
	int i;
	for (i = 0; i < ROWS; i++) {
		int j;
		for (j = 0; j < COLUMNS; j++) {
			if (bricks[i][j] == -1) {
				if (current_color_use_time > 0) current_color++;
			} else {
				bricks[i][j] = current_color;
				current_color_use_time++;
			}
		}
	}
	
	for (i = 0; i < ROWS; i++) {
		int j;
		for (j = 0; j < COLUMNS; j++) {
			if (bricks[i][j] != -1) {
				// Check neighbors
				loc_t l;
				
				int k;
				for (k = 0; k < 8; k++) {
					// Reset
					l.x = i;
					l.y = j;
					
					next_loc(&l, all_directions[k]);
					
					if (bricks[l.x][l.y] == -1) continue;
					else { // A zone number
						int o;
						for (o = 0; o < ROWS; o++) {
							int p;
							for (p = 0; p < COLUMNS; p++) {
								if (bricks[o][p] == -1) continue;
								else if (bricks[o][p] == bricks[i][j]) bricks[o][p] = bricks[l.x][l.y];
							}
						}
					}
				}
			}
		}
	}
}

void try_capture_zones() {
	int ghosts_zones[FLOAT_GHOSTS_COUNT * FLOAT_GHOST_LOCS_SIZE + RAIL_GHOSTS_COUNT * RAIL_GHOST_LOCS_SIZE +
	                 BRICK_ZONE_GHOSTS_COUNT * BRICK_ZONE_GHOST_LOCS_SIZE];
	
	int i;
	for (i = 0; i < GHOSTS_SIZE; i++) {
		int j;
		for (j = 0; j < ghosts[i].locs_size; j++) {
			ghosts_zones[i] = bricks[ghosts[i].locs[j].x][ghosts[i].locs[j].y];
		}
	}
	
	for (i = 0; i < ROWS; i++) {
		int j;
		for (j = 0; j < COLUMNS; j++) {
			if (bricks[i][j] == -1) continue;
			else if (linear_int_arr_search(GHOSTS_SIZE, ghosts_zones, bricks[i][j]) == -1) {
				replace_2d_int_arr(ROWS, COLUMNS, bricks, bricks[i][j], -1);
			}
		}
	}
}

void update_progress() {
	int bricks_count = 0;
	
	int i;
	for (i = 0; i < ROWS; i++) {
		int j;
		for (j = 0; j < COLUMNS; j++) {
			if (bricks[i][j] == -1) bricks_count++;
		}
	}
	
	static int all_bricks_count = (ROWS - 2) * (COLUMNS - 2);
	
	progress = (int) ((float) (bricks_count - (2 * (ROWS + COLUMNS))) / (float) all_bricks_count * 100);
	is_view_update = 0;
}

bool move_pac_man(const int dir, const int auto_call) {
	if (!can_pac_man_move(dir)) return false;
	
	if (!auto_call && pm.movement != -1) {
		if (reverse_dir(pm.movement) != dir) pm.movement = dir;
		return true;
	}
	
	next_loc(&pm.location, dir);
	
	if (bricks[pm.location.x][pm.location.y] != -1) { // Stepped out of the safe zone, run!
		bricks[pm.location.x][pm.location.y] = -2;
		
		pm.movement = dir;
	} else { // Reached the end or moving on the safe zone
		if (pm.movement != -1) { // Reached the end
			update_zones();
			try_capture_zones();
			update_progress();
			pm.movement = -1;
		}
	}
	
	is_view_update = 0;
	
	return true;
}

void control_auto_moves() {
	move_ghosts();
	
	if (pm.movement != -1) move_pac_man(pm.movement, 1);
}

int arrow_to_dir(const int arrow_key_code) {
	switch (arrow_key_code) {
		case UP_AKC :
			return VERTICAL | UP;
		case DOWN_AKC :
			return VERTICAL | !UP;
		case LEFT_AKC :
			return HORIZONTAL | LEFT;
		case RIGHT_AKC :
			return HORIZONTAL | !LEFT;
		default:
			return SHOULD_NOT_REACH_HERE;
	}
}

int main() {
	initscr();
	noecho();
	nl();
	keypad(stdscr, TRUE);
	cbreak();
	nodelay(stdscr, TRUE);
	
	srand(time(NULL)); //for absolute random
	
	clear_screen();
	init();
	draw_game();
	
	while (true) {
		
		if (kbhit()) {
			int c = getch();
			switch (c) { // Key press handler
				case LEFT_AKC:
				case RIGHT_AKC:
				case UP_AKC:
				case DOWN_AKC : { // Arrow key
					move_pac_man(arrow_to_dir(c), 0);
					break;
				}
			}
		}
		
		purge_keys();
		control_auto_moves();
		draw_game();
		sleep_ms(1000 / REFRESH_RATE);
	}
	
	endwin();
	return 0;
}