#include<M5Stack.h>
#include "Dashboard.h"

Dashboard::Dashboard(){
  
}

void Dashboard::init(){
  M5.Lcd.drawFastHLine(0, 20, 320, TFT_GREEN);
  M5.Lcd.drawFastVLine(100, 20, 50, TFT_GREEN);
  M5.Lcd.drawFastVLine(200, 20, 50, TFT_GREEN);
  M5.Lcd.drawFastHLine(0, 39, 320, TFT_NAVY);
  M5.Lcd.drawFastHLine(0, 70, 320, TFT_GREEN);
  M5.Lcd.drawFastHLine(0, 152, 320, TFT_GREEN);
  M5.Lcd.drawFastHLine(0, 215, 320, TFT_GREEN);
}

void Dashboard::createMeter(uint8_t x,uint8_t y,uint8_t r){
  //top 0
  M5.Lcd.drawCircleHelper(150, 148, 70, 0x3, TFT_GREEN);
}

void Dashboard::updateMeter(float val){
  //150 + 60 * sin(ins.roll() * 6.28 / 360), 148 - 60 * cos(ins.roll() * 6.28 / 360
  M5.Lcd.drawLine(150, 148, 150 + 60 * sin(_val * 6.28 / 360), 148 - 60 * cos(_val * 6.28 / 360), TFT_BLACK);
  M5.Lcd.drawLine(150, 148, 150 + 60 * sin(val * 6.28 / 360), 148 - 60 * cos(val * 6.28 / 360), TFT_GREEN);
  _val=val;
}

void Dashboard::bottomButton(String left,String center,String right){
  //69,160,236
  M5.Lcd.fillRect(0,216,320,24,TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(70-left.length()*8,220);
  M5.Lcd.print(left);
  M5.Lcd.setCursor(160-center.length()*8,220);
  M5.Lcd.print(center);
  M5.Lcd.setCursor(250-right.length()*8,220);
  M5.Lcd.print(right);
}

void Dashboard::env(float temp,float hum,float pressure){
  M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 22);
    M5.Lcd.printf("%2.1fC", temp);
    M5.Lcd.setCursor(102, 22);
    M5.Lcd.printf("%2.1f%%", hum);
    M5.Lcd.setCursor(202, 22);
    M5.Lcd.printf("%2.2finHg", pressure);
}
