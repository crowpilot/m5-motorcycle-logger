#ifndef Ins_h
#define Ins_h
#define M5STACK_MPU6886
#include<M5Stack.h>

class Ins {
  public:
    Ins();
    void init();
    void reload();
    float pitch();
    float roll();
    float yaw();
    float forward_g();
    float side_g();
    float temp();

  private:
    float _pitch = 0.0F;
    float _roll  = 0.0F;
    float _yaw   = 0.0F;
    
    float _accX = 0.0F;
    float _accY = 0.0F;
    float _accZ = 0.0F;

    float _gyroX = 0.0F;
    float _gyroY = 0.0F;
    float _gyroZ = 0.0F;

    float _gyroXoff = 0.0F;
    float _gyroYoff = 0.0F;
    float _gyroZoff = 0.0F;

    float _temp=0.0F;

    
};

#endif
