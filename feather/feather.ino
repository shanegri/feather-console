#include "wifi.h"
#include "game.h"

#define LEFT_P1_PIN 13
#define RIGHT_P1_PIN 12
#define LEFT_P2_PIN 11
#define RIGHT_P2_PIN 10


void p1UpInt(){
  if(digitalRead(LEFT_P1_PIN)){
    button_status |= (1 << 3);
  }else{
    button_status &= ~(1 << 3); 
  }
  
}
void p1DownInt(){
  if(digitalRead(RIGHT_P1_PIN)){
    button_status |= (1 << 1);
  }else{
    button_status &= ~(1 << 1); 
  }
}
void p2UpInt(){
  if(digitalRead(LEFT_P2_PIN)){
    button_status |= (1 << 2);
  }else{
    button_status &= ~(1 << 2); 
  }
}
void p2DownInt(){
  if(digitalRead(RIGHT_P2_PIN)){
    button_status |= (1 << 0);
  }else{
    button_status &= ~(1 << 0); 
  }
}
void setup() {
  
  attachInterrupt(digitalPinToInterrupt(LEFT_P1_PIN), p1UpInt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_P1_PIN), p1DownInt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_P2_PIN), p2UpInt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_P2_PIN), p2DownInt, CHANGE);
  
  wifi_init();
  game_init();

}


void loop() {
  switch (wifi_state) {
    case WIFI_WAIT:
      
      //Waiting for a client to connect
      game_init();
      wifi_check_client();
      break;
    case WIFI_SOCKETED:

      game_update();

      delay(10);

      wifi_push_data(p1, p2, bx, by,score1,score2);
      break;
  }
}
