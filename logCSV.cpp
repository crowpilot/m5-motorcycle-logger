#include <M5Stack.h>
#include<SD.h>
#include "logCSV.h"


logCSV::logCSV() {

}

void logCSV::setAHRS(float pitch, float roll , float yaw) {
  _pitch = pitch;
  _roll = roll;
  _yaw = yaw;
}
void logCSV::setG(float accelG, float sideG) {
  _accelG = accelG;
  _sideG = sideG;
}
void logCSV::setGPS(float latitude, float longitude) {
  _latitude = latitude;
  _longitude = longitude;
}
void logCSV::setTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {
  _years = years;
  _months = months;
  _days = days;
  _hours = hours;
  _minutes = minutes;
  _seconds = seconds;

}

void logCSV::writeCSV() {
  //char filename[100];
  String filename;
  bool header = false;

  //sprintf(filename, "/log/%04u-%02u-%02u-%02u:00.csv",_years,_months,_days,_hours);
  filename = "/log/"+String(_years) + "-" + String(_months) + "-" + String(_days) + "_" + String(_hours)+".csv";

  //M5.Lcd.setCursor(0,0);
  //M5.Lcd.print(_hours);
  if (!SD.exists(filename.c_str())) {
    header = true;
  }
  //timestamp,minutes,seconds,latitude,logitude,pitch,roll,yaw,accelG,sideG
  String csv = String(millis()) + "," + String(_minutes) + "," + String(_seconds) + ","
               + String(_latitude) + "," + String(_longitude) + ","
               + String(_pitch) + "," + String(_roll) + "," + String(_yaw) + ","
               + String(_accelG) + "," + String(_sideG);

  f = SD.open(filename.c_str(), FILE_APPEND);
  if (f) {
    if (header) {
      f.println("timestamp,minutes,seconds,latitude,longitude,pitch,roll,yaw,accelG,sideG");
    }
    f.println(csv);
  }
  f.close();
}

void logCSV::setInterval(int interval) {
  _interval = interval;
}
int logCSV::getInterval() {
  //10 10hz 1000 0.1hz
  return _interval;
}
