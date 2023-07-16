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
    - reflectDirection method
    - colored char struct
**/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "utils.c"
#include "ghosts.c"
#include "pacman.c"

#define MAX_GHOSTS 100 //maximum capacity of the ghosts array

#define REFRESH_RATE 120 //Hz; affects velocity of things in game (pac man, ghosts)

//shapes
#define BRICK_CHAR 178

//the whole game boar dimensions
#define ROWS 30
#define COLUMNS 70

#define WIN_PROGRESS 80 //win condition

//global variables
int float_ghosts = 2;
int rail_ghosts = 1;
int brick_zone_ghosts = 0;
int ghosts_quantity; //count of active ghosts
Ghost ghosts[MAX_GHOSTS]; //ghosts pool
int bricks[ROWS][COLUMNS]; //-1 for bricks, >= 0 for zones
int view_validation = 0; //the drawn picture is the current state or not, if not redraw it
PacMan pm; //the hero
char picture[ROWS][COLUMNS]; //the whole things view except the header
int score = 0; //score
int progress = 0; //current progress; from 100
int ghosts_speed_ratio = 3; //ghosts speed = pac man speed / this ratio

//how ghosts start
//f = count of float ghosts
//r = count of rail ghosts
//b = count of brick zone ghosts
//f + r + b == ghosts_quantity
void initGhosts() {

    int append_index = 0;

    int i;
    for(i = 0; i < float_ghosts; i++) { //float ghosts

        addGhost(FLOAT_GHOST, append_index);
        append_index++;

    }

    for(i = 0; i < rail_ghosts; i++) { //rail ghosts

        addGhost(RAIL_GHOST, append_index);
        append_index++;

    }

    for(i = 0; i < brick_zone_ghosts; i++) { //brick zone ghosts

        addGhost(BRICK_ZONE_GHOST, append_index);
        append_index++;

    }

}

//for initGhosts method
void addGhost(int type, int index) {

    ghosts[index].type = type;

    switch(type) {

        case FLOAT_GHOST : {

            ghosts[index].locationss = 1;
            randPoint(&(ghosts[index].locations[0]), 1, ROWS - 2, 1, COLUMNS - 2);
            ghosts[index].movement = randDirection(1);

            break;
        }

        case RAIL_GHOST : {

            ghosts[index].locationss = 2;

            int bricks_count = (ROWS * 2 + COLUMNS * 2) - 4; //count of bricks at the start of the game

            int rand_room = rand() % bricks_count;

            //rand_room to location
            int i, counter = 0;
            for(i = 0; i < ROWS; i++) {

                int j;
                for(j = 0; j < COLUMNS; j++) {

                    if(bricks[i][j] == -1) {

                        if(counter == rand_room) {

                            ghosts[index].locations[0].x = i;
                            ghosts[index].locations[0].y = j;

                            //break both loops
                            i = ROWS;
                            break;

                        }

                        counter++;

                    }

                }

            }

            //choose a random possible direction
            int possiblitites[2];
            int possiblitites_append = 0;
            for(i = 0; i < 8; i++) {

                Location t = ghosts[index].locations[0];
                nextLocation(&t, directions[i]);

                if(isBrickThere(t)) {

                    possiblitites[possiblitites_append++] = directions[i];

                    printf("%d", possiblitites_append);

                }

            }

            ghosts[index].movement = possiblitites[rand()%2];

            ghosts[index].locations[1] = ghosts[index].locations[0];

            if(ghosts[index].movement & VERTICAL) { //vertical

                //left or right
                if(ghosts[index].locations[0].y == 0) { //right

                    nextLocation(&ghosts[index].locations[1], HORIZONTAL | !LEFT);

                } else { //left

                    nextLocation(&ghosts[index].locations[1], HORIZONTAL | LEFT);

                }

            } else { //horizontal

                //up or down
                if(ghosts[index].locations[0].x == 0) { //down

                    nextLocation(&ghosts[index].locations[1], VERTICAL | !UP);

                } else { //up

                    nextLocation(&ghosts[index].locations[1], VERTICAL | UP);

                }

            }

            break;

        }

        case BRICK_ZONE_GHOST : {

            //should initialize the location after the first zone captured
            ghosts[index].locationss = 1;
            ghosts[index].locations[0].x = -1;
            ghosts[index].locations[0].y = -1;
            ghosts[index].movement = randDirection(1);

            break;
        }

    }

    index++;

}

//how pac man starts
void initPacMan() {

    pm.location.x = 0;
    pm.location.y = 0;

    pm.movement = -1;
    pm.lives = 3;

}

