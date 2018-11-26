#include "wifi.h"
#include "game.h"

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Serial.println("Serial Monitor Connected");

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

