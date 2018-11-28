enum WIFI_STATE { WIFI_WAIT, WIFI_SOCKETED };

extern WIFI_STATE wifi_state;

//Initialize WiFi connection and server
void wifi_init();

//Check if a new client has connected and handle
void wifi_check_client();

//Push new data to the server-event stream
void wifi_push_data(int p1, int p2, int bx, int by, int p1Score, int p2Score);
