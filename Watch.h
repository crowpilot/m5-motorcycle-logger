#ifndef Watch_h
#define Watch_h
#include<M5Stack.h>

class Watch {
  public:
    Watch();
    void init();
    void startLap();
    unsigned long lapTime();

    void clockAdjust(int hours, int minutes, int seconds);
    void refreshClock();
    int getHours();
    int getMinutes();
    int getSeconds();

  private:
    unsigned long _startMillis;
    unsigned long _clockMillis;
    int _hours;
    int _minutes;
    int _seconds;
};
#endif
