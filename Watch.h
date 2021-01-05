//Clock and Laptimer(stop watch)
//_mode==0 clock
//_mode==1 or 2 Laptimer

#ifndef Watch_h
#define Watch_h
#include<M5Stack.h>

class Watch {
  public:
    Watch(int x,int y,int w,int h);
    void init();//w h not support

    void displayWatch();

    bool isClock();
    void toggleLap();
    String getButton();

    void clockMode();
    void lapMode();
    
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
    uint8_t _mode=0;//0:clock 1:lap(stop) 2:lap(started)
};
#endif
