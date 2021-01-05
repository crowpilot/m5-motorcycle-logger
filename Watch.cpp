#include<M5Stack.h>
#include "Watch.h"

Watch::Watch(int x, int y, int w, int h) {
  _x = x;
  _y = y;
  _w=w;
  _h=h;
}

void Watch::init() {
  
}

void Watch::displayWatch(){
  if (_mode == 0) {
    //cloc kmode
    Watch::displayClock();
  }
  else if (_mode == 1 or _mode==2) {
    //Laptimer stop
    Watch::displayLap();
  } 
}

bool Watch::isClock(){
  if(_mode==0){
    return true;
  }else{
    return false;
  }
}

void Watch::toggleLap(){
  if(_mode==1){
    Watch::startLap();
  }else{
    Watch::stopLap();
  }
}

String Watch::getButton(){
  if(_mode==1){
    //stop mode
    return String("START");
  }else{
    return String("STOP");
  }
}

void Watch::clockMode(){
  _mode=0;
}

void Watch::lapMode(){
  if(_startMillis == 0){
    _mode=1;
  }else{
    _mode=2;
  }
}

void Watch::startLap() {
  _startMillis = millis();
  _mode=2;//lap started
}

void Watch::stopLap(){
  _lastLap=millis()-_startMillis+_lastLap;
  _startMillis=0;
  _mode=1;//lap stop
}

unsigned long Watch::lapTime() {
  if (_startMillis != 0) {
    return millis() - _startMillis+_lastLap;
  } else {
    return _lastLap;
  }
}

void Watch::resetLap() {
  _startMillis = 0;
  _lastLap=0;
  _mode=1;
  Watch::resetDisplay();
}

void Watch::displayLap() {
  M5.Lcd.setCursor(_x,_y);
  M5.Lcd.printf("%02lu:",(Watch::lapTime()/3600000)%24);
  M5.Lcd.setTextSize(7);
  M5.Lcd.printf("%02lu.%02lu",(Watch::lapTime()/60000)%60,Watch::lapTime()/1000%60);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf(".%lu",Watch::lapTime()/100%10);
}

void Watch::clockAdjust(int hours, int minutes, int seconds) {
  _clockMillis = (hours * 3600 + minutes * 60 + seconds) * 1000;
}

void Watch::displayClock() {
  // 60 1000
  Watch::refreshClock();
  M5.Lcd.setCursor(_x, _y);
  M5.Lcd.setTextSize(7);
  M5.Lcd.printf("%02d:%02d", _hours , _minutes);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf(".%02d", _seconds);
}
void Watch::refreshClock(){
  unsigned now = (millis() + _clockMillis) / 1000;
  _seconds = now % 60;
  _minutes = now / 60 % 60;
  _hours = (now / 3600 + 9) % 24;
}

int Watch::getHours() {
  return _hours;
}
int Watch::getMinutes() {
  return _minutes;
}
int Watch::getSeconds() {
  return _seconds;
}

void Watch::resetDisplay(){
  M5.Lcd.fillRect(_x,_y,_w,_h,TFT_BLACK);
}
