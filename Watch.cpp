#include<M5Stack.h>
#include "Watch.h"

Watch::Watch() {

}

void Watch::init() {

}

void Watch::startLap() {
  _startMillis = millis();
}

unsigned long Watch::lapTime() {
  return millis() - _startMillis;
}

void Watch::clockAdjust(int hours, int minutes, int seconds) {
  _clockMillis = (hours * 3600 + minutes * 60 + seconds) * 1000;
}

void Watch::refreshClock() {
  // 60 1000
  unsigned now = (millis() + _clockMillis) / 1000;
  _seconds = now % 60;
  _minutes = now / 60 % 60;
  _hours = now / 3600 % 24;
}
int Watch::getHours(){
  return _hours;
}
int Watch::getMinutes(){
  return _minutes;
}
int Watch::getSeconds(){
  return _seconds;
}
