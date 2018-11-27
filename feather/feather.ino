#include "wifi.h"
#include "game.h"

#define LEFT_P1_PIN 13
#define RIGHT_P1_PIN 12
#define LEFT_P2_PIN 11
#define RIGHT_P2_PIN 10


void p1UpInt(){
  p1Up = digitalRead(LEFT_P1_PIN)? true:false;
}
void p1DownInt(){
  p1Down = digitalRead(RIGHT_P1_PIN)? true:false;
}
void p2UpInt(){
  p2Up = digitalRead(LEFT_P2_PIN)? true:false;
}
void p2DownInt(){
  p2Down = digitalRead(RIGHT_P2_PIN)? true:false;
}
void setup() {
  Serial.begin(9600);
  while(!Serial);

  Serial.println("Serial Monitor Connected");
  
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

      wifi_check_client();
      break;
    case WIFI_SOCKETED:

      game_update();

      wifi_push_data(p1, p2, bx, by);
      break;
  }
}