//bricks layout at the start
void initBricks() {

    int i;
    for(i = 0; i < ROWS; i++) {

        int j;
        for(j = 0; j < COLUMNS; j++) {

            if(i == 0 || i == ROWS - 1 || j == 0 || j == COLUMNS - 1) bricks[i][j] = -1;
            else bricks[i][j] = 0; //first zone

        }

    }

}

//draws whole things
void drawGame() {

    if(view_validation) return;

    updatePicture();

    cls();

    //print the header
    drawHeader();

    //print the picture
    print2DCharArray(ROWS, COLUMNS, picture);

}

void init() {

    ghosts_quantity = float_ghosts + rail_ghosts + brick_zone_ghosts;

    initBricks();
    initGhosts();
    initPacMan();

}

void updatePicture() {

    int i;
    for(i = 0; i < ROWS; i++) {

        int j;
        for(j = 0; j < COLUMNS; j++) {

            if(bricks[i][j] == -1) picture[i][j] = BRICK_CHAR; //brick here
            else if(bricks[i][j] == -2) picture[i][j] = PAC_MAN_TRACE_CHAR;//pacman trace
            else picture[i][j] = ' '; //empty

        }

    }

    //add pac man
    picture[pm.location.x][pm.location.y] = PAC_MAN_CHAR;

    //add ghosts
    for(i = 0; i < ghosts_quantity; i++) {

        int j;
        for(j = 0; j < ghosts[i].locationss; j++) {

            picture[ghosts[i].locations[j].x][ghosts[i].locations[j].y] = (ghosts[i].type == FLOAT_GHOST ? FLOAT_GHOST_CHAR : ghosts[i].type == RAIL_GHOST ? RAIL_GHOST_CHAR : ghosts[i].type == BRICK_ZONE_GHOST ? BRICK_ZONE_GHOST_CHAR : SHOULD_NOT_REACH_HERE);

        }


    }

    view_validation = 1;

}

void drawHeader() {

    printf("Lives: %d\tScore: %d\tProgress: %d/%d%c\n", pm.lives, score, progress, WIN_PROGRESS, PERCENT_CHAR);

}

void controlAutoMoves() {

    moveGhosts();

    if(pm.movement != -1) movePacMan(pm.movement, 1);

}

//controls ghosts movement
void moveGhosts() {

    static int called_times = 0;
    called_times++;

    if(called_times == ghosts_speed_ratio) {

        int i;
        for(i = 0; i < ghosts_quantity; i++) {

            moveGhost(i);

        }

        called_times = 0;

    }

}

