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
