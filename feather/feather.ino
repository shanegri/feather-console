#include "wifi.h"
#include "game.h"

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Serial.println("Serial Monitor Connected");

  wifi_init();
}

void loop() {
  switch (wifi_state) {
    case WIFI_WAIT:
      
      //Waiting for a client to connect

      wifi_check_client();
      break;
    case WIFI_SOCKETED:

      //This is where game update code will be called
      //Update data will be passed to wifi_push_data 
      //causing the client to redraw the game

      wifi_push_data();
      break;
  }
}

