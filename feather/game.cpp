#include "game.h"
#include "constants.h"
#include <stdint.h>

//All game code goes here

uint8_t p1 = 0;
uint8_t p2 = 25;
uint8_t bx = 10;
uint8_t by = 10;

int velocity_x = 65536; // (1 << 16)
int velocity_y = 65536;

int real_bx = bx * (1 << 16);
int real_by = by * (1 << 16);

uint8_t button_status = 0;
//would like an byte
//xxxx (p1Up)(p2Up)(p1Down)(p2Down)


void update_movement(){
  if(button_status & (1 << 3)){
    p1 = p1 - 1;
  }
  if(button_status & (1 << 1)){
    p1 = p1 +1;
  }
  if(button_status & (1 << 2)){
    p2 = p2-1;
  }
  if(button_status & 1){
    p2 = p2 +1;
  }
}


void update_ball_position(){
  real_bx = real_bx +velocity_x;
  real_by = real_by + velocity_y;
  bx = real_bx >> 16;
  by = real_by >> 16;
  
}

void update_ball_velocity(){
  if(by < 0 || by >30){
    velocity_y = - velocity_y;
  }
  
}
void game_init() {
  p1 = 0;
  p2 = 0;
}

void game_update() {
    update_movement();
}
