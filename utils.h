#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

//directions
#define VERTICAL 0b0001
#define HORIZONTAL 0b0010
#define LEFT 0b0100
#define UP 0b1000

//Key Char Codes = KCC
#define AKCCM -32 //Arrow Key Char Code Modifier
#define LAKCC 75 //Left Arrow Key Char Code
#define RAKCC 77 //Right Arrow Key Char Code
#define UAKCC 72 //Up Arrow Key Char Code
#define DAKCC 80 //Down Arrow Key Char Code
#define SKCC 32 //Enter Key Char Code

//something
#define SHOULD_NOT_REACH_HERE -1

#define PERCENT_CHAR 37 //percent character

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

//random boolean
int randFlag();

//fills the passed point with random x, y within inclusive specified bounds
void randPoint(Location * p, int minx, int maxx, int miny, int maxy);

//prints the 2D char array
void print2DCharArray(int rows, int columns, char c[rows][columns]);

//clear dos
void cls();

//next step of the move by the specified direction
//next : current location; will be replaced by the new location
void nextLocation(Location * next, int movement);

int randDirection(int diagonal);

void printIntArray(int length, int a[length]);

void printPointArray(int length, Location p[length]);

void printPoint(Location p);

void pVector(Location p1, Location p2, Location * result);

int areEqualPoints(Location p1, Location p2);

int areNeighborPoints(Location p1, Location p2);

int reverseDirection(int direction);

void printIntArray(int length, int array[length]);

void printDirection(int direction);

int linearIntArraySearch(const int N, int array[N], int key);

int Replace2DintArray(int rows, int columns, int array[rows][columns], int key, int replace);

void insertionSortIntArray(int length, int array[length]);

//returns founded elements count; result array filled length
int findCommonElementsOfIntArrays(int result_length, int array1_length, int array2_length, int result[result_length], int array1[array1_length], int array2[array2_length]);

//returns the shift execution
int shiftLeftIntArray(int N, int array[N], int remove_index);

void bugAlert(char * msg);

int areSideBySideLocations(Location l1, Location l2, int check_being_neighbors);

int turnDirection(int direction, int clockwise, int steps);

#endif // UTILS_H_INCLUDED
