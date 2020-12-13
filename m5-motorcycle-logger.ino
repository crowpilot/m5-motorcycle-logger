//IMU must define before M5stack.h
//M5stack gray old
//#define M5STACK_MPU9250
#define M5STACK_MPU6886

#include<M5Stack.h>
#include<Wire.h>
#include "utility/MahonyAHRS.h"

#include "Graph.h"
#include "Ins.h"
#include "Watch.h"

File f;

//gps
#include <TinyGPS++.h>
//3G
//#define TINY_GSM_MODEM_UBLOX
//#include <TinyGsmClient.h>
//LED neopixel
#include<Adafruit_NeoPixel.h>
//ENV2
#include<Adafruit_BMP280.h>
#include<Adafruit_SHT31.h>


//GPS
TinyGPSPlus gps;
HardwareSerial GPSserial(2);

//3G
//TinyGsm modem(Serial2);
//TinyGsmClient ctx(modem);

//LED
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(10, 15, NEO_GRB + NEO_KHZ800);

//IMU
Ins ins = Ins();
uint8_t count = 0;

//bank graph G graph
Graph bankGraph = Graph(10, 78, 61, 70, 10);
Graph gravGraph = Graph(250, 78, 60, 71, 35);

//ENV2
Adafruit_BMP280 bme;
Adafruit_SHT31 sht30 = Adafruit_SHT31();

float tmp = 0.0F;
float hum = 0.0F;
float pressure = 0.0F;

Graph tmp_graph = Graph(0, 40, 99, 30);
Graph hum_graph = Graph(101, 40, 98, 30);
Graph press_graph = Graph(201, 40, 118, 30);

//Clock and Laptime
Watch watch=Watch();

