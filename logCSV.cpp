#include <M5Stack.h>
#include<SD.h>
#include "logCSV.h"


logCSV::logCSV(){
  
}

void logCSV::setAHRS(float pitch, float roll ,float yaw){
  _pitch=pitch;
  _roll=roll;
  _yaw=yaw;
}
void logCSV::setG(float accelG, float sideG){
  _accelG=accelG;
  _sideG=sideG;
}
void logCSV::setGPS(float latitude, float longitude){
  _latitude = latitude;
  _longitude=longitude; 
}
void logCSV::setTime(int years,int months,int days,int hours,int minutes,int seconds){
  _years=years;
  _months=months;
  _days = days;
  _hours=hours;
  _minutes=minutes;
  _seconds=seconds;
  
}

void logCSV::writeCSV(){
  char filename[100];
  if (_years) {
      sprintf(filename, "/%04d-%02d-%02d-%02d:00.csv"
              ,_years,_months,_days,_hours);
    } else {
      sprintf(filename, "/log.csv");
    }
  //timestamp,minutes,seconds,latitude,logitude,pitch,roll,yaw,accelG,sideG
  String csv=String(millis())+","+String(_minutes)+","+String(_seconds)+","
  +","+String(_latitude)+","+String(_longitude)
  +String(_pitch)+","+String(_roll)+","+String(_yaw)
  +","+String(_accelG)+","+String(_sideG);

  f = SD.open(filename, FILE_APPEND);
    if (f) {
      f.println(csv);
    }
    f.close();
}

void logCSV::setInterval(int interval){
  _interval=interval;
}
int logCSV::getInterval(){
  //10 10hz 1000 0.1hz
  return _interval;
}
