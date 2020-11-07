#include<M5Stack.h>
#include <TinyGPS++.h>

#define TINY_GSM_MODEM_UBLOX
#include <TinyGsmClient.h>

//GPS
TinyGPSPlus gps;
HardwareSerial GPSserial(1);

//3G
TinyGsm modem(Serial2);
TinyGsmClient ctx(modem);

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  
  GPSserial.begin(9600,SERIAL_8N1,13,5);
  Serial2.begin(115200,SERIAL_8N1,16,17);
  
  modem.restart();
  M5.Lcd.setCursor(0,0);
  //M5.Lcd.print(modem.getModemInfo());
  //while(!modem.waitForNetwork());
  modem.gprsConnect("soracom.io","sora","sora");
  //while(!modem.isNetworkConnected());
  
  M5.Lcd.setTextSize(2);
}

void loop() {
  M5.Lcd.setCursor(0,0);
  // put your main code here, to run repeatedly:
  //M5.Lcd.print(gps.satellites.value());
  if(modem.isNetworkConnected()){
    M5.Lcd.setCursor(0,0);
    M5.Lcd.print("3G");
  }
  if(gps.location.isUpdated()){  
    M5.Lcd.setCursor(0,40);
    M5.Lcd.print("GPS");
  }
}
