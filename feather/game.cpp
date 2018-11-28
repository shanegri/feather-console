#include "game.h"
#include "constants.h"
#include <stdint.h>

//All game code goes here
//p1 is player 1's y axis paddle location, same with p2
int p1 = 0;
int p2 = 25;

//bx and by are the ball coordinates (0 to Box size)
int bx = 10;
int by = 10;

int score1 = 0;
int score2 = 0;

/*
 the velocities deal with the motion of the ball,
 can give us variable speeds and is scalable.
 the ball travels about the same speed as the paddles
 we have multiple velocities because representing a
 negative number in this form is difficult
 */
int pos_velocity_x = 1<<15; // (1 << 15)
int pos_velocity_y = 1<<15;
int neg_velocity_x = 1<<15; // (1 << 15)
int neg_velocity_y = 1<<15;// subtract negative velocities

//the velocity flag controls the direction of the velocity
uint8_t velocity_flag = 3; //(xxxx xx(x)(y)
//0 is neg x, neg y
//1 is neg x, pos y
//2 is pos x, neg y
//3 is pos x, pos y
int real_bx = bx * (1 << 16);
int real_by = by * (1 << 16);
//real_by is a number from 0 to (1<<32)-1,
//we can be more percise with large numbers
//compared to small numbers

uint8_t button_status = 0;
//recieves the data from the controllers
//xxxx (p1Up)(p2Up)(p1Down)(p2Down)

enum game_states {Initialize,P1_Throw, P2_Throw, Action } state;


//places the ball infront of a paddle, used for after a point is scored
void set_position_ball(bool is_p1){
  if(is_p1){
    bx = P_OFFSET + 2;
    by = p1 -(P_HEIGHT/2);
    velocity_flag = 3;
  }else{
    bx = B_HEIGHT- P_OFFSET - 2;
    by = p2 -(P_HEIGHT/2);
    velocity_flag = 0;
  }
  real_bx = bx* (1 << 16);
  real_by = by * (1 << 16);
}

//moves the paddles depending on button inputs
void update_movement(){
  if((button_status & (1 << 3)) && p1 >= 0){
    p1 = p1 - 1;
  }
  if((button_status & (1 << 1)) && p1 < B_HEIGHT - P_HEIGHT ){
    p1 = p1 +1;
  }
  if((button_status & (1 << 2)) && p2 >= 0){
    p2 = p2-1;
  }
  if((button_status & 1) && p2 < B_HEIGHT - P_HEIGHT){
    p2 = p2 +1;
  }
}

//moves the ball depending on the velocity
int tempx;
void update_ball_position(){
  switch(velocity_flag){
    case 0:
      real_bx = real_bx - neg_velocity_x;
      real_by = real_by - neg_velocity_y;
      break;
    case 1:
      real_bx = real_bx - neg_velocity_x;
      real_by = real_by + pos_velocity_y;
      break;
    case 2:
      real_bx = real_bx + pos_velocity_x;
      real_by = real_by - neg_velocity_y;
      break;
    case 3:
      real_bx = real_bx + pos_velocity_x;
      real_by = real_by + pos_velocity_y;
      break;
  }
  //saves the pixel before moving for bound checking with paddles
  //incase the ball moves too fast and goes through the paddle
  tempx = bx;
  bx = real_bx >> 16;
  by = real_by >> 16;
  //check deflection
  if(tempx >= P_OFFSET && bx <= P_OFFSET && by>= p1 && by <= p1+P_HEIGHT ){
    velocity_flag ^= 2;
    bx = tempx;
    bx = real_bx >> 16;
    return;
  }
  if(tempx <= B_WIDTH - P_OFFSET && bx >= B_WIDTH - P_OFFSET && by>= p2 && by <= p2+P_HEIGHT ){
    velocity_flag ^= 2;
    bx = tempx;
    bx = real_bx >> 16;
    return;
  }
  
}

//checks bounds and updates the velocity accordingly
//also sets the score
void update_ball_velocity(){
  if(by < 0 || by >B_HEIGHT){
    velocity_flag ^= 1;
  }
  if(bx < 0){
    score2 += 1;
    set_position_ball(true);
    state = P1_Throw;
  }
  if(bx > B_WIDTH){
    score1 +=1;
    set_position_ball(false);
    state - P2_Throw;
  }
  
}

//initializes the game
void game_init() {
  state = P1_Throw;
  p1 = 15;
  p2 = 15;
  score1 = 0;
  score2 = 0;
   
}

//the main game loop, gets called in feather.ino
void game_update() {
  switch(state){
    case Initialize:
      game_init();
      state = P1_Throw;
      break;
    case P1_Throw:
      set_position_ball(true);
      state = Action;
      break;
    case Action:
      // 99% of the time in this state
      update_movement();
      update_ball_velocity();
      update_ball_position();
      break;
    case P2_Throw:
      set_position_ball(false);
      state = Action;
      break;
      
  }
    
}
