#include <stdint.h>
extern int p1;
extern int p2;
extern int bx;
extern int by;
extern int score1;
extern int score2;

extern uint8_t button_status;

//Reset game 
void game_init();

//Update to next game frame
void game_update();