void moveGhost(int i) {

    switch(ghosts[i].type) {

        case FLOAT_GHOST : {

            if(ghosts[i].movement == -1) break;

            Location next = ghosts[i].locations[0];

            nextLocation(&next, ghosts[i].movement);

            if(!isBrickThere(next)) { //execute the move

                ghosts[i].locations[0] = next;
                view_validation = 0;

            } else { //opposite the vertical movement

                ghosts[i].movement = ghosts[i].movement ^ UP; //opposite it
                //reset the point
                next = ghosts[i].locations[0];
                nextLocation(&next, ghosts[i].movement);

                if(!isBrickThere(next)) { //execute the move

                    ghosts[i].locations[0] = next;
                    view_validation = 0;

                } else { //opposite the horizontal movement

                    ghosts[i].movement = ghosts[i].movement ^ UP; //undo vertical changes
                    ghosts[i].movement = ghosts[i].movement ^ LEFT; //opposite it
                    //reset the point
                    next = ghosts[i].locations[0];
                    nextLocation(&next, ghosts[i].movement);

                    if(!isBrickThere(next)) { //execute the move

                        ghosts[i].locations[0] = next;
                        view_validation = 0;

                    } else { //reverse both movements

                        ghosts[i].movement = ghosts[i].movement ^ UP;
                        //reset the point
                        next = ghosts[i].locations[0];
                        nextLocation(&next, ghosts[i].movement);

                        if(!isBrickThere(next)) { //execute the move

                            ghosts[i].locations[0] = next;
                            view_validation = 0;

                        } else { //stuck

                            //stuck in a rail

                            //find possible moving directions

                            int pdai = 0; //possible directions append index
                            int possible_directions[4];

                            int j;
                            for(j = 0; j < 8; j++) {

                                if(directions[j] & VERTICAL && directions[j] & HORIZONTAL) continue; //skip 2D directions

                                Location t = ghosts[i].locations[0];
                                nextLocation(&t, directions[j]);

                                if(!isBrickThere(t)) { //add

                                    possible_directions[pdai++] = directions[j];

                                }

                            }

                            //debug
//                            printf("possible directions : ");
//                            printDirectionsArray(pdai, possible_directions);
//                            printf("\n");

                            if(pdai == 0); //actually stuck
                            else {

                                //undo changes
                                ghosts[i].movement = reverseDirection(ghosts[i].movement);

                                //debug
//                                printf("current direction : ");
//                                printDirection(ghosts[i].movement);
//                                printf("\n");

                                //find nearest direction to current moving direction from possible_directions
                                int nearest_direction_index = -1;
                                int nearest_direction_changes = -1;

                                 //clockwise and non
                                for(j = 0; j <= 1; j++) {

                                    int k;
                                    for(k = 0; k < pdai; k++) {

                                        int changes = 0;
                                        int direction = ghosts[i].movement;

                                        while(direction != possible_directions[k]) {

                                            direction = turnDirection(direction, j, 1);
                                            changes++;

                                        }

                                        if(nearest_direction_index == -1 || changes < nearest_direction_changes) {

                                            nearest_direction_index = k;
                                            nearest_direction_changes = changes;

                                        }

//                                        printf("%s To ", j == 1 ? "clockwise" : "non_clockwise");
//                                        printDirection(possible_directions[k]);
//                                        printf(" = %d\t", changes);

                                    }

                                }

                                //update direction on this algorithm

                                if(pdai == 1) {

                                    ghosts[i].movement = reverseDirection(ghosts[i].movement);

                                } else {

                                    if(possible_directions[nearest_direction_index] & HORIZONTAL) {

                                        ghosts[i].movement ^= UP;

                                    } else { //vertical

                                        ghosts[i].movement ^= LEFT;

                                    }

                                }

                                //move on nearest direction
                                nextLocation(&ghosts[i].locations[0], possible_directions[nearest_direction_index]);
                                view_validation = 0;

                            }

                        }

                    }

                }

            }

            break;
        }

        case RAIL_GHOST : {

            //first and second locations should be sync

            int fpai = 0; //first possibilities append index
            int first_possibilities[4];

            int spai = 0; //second possibilities append index
            int second_possibilities[4];

            //check first location moving possibilities
            int j;
            for(j = 0; j < 8; j++) {

                if(directions[j] & VERTICAL && directions[j] & HORIZONTAL) continue; //skip 2D directions

                Location next = ghosts[i].locations[0];
                nextLocation(&next, directions[j]);

                if(isBrickThere(next)) { //possible

                    first_possibilities[fpai++] = directions[j];

                }

            }

            //check second location moving possibilities
            for(j = 0; j < 8; j++) {

                if(directions[j] & VERTICAL && directions[j] & HORIZONTAL) continue; //skip 2D directions

                Location next = ghosts[i].locations[1];
                nextLocation(&next, directions[j]);

                if(!isBrickThere(next)) { //possible

                    second_possibilities[spai++] = directions[j];

                }

            }

            //current moving direction possibility
            int first_possibility = linearIntArraySearch(fpai, first_possibilities, ghosts[i].movement) != -1;
            int second_possibility = linearIntArraySearch(spai, second_possibilities, ghosts[i].movement) != -1;

            int should_change_first_direction = 0;
            int should_change_second_direction = 0;

            if(first_possibility) {

                Location next = ghosts[i].locations[0];
                nextLocation(&next, ghosts[i].movement);

                //check to stay neighbors
                if(areNeighborPoints(next, ghosts[i].locations[1])) { //execute it

                    ghosts[i].locations[0] = next;
                    view_validation = 0;

                } else should_change_first_direction = 1;

            } else should_change_first_direction = 1;

            if(second_possibility) {

                Location next = ghosts[i].locations[1];
                nextLocation(&next, ghosts[i].movement);

                //check to stay neighbors
                if(areNeighborPoints(next, ghosts[i].locations[0])) { //execute it

                    ghosts[i].locations[1] = next;
                    view_validation = 0;

                } else should_change_second_direction = 1;

            } else should_change_second_direction = 1;

            if(should_change_first_direction && should_change_second_direction) {

                //find common possible directions then remove the reverse of current direction
                int commons[2];
                int cai = findCommonElementsOfIntArrays(2, fpai, spai, commons, first_possibilities, second_possibilities); //commons append index

                if(cai != 0) { //found some commons

//                    printf("remove index : %d\n", linearIntArraySearch(cai, commons, reverseDirection(ghosts[i].movement)));
//                    printf("commons before remove %d : ", cai);
//                    printDirectionsArray(cai, commons);
//                    printf("\n");
//
//                    printf("first p %d : ", fpai);
//                    printDirectionsArray(fpai, first_possibilities);
//                    printf("\n");
//
//                    printf("second p %d : ", spai);
//                    printDirectionsArray(spai, second_possibilities);
//                    printf("\n");

                    //remove the reverse of current direction
                    if(shiftLeftIntArray(cai, commons, linearIntArraySearch(cai, commons, reverseDirection(ghosts[i].movement)))) cai--;
//                    else bugAlert("reverse of current direction not removed!\n");

//                    printf("commons %d : ", cai);
//                    printDirectionsArray(cai, commons);
//                    printf("\n");
//                    printf("current movement : ");
//                    printDirection(ghosts[i].movement);
//                    printf("\n");
//                    printf("reverse of current movement : ");
//                    printDirection(reverseDirection(ghosts[i].movement));
//                    printf("\n");
//                    Sleep(1000);

                    if(cai != 0) {

                        ghosts[i].movement = commons[0];

                        //if ghosts are not side by side reach that else normal move
                        if(!areSideBySideLocations(ghosts[i].locations[1], ghosts[i].locations[0], 0)) { //are not side by side

//                            bugAlert("are not sbs\n");

                            Location next = ghosts[i].locations[0];
                            nextLocation(&next, ghosts[i].movement);

                            //check if now side by side
                            if(!areSideBySideLocations(ghosts[i].locations[1], next, 0)) { //are not side by side

                                next = ghosts[i].locations[1];
                                nextLocation(&next, ghosts[i].movement);

                                //check if now side by side
                                if(!areSideBySideLocations(next, ghosts[i].locations[0], 0)) { //stuck

                                    ///wtf

                                } else { //execute

                                    ghosts[i].locations[1] = next;
                                    view_validation = 0;

                                }

                            } else { //are side by side; execute the move

                                ghosts[i].locations[0] = next;
                                view_validation = 0;

                            }

                        } else { //are side by side; do a normal move

                            moveGhost(i);

                        }

                    } else { //no commons after removing reverse direction of current moving direction

                        //find a direction based on being side by side but not reverse of current direction
                        for(j = 0; j < 8; j++) {

                            if((directions[j] & VERTICAL && directions[j] & HORIZONTAL) || directions[j] == reverseDirection(ghosts[i].movement)) continue; //skip 2D directions and reverse direction of current direction

                            Location next = ghosts[i].locations[0];
                            nextLocation(&next, directions[j]);

                            if(isBrickThere(next)) { //check to be side by side

//                                printf("sbs = ");
//                                printDirection(sbs);
//                                printf("\n");

                                if(!areSideBySideLocations(ghosts[i].locations[1], next, 0)) continue; //not side by side
                                else { //found it; execute the move

//                                    printf("found it : ");
//                                    printDirection(directions[j]);
//                                    printf("\n");
//                                    Sleep(1000);

                                    ghosts[i].movement = directions[j];

                                    moveGhost(i);

                                    break;

                                }

                            }

                        }

                    }

                } else { //stuck

//                    printf("stuckio!\n");

                    //find a direction based on being side by side but not reverse of current direction
                    for(j = 0; j < 8; j++) {

                        if((directions[j] & VERTICAL && directions[j] & HORIZONTAL) || directions[j] == reverseDirection(ghosts[i].movement)) continue; //skip 2D directions and reverse direction of current direction

                        Location next = ghosts[i].locations[0];
                        nextLocation(&next, directions[j]);

                        if(isBrickThere(next)) {

//                            printf("sbs = ");
//                            printDirection(sbs);
//                            printf("\n");

                            if(!areSideBySideLocations(ghosts[i].locations[1], next, 0)) continue; //not side by side
                            else { //found it; execute the move

//                                printf("found it : ");
//                                printDirection(directions[j]);
//                                printf("\n");
//                                Sleep(1000);

                                ghosts[i].movement = directions[j];

                                moveGhost(i);

                                break;

                            }

                        }

                    }

                }

            }

            break;
        }

        case BRICK_ZONE_GHOST : {

            ///

            break;
        }

    }

}

