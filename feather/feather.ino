#include "wifi.h"
#include "game.h"

#define LEFT_P1_PIN 13
#define RIGHT_P1_PIN 12
#define LEFT_P2_PIN 11
#define RIGHT_P2_PIN 10

//The pin change interrupt function for the following pins
void p1UpInt(){
  //flips the bit in button status
  if(digitalRead(LEFT_P1_PIN)){
    button_status |= (1 << 3);
  }else{
    button_status &= ~(1 << 3); 
  }
  
}
//same pattern as before
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
  //this attaches the interrupts with the pins
  attachInterrupt(digitalPinToInterrupt(LEFT_P1_PIN), p1UpInt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_P1_PIN), p1DownInt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_P2_PIN), p2UpInt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_P2_PIN), p2DownInt, CHANGE);
  
  wifi_init();
  game_init();

}


void loop() {
  switch (wifi_state) {
    //Waiting for a client to connect
    case WIFI_WAIT:
      
      //Game is perpetually reset to inital state if no client
      game_init();
      
      //Check if new client is connected and handle appropriately   
      wifi_check_client();
      break;
    
    //Client has connected and ready to recieve game updates
    case WIFI_SOCKETED:

      //Update to next game frame
      game_update();

      //Give client time to receive previous data
      delay(10);

      //Push game updates to stream
      wifi_push_data(p1, p2, bx, by,score1,score2);
      break;
  }
}
