#include "game.h"
#include "constants.h"

//All game code goes here

int p1 = 0;
int p2 = 25;
int bx = 10;
int by = 10;

void game_init() {

}

void game_update() {
    p1 = p1 > 25 ? 0  : p1 + 1;
    p2 = p2 == 0 ? 25 : p2 - 1;
}

