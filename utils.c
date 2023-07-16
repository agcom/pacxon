#ifndef UTILS_C_INCLUDED
#define UTILS_C_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "getch.c"
#include "kbhit.c"

//directions
#define VERTICAL 0b0001
#define HORIZONTAL 0b0010
#define LEFT 0b0100
#define UP 0b1000

//Key Char Codes = KCC
#define LAKCC KEY_LEFT //Left Arrow Key Char Code
#define RAKCC KEY_RIGHT //Right Arrow Key Char Code
#define UAKCC KEY_UP //Up Arrow Key Char Code
#define DAKCC KEY_DOWN //Down Arrow Key Char Code
#define SKCC '\n' //Enter Key Char Code

//something
#define SHOULD_NOT_REACH_HERE -1

#define PERCENT_CHAR '%' //percent character

//point = location; x = row, y = column
typedef struct {
	
	int x, y;
	
} Location;

//clockwise order
static int directions[8] = {VERTICAL | UP, //up
                            HORIZONTAL | VERTICAL | UP | !LEFT, //up right
                            HORIZONTAL | !LEFT, //right
                            HORIZONTAL | VERTICAL | !UP | !LEFT, //down right
                            VERTICAL | !UP, //down
                            HORIZONTAL | VERTICAL | !UP | LEFT, //down left
                            HORIZONTAL | LEFT, //left
                            HORIZONTAL | VERTICAL | UP | LEFT}; //up left

int randFlag() {

    return rand()%2;

}

void randPoint(Location * p, int minx, int maxx, int miny, int maxy) {

    p->x = minx + rand() % (maxx - minx + 1);
    p->y = miny + rand() % (maxy - miny + 1);

}

void print2DStrArray(int rows, int columns, char* c[rows][columns]) {

    int i;
    for(i = 0; i < rows; i++) {

        int j;
        for(j = 0; j < columns; j++) {

            printf("%s", c[i][j]);

        }

        printf("\r\n");

    }

}

void printIntArray(int length, int array[length]) {

    int i;
    for(i = 0; i < length; i++) {

        printf("%d", array[i]);

        if(i != length - 1) printf(", ");

    }

}

void printDirection(int direction) {
	
	int vertical_printed = 0;
	
	if(direction & VERTICAL) {
		
		if(direction & UP) printf("Up");
		else printf("Down");
		
		vertical_printed = 1;
		
	}
	
	if(direction & HORIZONTAL) {
		
		if(direction & LEFT) {
			
			if(vertical_printed) printf(" Left");
			else printf("Left");
			
		} else {
			
			if(vertical_printed) printf(" Right");
			else printf("Right");
			
		}
		
	}
	
	
}

void printDirectionsArray(int length, int directions[length]) {

    int i;
    for(i = 0; i < length; i++) {

        printDirection(directions[i]);

        if(i != length - 1) printf(", ");

    }

}

int Replace2DintArray(int rows, int columns, int array[rows][columns], int key, int replace) {

    int replaceds = 0;

    int i;
    for(i = 0; i < rows; i++) {

        int j;
        for(j = 0; j < columns; j++) {

            if(array[i][j] == key) {

                array[i][j] = replace;
                replaceds++;

            }

        }

    }

    return replaceds;

}

int linearIntArraySearch(const int N, int array[N], int key) {

    int i;
    for(i = 0; i < N; i++) {

        if(array[i] == key) return i;

    }

    return -1;

}

void cls() {
#ifdef WINDOWS
	system("cls");
#else
	system("clear");
#endif
}

void nextLocation(Location * next, int direction) {

    if(direction & VERTICAL) {

        if(direction & UP) { //up

            next->x--;

        } else { //down

            next->x++;

        }

    }

    if(direction & HORIZONTAL) {

        if(direction & LEFT) { //left

            next->y--;

        } else { //right

            next->y++;

        }

    }

}

int reverseDirection(int direction) {

    if(direction & HORIZONTAL) direction ^= LEFT;

    if(direction & VERTICAL) direction ^= UP;

    return direction;

}

int randDirection(int diagonal) {

    if(diagonal) {

        return VERTICAL | HORIZONTAL | (randFlag() ? LEFT : !LEFT) | (randFlag() ? UP : !UP);

    } else {

        int rnd = rand()%4;

        return rnd == 0 ? (HORIZONTAL | LEFT) : rnd == 1 ? (VERTICAL | UP) : rnd == 2 ? (HORIZONTAL | !LEFT) : rnd == 3 ? (VERTICAL | !UP) : SHOULD_NOT_REACH_HERE;


    }

}

void purgePressedKeys() {

    while(_kbhit()) _getch();

}