int isBrickThere(Location p) {

    //check location to be in bounds
    if(p.x < 0 || p.x > ROWS - 1 || p.y < 0 || p.y > COLUMNS - 1) return 0; //out of bound location

    return bricks[p.x][p.y] < 0;

}

int canPacManMove(int direction) {

    if(direction & HORIZONTAL) {

        if(direction & LEFT) return pm.location.y - 1 >= 0; //left
        else return pm.location.y + 1 < COLUMNS; //right

    }

    if(direction & VERTICAL) {

        if(direction & UP) return pm.location.x - 1 >= 0; //up
        else return pm.location.x + 1 < ROWS; //down

    }

    return SHOULD_NOT_REACH_HERE;

}

void updateZones() {

    int current_color = 0;
    int current_color_use_time = 0;

    //replace pac man trace with bricks
    Replace2DintArray(ROWS, COLUMNS, bricks, -2, -1);

    int i;
    for(i = 0; i < ROWS; i++) {

        int j;
        for(j = 0; j < COLUMNS; j++) {

            if(bricks[i][j] == -1) {

                if(current_color_use_time > 0) current_color++;

            } else {

                bricks[i][j] = current_color;
                current_color_use_time++;

            }

        }

    }

    for(i = 0; i < ROWS; i++) {

        int j;
        for(j = 0; j < COLUMNS; j++) {

            if(bricks[i][j] != -1) {

                //check neighbors
                Location l;

                int k;
                for(k = 0; k < 8; k++) {

                    //reset
                    l.x = i;
                    l.y = j;

                    nextLocation(&l, directions[k]);

                    if(bricks[l.x][l.y] == -1) continue;
                    else { //a zone number

                        int o;
                        for(o = 0; o < ROWS; o++) {

                            int p;
                            for(p = 0; p < COLUMNS; p++) {

                                if(bricks[o][p] == -1) continue;
                                else if(bricks[o][p] == bricks[i][j]) bricks[o][p] = bricks[l.x][l.y];

                            }

                        }

                    }

                }

            }

        }

    }

}

