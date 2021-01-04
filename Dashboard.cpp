#include<M5Stack.h>
#include "Dashboard.h"

Dashboard::Dashboard(){
  
}

void Dashboard::createMeter(uint8_t x,uint8_t y,uint8_t r){
  //top 0
}

void Dashboard::updateMeter(float val){
  //150 + 60 * sin(ins.roll() * 6.28 / 360), 148 - 60 * cos(ins.roll() * 6.28 / 360
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