//xTask
void refreshIMU(void* arg);
void refreshIMUGraph(void* arg);
void refreshENV(void* arg);
void refreshClock(void* arg);
void writeData(void* arg);
SemaphoreHandle_t xMutex = NULL;
//

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Power.begin();
  Wire.begin();

  M5.Lcd.drawFastHLine(0, 20, 320, TFT_GREEN);
  M5.Lcd.drawFastVLine(100, 20, 50, TFT_GREEN);
  M5.Lcd.drawFastVLine(200, 20, 50, TFT_GREEN);
  M5.Lcd.drawFastHLine(0, 39, 320, TFT_NAVY);
  M5.Lcd.drawFastHLine(0, 70, 320, TFT_GREEN);
  M5.Lcd.drawFastHLine(0, 152, 320, TFT_GREEN);
  M5.Lcd.drawFastHLine(0, 215, 320, TFT_GREEN);

  //GPS
  GPSserial.begin(9600, SERIAL_8N1, 36, 26);
  //3G
  //Serial2.begin(115200, SERIAL_8N1, 16, 17);
  //modem.init();
  // modem.gprsConnect("soracom.io", "sora", "sora");

  //LED
  pixels.begin();

  //IMU
  ins.init();

  //bank graph G graph
  bankGraph.init();
  gravGraph.init();


  //G graph
  M5.Lcd.drawRect(230, 77, 10, 70, TFT_GREEN);

  //ENV2
  bme.begin(0x76);
  sht30.begin(0x44);
  tmp_graph.init();
  hum_graph.init();
  press_graph.init();

  //clock box
  M5.Lcd.drawRect(20, 215, 80, 25, TFT_GREEN);

  M5.Lcd.setCursor(0, 0);
  //M5.Lcd.print(modem.getModemInfo());
  //while(!modem.waitForNetwork());
  //while(!modem.isNetworkConnected());
  M5.Lcd.setTextSize(2);
  //  M5.Lcd.setTextFont(2); not good
  xMutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(refreshENV, "ENV", 8192, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(refreshIMU, "IMU", 8192, NULL, 10, NULL, 1);
  xTaskCreatePinnedToCore(refreshIMUGraph, "IMU", 8192, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(writeData, "writeData", 8192, NULL, 2, NULL, 0);
}

//mode 0:clock 1:laptimer 2:laptimer running
void loop() {
  xSemaphoreTake(xMutex, portMAX_DELAY);

  static uint8_t mode = 0;
  //header 3G connection GPS
  //M5.Lcd.print(gps.satellites.value());
  /*if (modem.isNetworkConnected()) {
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("3G");
    }*/

  //LED
  pixels.setPixelColor(0, pixels.Color(0, 100, 0));
  pixels.show();

  if (mode == 0) {
    //clock
    M5.Lcd.setCursor(10, 159);
    M5.Lcd.setTextSize(7);
    M5.Lcd.printf("%02d:%02d", gps.time.hour() + 9, gps.time.minute());
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf(".%02d", gps.time.second());
    //bottom
    M5.Lcd.setCursor(120, 220);
    M5.Lcd.print("  LAP  ");
  }
  else if (mode == 1) {
    //Laptimer stop

    //bottom
    M5.Lcd.drawRect(20, 216, 80, 24, TFT_BLACK);
    //M5.Lcd.fillRect(125,216,80,24,TFT_BLACK);
    M5.Lcd.setCursor(125, 220);
    M5.Lcd.print("START");
    M5.Lcd.setCursor(220, 220);
    M5.Lcd.print("RESET");
  }

  //common bottom
  M5.Lcd.setCursor(30, 220);
  M5.Lcd.print("clock");

  //button setting
  //lap clock
  M5.update();
  if (M5.BtnA.wasPressed()) {
    //lap
    mode = 0;
  }
  if (M5.BtnB.wasPressed()) {
    mode = 1;
  }
  xSemaphoreGive(xMutex);
  vTaskDelay(100);
}

void refreshIMU(void* arg) {
  for (;;) {
    xSemaphoreTake(xMutex, portMAX_DELAY);

    //roll indicator G indicator
    //bank angle
    M5.Lcd.drawLine(150, 148, 150 + 60 * sin(ins.roll() * 6.28 / 360), 148 - 60 * cos(ins.roll() * 6.28 / 360), TFT_BLACK);
    M5.Lcd.drawFastHLine(231, 123 - ins.forward_g() * 30, 8, TFT_BLACK);
    ins.reload();
    M5.Lcd.drawCircleHelper(150, 148, 70, 0x3, TFT_GREEN);
    M5.Lcd.drawLine(150, 148, 150 + 60 * sin(ins.roll() * 6.28 / 360), 148 - 60 * cos(ins.roll() * 6.28 / 360), TFT_GREEN);
    M5.Lcd.drawFastHLine(231, 123 - ins.forward_g() * 30, 8, TFT_GREEN);
    xSemaphoreGive(xMutex);
    vTaskDelay(20);
  }
}
void refreshIMUGraph(void* arg) {
  for (;;) {
    xSemaphoreTake(xMutex, portMAX_DELAY);

    M5.Lcd.setCursor(120, 0);
    M5.lcd.print(ins.temp());
    M5.Lcd.setCursor(200,0);
    M5.Lcd.print(ins.yaw());

    bankGraph.plotV(30 + ins.roll() / 3);
    gravGraph.plotH(35 - ins.forward_g() * 30);

    xSemaphoreGive(xMutex);
    vTaskDelay(100);
  }
}

void refreshENV(void* arg) {
  for (;;) {
    xSemaphoreTake(xMutex, portMAX_DELAY);

    tmp = sht30.readTemperature();
    hum = sht30.readHumidity();
    pressure = bme.readPressure() * 0.0002953;
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 22);
    M5.Lcd.printf("%2.1fC", tmp);
    M5.Lcd.setCursor(102, 22);
    M5.Lcd.printf("%2.1f%%", hum);
    M5.Lcd.setCursor(202, 22);
    M5.Lcd.printf("%2.2finHg", pressure);
    M5.Lcd.setTextSize(2);
    tmp_graph.centerPlot(tmp * 30);
    hum_graph.centerPlot(hum * 10);
    press_graph.centerPlot(pressure * 1000);

    xSemaphoreGive(xMutex);
    vTaskDelay(10000);
  }
}
void refreshClock(void* arg) {

}
void writeData(void* arg) {
  float yaw, roll, pitch,temp;
  for (;;) {
    if (GPSserial.available()) {
      gps.encode(GPSserial.read());

    }

    xSemaphoreTake(xMutex, portMAX_DELAY);

    yaw = ins.yaw();
    roll = ins.roll();
    pitch = ins.pitch();
    if (gps.satellites.value()) {
      M5.Lcd.setCursor(60, 0);
      M5.Lcd.printf("GPS%1d", gps.satellites.value());
    }

    xSemaphoreGive(xMutex);
    
    //write SD
    static char filename[100];
    if (gps.satellites.value()) {
      sprintf(filename, "/%04d-%02d-%02d-%02d:00.csv"
              , gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour());
    } else {
      sprintf(filename, "/log.csv");
    }
    f = SD.open(filename, FILE_APPEND);
    if (f) {
      f.println(String(pitch) + "," + String(roll) + "," + String(yaw));
    }
    f.close();

    vTaskDelay(100);
  }
}
