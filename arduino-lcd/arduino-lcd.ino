#include <LiquidCrystal.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(3, 4); // RX, TX
LiquidCrystal lcd(7, 8, 9, 10, 11 , 12);

void setup() { 

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.write("192.198.1.172");
  lcd.setCursor(0,1);
  lcd.write("/game");
}

void loop() {  
  if (mySerial.available()) {
    lcd.write(mySerial.read());
  }
}
