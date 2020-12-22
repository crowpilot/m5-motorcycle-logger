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

void Watch::startLap() {
  _startMillis = millis();
}

void Watch::stopLap(){
  _lastLap=millis()-_startMillis+_lastLap;
  _startMillis=0;
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
