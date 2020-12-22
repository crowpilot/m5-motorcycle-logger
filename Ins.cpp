#define M5STACK_MPU6886
#include<M5Stack.h>
#include "Ins.h"
#include "utility/MahonyAHRS.h"

#define DEG_TO_RAD 0.017453292519943295769236907684886

Ins::Ins() {
}

void Ins::init() {
  M5.IMU.Init();
  M5.IMU.setGyroFsr(M5.IMU.GFS_250DPS);
  M5.IMU.setAccelFsr(M5.IMU.AFS_8G);
  M5.IMU.setGyroOffset(700, 1100, -300);
  //MadgwickFilter.begin(50);
  for (int i = 0; i < 100; i++) {
    M5.IMU.getGyroData(&_gyroX, &_gyroY, &_gyroZ);
    _gyroXoff += _gyroX / 100;
    _gyroYoff += _gyroY / 100;
    _gyroZoff += _gyroZ / 100;
    //bmm150_read_mag_data(&_dev);
    delay(10);
  }
  if (bmm150_initialization() != BMM150_OK)
  {
  }
  bmm150_offset_load();

}

void Ins::reload() {
  M5.IMU.getTempData(&_temp);
  M5.IMU.getGyroData(&_gyroX, &_gyroY, &_gyroZ);
  M5.IMU.getAccelData(&_accX, &_accY, &_accZ);
  bmm150_read_mag_data(&_dev);
  Ins::MahonyAHRSupdate((_gyroX - _gyroXoff) * DEG_TO_RAD , (_gyroY - _gyroYoff) * DEG_TO_RAD , (_gyroZ - _gyroZoff) * DEG_TO_RAD ,
                        _accX, _accY, _accZ,
                        _dev.data.x - _mag_offset.x, _dev.data.y - _mag_offset.y, _dev.data.z - _mag_offset.z, &_roll, &_pitch, &_yaw);
  //MahonyAHRSupdateIMU((_gyroX-_gyroXoff) * DEG_TO_RAD*0.6, (_gyroY-_gyroYoff) * DEG_TO_RAD*0.6, (_gyroZ-_gyroZoff) * DEG_TO_RAD*0.6, _accX, _accY, _accZ, &_roll, &_pitch, &_yaw);
  //MadgwickFilter.update(_gyroX,_gyroY,_gyroZ,_accX,_accY,_accZ,_dev.data.x,_dev.data.y,_dev.data.z);
}

float Ins::pitch() {
  return _pitch;
  //return MadgwickFilter.getRoll();
}
float Ins::roll() {
  return _roll;
  //return MadgwickFilter.getPitch();
}
float Ins::yaw() {
  if (_yaw > 0) {
    return _yaw;
  } else {
    return 360 + _yaw;
  }
  //return MadgwickFilter.getYaw();
}

float Ins::accelG() {
  return -_accY * cos(_pitch * DEG_TO_RAD) + _accZ * sin(_pitch * DEG_TO_RAD);
}

float Ins::sideG(){
  return 0;
}

float Ins::temp() {
  return _temp;
}

int8_t Ins::i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len)
{
  if (M5.I2C.readBytes(dev_id, reg_addr, len, read_data))
  {
    return BMM150_OK;
  }
  else
  {
    return BMM150_E_DEV_NOT_FOUND;
  }
}

int8_t Ins::i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *read_data, uint16_t len)
{
  if (M5.I2C.writeBytes(dev_id, reg_addr, read_data, len))
  {
    return BMM150_OK;
  }
  else
  {
    return BMM150_E_DEV_NOT_FOUND;
  }
}

int8_t Ins::bmm150_initialization()
{
  int8_t rslt = BMM150_OK;

  /* Sensor interface over SPI with native chip select line */
  _dev.dev_id = 0x10;
  _dev.intf = BMM150_I2C_INTF;
  _dev.read = &Ins::i2c_read;
  _dev.write = &Ins::i2c_write;
  _dev.delay_ms = delay;

  /* make sure max < mag data first  */
  _mag_max.x = -2000;
  _mag_max.y = -2000;
  _mag_max.z = -2000;

  /* make sure min > mag data first  */
  _mag_min.x = 2000;
  _mag_min.y = 2000;
  _mag_min.z = 2000;

  rslt = bmm150_init(&_dev);
  _dev.settings.pwr_mode = BMM150_NORMAL_MODE;
  rslt |= bmm150_set_op_mode(&_dev);
  _dev.settings.preset_mode = BMM150_PRESETMODE_ENHANCED;
  rslt |= bmm150_set_presetmode(&_dev);
  return rslt;
}

void Ins::bmm150_offset_save()
{
  _prefs.begin("bmm150", false);
  _prefs.putBytes("offset", (uint8_t *)&_mag_offset, sizeof(bmm150_mag_data));
  _prefs.end();
}

