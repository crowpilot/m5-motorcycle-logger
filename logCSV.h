#ifndef logCSV_h
  #define logCSV_h
#include<M5Stack.h>

class logCSV{
  public:
    logCSV();
    void setAHRS(float pitch,float roll ,float yaw);
    void setG(float accelG=0,float sideG=0);
    void setGPS(float latitude=0,float longitude=0);
    void setTime(uint16_t years,uint8_t months,uint8_t days,uint8_t hours,uint8_t minutes,uint8_t seconds);
    //void setTime();

    void writeCSV();

    void setInterval(int interval);
    int getInterval();
  private:
    float _pitch;
    float _roll;
    float _yaw;
    float _accelG;
    float _sideG;
    int _interval=100;
    float _latitude;
    float _longitude;

    uint16_t _years;
    uint8_t _months;
    uint8_t _days;
    uint8_t _hours;
    uint8_t _minutes;
    uint8_t _seconds;
    File f;
    //int rpm
    
};
#endif
