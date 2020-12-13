#ifndef Dashboard_h
  #define Dashboard_h
#include<M5Stack.h>

class Dashboard{
  public:
    Dashboard();
    void createMeter(uint8_t x,uint8_t y,uint8_t r);
    void updateMeter(float val);

    void createVBar();
    void updateVBar();
  private:
    uint8_t _x;
    uint8_t _y;
    uint8_t _r;
    float _val;
};
#endif