void Ins::bmm150_offset_load()
{
  if (_prefs.begin("bmm150", true))
  {
    _prefs.getBytes("offset", (uint8_t *)&_mag_offset, sizeof(bmm150_mag_data));
    _prefs.end();
    Serial.printf("bmm150 load offset finish.... \r\n");
  }
  else
  {
    Serial.printf("bmm150 load offset failed.... \r\n");
  }
}

void Ins::MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, float *pitch, float *roll, float *yaw) {
  float interval;
  float recipNorm;
  float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
  float hx, hy, bx, bz;
  float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
  float halfex, halfey, halfez;
  float qa, qb, qc;

  // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
  if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
    //MahonyAHRSupdateIMU(gx, gy, gz, ax, ay, az);
    return;
  }

  // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
  if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

    // Normalise accelerometer measurement
    recipNorm = sqrt(ax * ax + ay * ay + az * az);
    ax /= recipNorm;
    ay /= recipNorm;
    az /= recipNorm;

    // Normalise magnetometer measurement
    recipNorm = sqrt(mx * mx + my * my + mz * mz);
    mx /= recipNorm;
    my /= recipNorm;
    mz /= recipNorm;

    // Auxiliary variables to avoid repeated arithmetic
    q0q0 = q0 * q0;
    q0q1 = q0 * q1;
    q0q2 = q0 * q2;
    q0q3 = q0 * q3;
    q1q1 = q1 * q1;
    q1q2 = q1 * q2;
    q1q3 = q1 * q3;
    q2q2 = q2 * q2;
    q2q3 = q2 * q3;
    q3q3 = q3 * q3;

    // Reference direction of Earth's magnetic field
    hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
    hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
    bx = sqrt(hx * hx + hy * hy);
    bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

    // Estimated direction of gravity and magnetic field
    halfvx = q1q3 - q0q2;
    halfvy = q0q1 + q2q3;
    halfvz = q0q0 - 0.5f + q3q3;
    halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
    halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
    halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

    // Error is sum of cross product between estimated direction and measured direction of field vectors
    halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
    halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
    halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

    // Compute and apply integral feedback if enabled
    if (twoKi > 0.0f) {
      //integralFBx += twoKi * halfex * (1.0f / sampleFreq);  // integral error scaled by Ki
      //integralFBy += twoKi * halfey * (1.0f / sampleFreq);
      //integralFBz += twoKi * halfez * (1.0f / sampleFreq);
      interval = float(millis() - _timing) / 1000;
      integralFBx += twoKi * halfex * (interval);  // integral error scaled by Ki
      integralFBy += twoKi * halfey * (interval);
      integralFBz += twoKi * halfez * (interval);
      gx += integralFBx;  // apply integral feedback
      gy += integralFBy;
      gz += integralFBz;
    }
    else {
      integralFBx = 0.0f; // prevent integral windup
      integralFBy = 0.0f;
      integralFBz = 0.0f;
    }

    // Apply proportional feedback
    gx += twoKp * halfex;
    gy += twoKp * halfey;
    gz += twoKp * halfez;
  }

  // Integrate rate of change of quaternion
  //gx *= (0.5f * (1.0f / sampleFreq));   // pre-multiply common factors
  //gy *= (0.5f * (1.0f / sampleFreq));
  //gz *= (0.5f * (1.0f / sampleFreq));
  interval = float(millis() - _timing) / 1000;
  _timing = millis();
  gx *= (0.5f * interval);   // pre-multiply common factors
  gy *= (0.5f * interval);
  gz *= (0.5f * interval);
  qa = q0;
  qb = q1;
  qc = q2;
  q0 += (-qb * gx - qc * gy - q3 * gz);
  q1 += (qa * gx + qc * gz - q3 * gy);
  q2 += (qa * gy - qb * gz + q3 * gx);
  q3 += (qa * gz + qb * gy - qc * gx);

  // Normalise quaternion
  recipNorm = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
  q0 /= recipNorm;
  q1 /= recipNorm;
  q2 /= recipNorm;
  q3 /= recipNorm;

  *pitch = asin(-2 * q1 * q3 + 2 * q0 * q2); // pitch
  *roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1); // roll
  *yaw   = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3); //yaw

  *pitch *= RAD_TO_DEG;
  *yaw   *= -RAD_TO_DEG;
  // Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
  //  8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
  // - http://www.ngdc.noaa.gov/geomag-web/#declination
  //*yaw   -= 7;
  *roll  *= RAD_TO_DEG;
}

float Ins::invSqrt(float x) {
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;
  i = 0x5f3759df - (i >> 1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}
