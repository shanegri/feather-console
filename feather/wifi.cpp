#include "wifi.h"
#include "secrets.h"
#include <WiFi101.h>

WIFI_STATE wifi_state = WIFI_WAIT;

int wifi_status = WL_IDLE_STATUS;

WiFiServer server(80);

void wifi_init() {
    WiFi.setPins(8,7,4,2);

    //Attempt to connect to WiFi
    while( wifi_status != WL_CONNECTED ) {
        Serial.print("Attepting to connected to: ");
        Serial.println(SECRET_SSID);
        wifi_status = WiFi.begin(SECRET_SSID, SECRET_PASS);
        delay(1000);
    }

    Serial.println("WiFi Connected");
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    server.begin();
}


WiFiClient client_connected;

//Push data to the client event stream
void wifi_push_data() {
    if( client_connected && client_connected.connected() ) {
        client_connected.print("data: {\"count\": ");
        client_connected.print(0);
        client_connected.print(" } ");
        client_connected.println();
        client_connected.println();
        client_connected.flush();    
        return;
    }
    
    //Client has disconnected
    if( client_connected ) client_connected.stop();
    wifi_state = WIFI_WAIT;
}


void serverSentEventHeader(WiFiClient client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/event-stream;charset=UTF-8");
    client.println("Connection: close");  
    
    //Remove this line when files are served from feather
    client.println("Access-Control-Allow-Origin: *");  
    
    client.println("Cache-Control: no-cache");  
    client.println();
    client.flush();
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
		sendClientError(client);
	} else {
		sendClientError(client);
	}
    
    client.stop();
    Serial.println("Client disconnected");
  }
}
