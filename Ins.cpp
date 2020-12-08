#define M5STACK_MPU6886
#include<M5Stack.h>
#include "Ins.h"
#include "utility/MahonyAHRS.h"

#define DEG_TO_RAD 0.017453292519943295769236907684886

Ins::Ins(){
}

void Ins::init(){
  M5.IMU.Init();
  M5.IMU.setGyroFsr(M5.IMU.GFS_250DPS);
  M5.IMU.setAccelFsr(M5.IMU.AFS_8G);
  M5.IMU.setGyroOffset(700, 1100, -300);
  for(int i=0;i<100;i++){
    M5.IMU.getGyroData(&_gyroX,&_gyroY,&_gyroZ);
    _gyroXoff+=_gyroX/100;
    _gyroYoff+=_gyroY/100;
    _gyroZoff+=_gyroZ/100;
    delay(10);
  }
}

void Ins::reload(){
  M5.IMU.getTempData(&_temp);
  M5.IMU.getGyroData(&_gyroX,&_gyroY,&_gyroZ);
  M5.IMU.getAccelData(&_accX,&_accY,&_accZ);
  MahonyAHRSupdateIMU((_gyroX-_gyroXoff) * DEG_TO_RAD*0.6, (_gyroY-_gyroYoff) * DEG_TO_RAD*0.6, (_gyroZ-_gyroZoff) * DEG_TO_RAD*0.6, _accX, _accY, _accZ, &_roll, &_pitch, &_yaw);
}

float Ins::pitch(){
  return _pitch;
}
float Ins::roll(){
  return _roll;
}
float Ins::yaw(){
  return _yaw;
}

float Ins::forward_g(){
  return -_accY*cos(_pitch*DEG_TO_RAD)+_accZ*sin(_pitch*DEG_TO_RAD);
}

float Ins::temp(){
  return _temp;
}
