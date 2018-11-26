#include "game.h"
#include "constants.h"

//All game code goes here

int p1 = 0;
int p2 = 25;
int bx = 10;
int by = 10;

enum BTN_STATE {PRESSED = 0, RELEASED };

volatile BTN_STATE ctr11_btn1 = RELEASED;
volatile BTN_STATE ctr11_btn2 = RELEASED;
volatile BTN_STATE ctr12_btn1 = RELEASED;
volatile BTN_STATE ctrl2_btn2 = RELEASED;

void game_init() {

}

void game_update() {
    p1 = p1 > 25 ? 0  : p1 + 1;
    p2 = p2 == 0 ? 25 : p2 - 1;
}
