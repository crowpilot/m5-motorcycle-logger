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
//  M5.Lcd.setTextFont(2); not good
}

int getTime(uint8_t mode=0){
  return 0;
}

//mode 0:clock 1:laptimer 2:laptimer running
void loop() {
  static uint8_t mode=0;
  //header 3G connection GPS
  //M5.Lcd.print(gps.satellites.value());
  if(modem.isNetworkConnected()){
    M5.Lcd.setCursor(0,0);
    M5.Lcd.print("3G");
  }
  if(gps.location.isUpdated()){  
    M5.Lcd.setCursor(0,60);
    M5.Lcd.print("GPS");
  }
  M5.Lcd.drawFastHLine(0,20,320,TFT_GREEN);
  M5.Lcd.drawFastHLine(0,40,320,TFT_GREEN);

  //Clock / laptime

  M5.Lcd.setCursor(20,130);
  M5.Lcd.setTextSize(7);
  M5.Lcd.print("00:00");
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(".00");
  //bottom
  M5.Lcd.drawFastHLine(0,215,320,TFT_GREEN);
  M5.Lcd.setCursor(0,220);
  //M5.Lcd.print(F("    lap    clock"));
  M5.Lcd.setCursor(30,220);
  M5.Lcd.print("clock");
  if(mode==0){
    //clock
    M5.Lcd.drawRect(20,215,80,25,TFT_GREEN);
    M5.Lcd.setCursor(120,220);
    M5.Lcd.print("  LAP  ");
  }
  else if(mode==1){
    //timer
    M5.Lcd.drawRect(20,216,80,24,TFT_BLACK);
    //M5.Lcd.fillRect(125,216,80,24,TFT_BLACK);
    M5.Lcd.setCursor(125,220);
    M5.Lcd.print("START");
    M5.Lcd.setCursor(220,220);
    M5.Lcd.print("RESET");
  }
  
  //button setting
  //lap clock 
  M5.update();
  if(M5.BtnA.wasPressed()){
    //lap
    mode=0;
  }
  if(M5.BtnB.wasPressed()){
    mode=1;
  }
}
