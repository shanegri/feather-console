#include "game.h"
#include "constants.h"

//All game code goes here

int p1 = 0;
int p2 = 25;
int bx = 10;
int by = 10;

bool p1Up;
bool p2Up;
bool p1Down;
bool p2Down;

//not going to use dis
enum BTN_STATE {PRESSED = 0, RELEASED };

volatile BTN_STATE ctr1_btn1 = RELEASED;
volatile BTN_STATE ctr11_btn2 = RELEASED;
volatile BTN_STATE ctr12_btn1 = RELEASED;
volatile BTN_STATE ctrl2_btn2 = RELEASED;

const int movement_const = 1;
void update_movement(){
  if(p1Up){
    p1 = p1 - movement_const;
  }
  if(p1Down){
    p1 = p1 +movement_const;
  }
  if(p2Up){
    p2 = p2-movement_const;
  }
  if(p2Down){
    p2 = p2 + movement_const;
  }
}

void game_init() {
  p1 = 0;
  p2 = 2;
}

void game_update() {
    update_movement();
}
