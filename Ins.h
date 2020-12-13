#ifndef Ins_h
#define Ins_h
#define M5STACK_MPU6886
#include<M5Stack.h>
#include <Wire.h>
//#include<MadgwickAHRS.h>

#include "Preferences.h"
#include "math.h"
#include "bmm150.h"
#include "bmm150_defs.h"

#define sampleFreq  48.0f     // sample frequency in Hz
#define twoKpDef  (2.0f * 1.0f) // 2 * proportional gain
#define twoKiDef  (2.0f * 0.0f) // 2 * integral gain

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

    static int8_t i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len);
    static int8_t i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len);
    int8_t bmm150_initialization();
    void bmm150_offset_save();
    void bmm150_offset_load();
    void bmm150_calibrate(uint32_t calibrate_time);

    void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, float *pitch, float *roll, float *yaw);
    float invSqrt(float x);

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

    float _temp = 0.0F;

    unsigned long _timing;

    struct bmm150_dev _dev;
    bmm150_mag_data _mag_offset;
    bmm150_mag_data _mag_max;
    bmm150_mag_data _mag_min;
    Preferences _prefs;

    volatile float twoKp = twoKpDef;                      // 2 * proportional gain (Kp)
    volatile float twoKi = twoKiDef;                      // 2 * integral gain (Ki)
    volatile float q0 = 1.0, q1 = 0.0, q2 = 0.0, q3 = 0.0;          // quaternion of sensor frame relative to auxiliary frame
    volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;

    //Madgwick MadgwickFilter;
};

#endif