void tryCaptureZones() {

    int ghosts_zones[float_ghosts * FLOAT_GHOST_LOCATIONSS + rail_ghosts * RAIL_GHOST_LOCATIONSS + brick_zone_ghosts * BRICK_ZONE_GHOST_LOCATIONSS];

    int i;
    for(i = 0; i < ghosts_quantity; i++) {

        int j;
        for(j = 0; j < ghosts[i].locationss; j++) {

            ghosts_zones[i] = bricks[ghosts[i].locations[j].x][ghosts[i].locations[j].y];

        }

    }

    for(i = 0; i < ROWS; i++) {

        int j;
        for(j = 0; j < COLUMNS; j++) {

            if(bricks[i][j] == -1) continue;
            else if(linearIntArraySearch(ghosts_quantity, ghosts_zones, bricks[i][j]) == -1) {

                Replace2DintArray(ROWS, COLUMNS, bricks, bricks[i][j], -1);

            }

        }

    }

}

void updateProgress() {

    int bricks_count = 0;

    int i;
    for(i = 0; i < ROWS; i++) {

        int j;
        for(j = 0; j < COLUMNS; j++) {

            if(bricks[i][j] == -1) bricks_count++;

        }

    }

    static int all_bricks_count = (ROWS - 2) * (COLUMNS - 2);

    progress = ((float) bricks_count - (2 * (ROWS + COLUMNS))) / (float) all_bricks_count * 100;
    view_validation = 0;

}

int movePacMan(int direction, int auto_call) {

    if(!canPacManMove(direction)) return 0;

    if(!auto_call && pm.movement != -1) {

        if(reverseDirection(pm.movement) != direction) pm.movement = direction;

        return 0;

    }

    nextLocation(&pm.location, direction);

    if(bricks[pm.location.x][pm.location.y] != -1) { //stepped out of the safe zone, run!

        bricks[pm.location.x][pm.location.y] = -2;

        pm.movement = direction;

    } else { //reached the end or moving on the safe zone

        if(pm.movement != -1) { //reached the end

            updateZones();
            tryCaptureZones();
            updateProgress();
            pm.movement = -1;

        }

    }

    view_validation = 0;

    return 1;

}

int arrowToDirection(char a) {

    switch(a) {

        case UAKCC : return VERTICAL | UP;
        case DAKCC : return VERTICAL | !UP;
        case LAKCC : return HORIZONTAL | LEFT;
        case RAKCC : return HORIZONTAL | !LEFT;

    }

    return SHOULD_NOT_REACH_HERE;

}

int main(){

    srand(time(NULL)); //for absolute random
    system("chcp 437"); //support ascii characters
    cls(); //clear Active Code Page : 437

    init();

    drawGame();

    while(1) {

        if(_kbhit()) {

            char c = _getch();

            switch(c) { //key press handler

                case AKCCM : { //arrow key

                    movePacMan(arrowToDirection(_getch()), 0);

                    break;
                }

                case SKCC : {

                    bricks[pm.location.x][pm.location.y] = 0;
                    updateZones();
                    tryCaptureZones();

                }

                //default : printf("%d\t", c);

            }

        }

        purgePressedKeys();
        controlAutoMoves();
        drawGame();
        Sleep(1000/REFRESH_RATE);
    }

    return 0;

}