int areEqualPoints(Location p1, Location p2) {
	
	return p1.x == p2.x && p1.y == p2.y;
	
}

int areNeighborPoints(Location p1, Location p2) {

    if(areEqualPoints(p1, p2)) return 0;

    int i;
    for(i = 0; i < 8; i++) {

        Location temp = p2;

        nextLocation(&temp, directions[i]);

        if(areEqualPoints(p1, temp)) return 1;

    }

    return 0;

}

void printPointArray(int length, Location p[length]) {

    int i;
    for(i = 0; i < length; i++) {

        printf("(%d, %d) ", p[i].x, p[i].y);

    }

}

void pVector(Location p1, Location p2, Location * result) {

    result->x = p2.x - p1.x;
    result->y = p2.y - p1.y;

}

int vectorToDirection(Location vector) {

    if(vector.x < 0 && vector.y < 0) { //up left

        return VERTICAL | HORIZONTAL | LEFT | UP;

    } else if(vector.x < 0 && vector.y == 0) { //up

        return VERTICAL | UP;

    } else if(vector.x < 0 && vector.y > 0) { //up right

        return VERTICAL | HORIZONTAL | !LEFT | UP;

    } else if(vector.x == 0 && vector.y < 0) { //left

        return HORIZONTAL | LEFT;

    } else if(vector.x == 0 && vector.y == 0) { //zero vector

        return SHOULD_NOT_REACH_HERE;

    } else if(vector.x == 0 && vector.y > 0) { //right

        return HORIZONTAL | !LEFT;

    } else if(vector.x > 0 && vector.y < 0) { //down left

        return HORIZONTAL | VERTICAL | !UP | LEFT;

    } else if(vector.x > 0 && vector.y == 0) { //down

        return VERTICAL | !UP;

    } else if(vector.x > 0 && vector.y > 0) { //down right

        return VERTICAL | HORIZONTAL | !UP | !LEFT;

    }

}

int areSideBySideLocations(Location l1, Location l2, int check_being_neighbors) {

    if(check_being_neighbors && !areNeighborPoints(l1, l2)) return 0;

    Location vector;

    pVector(l1 ,l2, &vector);

    int sbs = vectorToDirection(vector);

    if(sbs & VERTICAL && sbs & HORIZONTAL) return 0;
    else return 1;

}

void printPoint(Location p) {

    printf("(%d, %d)", p.x, p.y);

}

int reflectDirection(int direction, int mirror_position) {

    ///

}

int findCommonElementsOfIntArrays(int result_length, int array1_length, int array2_length, int result[result_length], int array1[array1_length], int array2[array2_length]) {

    int rai = 0; //result append index
    int has_space = result_length != 0;

    //set the smaller length array as base

    if(array1_length < array2_length) {

        int i;
        for(i = 0; i < array1_length; i++) {

            if(linearIntArraySearch(array2_length, array2, array1[i]) != -1) { //found a common int

                if(has_space) result[rai] = array1[i];

                rai++;

                if(rai == result_length) has_space = 0;

            }

        }

    } else {

        int i;
        for(i = 0; i < array2_length; i++) {

            if(linearIntArraySearch(array1_length, array1, array2[i]) != -1) { //found a common int

                if(has_space) result[rai] = array2[i];

                rai++;

                if(rai == result_length) has_space = 0;

            }

        }

    }

    return rai;

}

void insertionSortIntArray(int length, int array[length]) {

    int i;
    for(i = 1; i < length; i++) {

        int j;
        for(j = i; array[j] < array[j-1] && j > 0; j--) {

            int t = array[j];
            array[j] = array[j-1];
            array[j-1] = t;

        }

    }

}

int shiftLeftIntArray(int N, int array[N], int remove_index) {

    if(remove_index < 0) return 0;
    if(remove_index == N - 1) return 1;

    int executed = 0;

    int i;
    for(i = remove_index + 1; i < N; i++) {

        int t = array[i];
        array[i] = array[i-1];
        array[i-1] = t;

        if(!executed) executed = 1;

    }

    return executed;

}

void Sleep(long ms) {
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	
	nanosleep(&ts, NULL);
}

void bugAlert(char * msg) {

    printf("Bug : %s", msg);
    Sleep(1000);

}

int turnDirection(int direction, int clockwise, int steps) {

    //find its index at directions
    int index;
    if((index = linearIntArraySearch(8, directions, direction)) == -1) return SHOULD_NOT_REACH_HERE; //not a direction

    int i;
    for(i = 0; i < steps; i++) {

        if(clockwise) {

            direction = directions[index == 7 ? 0 : index + 1];

        } else {

            direction = directions[index == 0 ? 7 : index - 1];

        }

    }

    return direction;

}

#endif // UTILS_C_INCLUDED