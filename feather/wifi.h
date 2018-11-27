enum WIFI_STATE { WIFI_WAIT, WIFI_SOCKETED };

extern WIFI_STATE wifi_state;

void wifi_init();

void wifi_check_client();

void wifi_push_data(int p1, int p2, int bx, int by, int p1Score, int p2Score);
