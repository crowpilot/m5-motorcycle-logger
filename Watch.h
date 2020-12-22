#ifndef Watch_h
#define Watch_h
#include<M5Stack.h>

class Watch {
  public:
    Watch(int x,int y,int w,int h);
    void init();//w h not support
    void startLap();
    void stopLap();
    unsigned long lapTime();
    void resetLap();
    void displayLap();

    void clockAdjust(int hours, int minutes, int seconds);
    void refreshClock();
    void displayClock();
    int getHours();
    int getMinutes();
    int getSeconds();

    void resetDisplay();

  private:
    unsigned long _startMillis=0;
    unsigned long _clockMillis=0;
    unsigned long _lastLap=0;
    int _hours;
    int _minutes;
    int _seconds;
    int _x,_y,_w,_h;
};
#endif
