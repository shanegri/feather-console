#include "wifi.h"
#include "secrets.h"
#include <WiFi101.h>
#include "client.h"
#include "constants.h"
#include <stdint.h>
#include <LiquidCrystal.h>

/*
Connecting to wifi:
    Create a file called secrets.s that contain this:
    #define SECRET_SSID "WIFI NAME"
    #define SECRET_PASS "WIFI PASS"
*/
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);


WIFI_STATE wifi_state = WIFI_WAIT;

volatile uint8_t wifi_status = WL_IDLE_STATUS;

WiFiServer server(80);

void lcdPrintIP() {
    IPAddress ip = WiFi.localIP();
    
    lcd.setCursor(0,0);
    lcd.clear();

    lcd.write(SECRET_SSID);

    String localIP = String(ip[0]) + "." +String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);

    lcd.setCursor(0,1);
    lcd.write(localIP.c_str());
}


void wifi_init() {
    WiFi.setPins(8,7,4,2);

    lcd.begin(16, 2);
    lcd.setCursor(0,0);


    //Attempt to connect to WiFi
    while( wifi_status != WL_CONNECTED ) {
        lcd.setCursor(0,0);
        lcd.write("Connecting.  ");
        lcd.setCursor(0,1);
        lcd.write(SECRET_SSID);
        wifi_status = WiFi.begin(SECRET_SSID, SECRET_PASS);
        delay(200);
        lcd.setCursor(0,0);
        lcd.write("Connecting.. ");
        delay(200);
        lcd.setCursor(0,0);
        lcd.write("Connecting...");
        delay(200);
        lcd.setCursor(0,0);
        lcd.write("Connecting.. ");
        delay(200);
    }

    lcdPrintIP();


    server.begin();
}




WiFiClient client_connected;

//Push data to the client event stream
void wifi_push_data(int p1, int p2, int bx, int by, int p1Score, int p2Score) {
    if( client_connected && client_connected.connected() ) {
        client_connected.print("data: [");
        client_connected.print(p1);
        client_connected.print(",");
        client_connected.print(p2);
        client_connected.print(",");
        client_connected.print(bx);   
        client_connected.print(",");     
        client_connected.print(by);
        client_connected.print(",");     
        client_connected.print(p1Score);
        client_connected.print(",");     
        client_connected.print(p2Score);
        client_connected.println("]");
        client_connected.println();
        client_connected.flush();    
        return;
    }
    
    //Client has disconnected
    lcdPrintIP();
    if( client_connected ) client_connected.stop();
    wifi_state = WIFI_WAIT;
}


void serverSentEventHeader(WiFiClient client) {
    lcd.setCursor(0,0);
    lcd.clear();
    lcd.write("Game In Progress..");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/event-stream;charset=UTF-8");
    client.println("Connection: close");  
    
    //Remove this line when files are served from feather
    client.println("Access-Control-Allow-Origin: *");  
    
    client.println("Cache-Control: no-cache");  
    client.println();
    client.flush();
}

//Game Url
void sendClientGame(WiFiClient client) {
	client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML><html><body><script>");
    client.print("var TESTING = false;");
    client.print("var B_WIDTH = ");
    client.print(B_WIDTH);
    client.print(";var B_HEIGHT = ");
    client.print(B_HEIGHT);
    client.print(";var P_HEIGHT = ");
    client.print(P_HEIGHT);
    client.print(";var P_OFFSET = ");
    client.print(P_OFFSET);
    client.print(";</script>");
    client.println(CLIENT_JS);
    client.println("</body></html>");
}

//Invalid url
void sendClientError(WiFiClient client) {
	client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML><html>Error</html>");
}

//Returns url client wants to access
String waitForClientData(WiFiClient client) {
  bool prevLineBlank = false;
  bool readPage = false;
  int count = 0;
  
  String page = "";

  while( client.connected() ) {
    if( client.available() ) {
    	char c = client.read();
    	
		if(!readPage && count < 4) {
			count++;
		} else if (readPage) {
			if(c == 32) {
				readPage = false;
			} else {
				page += c;
			}
		} else if (count == 4) {
			count++;
			readPage = true;	
		}

		Serial.write(c);

		if (c == '\n' && prevLineBlank) {
			return page;
		}
		
		if( c == '\n' ) {
			prevLineBlank = true;
		} else if ( c != '\r') {
			prevLineBlank = false;
		}

    }
  }
  return "";
}


//Handle client request
//If request is 'socket' goto WIFI_SOCKETED state, client remains connected
void wifi_check_client() { 
  WiFiClient client = server.available();
  
  if(client) {

	String page = waitForClientData(client);

	Serial.println("New client connected for page: " + page);

	if(page == "socket") {
        serverSentEventHeader(client);
        wifi_state = WIFI_SOCKETED;
        client_connected = client;
        return;
	} else if ( page == "game") {
		sendClientGame(client);
	} else {
		sendClientError(client);
	}
    
    client.stop();
    Serial.println("Client disconnected");
  }
}
