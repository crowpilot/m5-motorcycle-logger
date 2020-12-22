#ifndef logCSV_h
  #define logCSV_h
#include<M5Stack.h>

class logCSV{
  public:
    logCSV();
    void setAHRS(float pitch,float roll ,float yaw);
    void setG(float accelG=0,float sideG=0);
    void setGPS(float latitude=0,float longitude=0);
    void setTime(int years,int months,int days,int hours,int minutes,int seconds);
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
    int _interval=1000;
    float _latitude;
    float _longitude;
    File f;

    int _years,_months,_days,_hours,_minutes,_seconds;
    //int rpm
    
};
#endif
