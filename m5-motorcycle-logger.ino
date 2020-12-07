//IMU must define before M5stack.h
//M5stack gray old
//#define M5STACK_MPU9250
#define M5STACK_MPU6886

#include<M5Stack.h>
#include<Wire.h>
#include "utility/MahonyAHRS.h"

#include "./Graph.h"

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
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(10,15,NEO_GRB+NEO_KHZ800);

//IMU
#define DEG_TO_RAD 0.017453292519943295769236907684886


float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float gyroX0 = 0.0F;
float gyroY0 = 0.0F;
float gyroZ0 = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float lastpitch = 0.0F;
float bf_g=0.0F;

float temp = 0.0F;

uint8_t count = 0;

TFT_eSprite bank = TFT_eSprite(&M5.Lcd);
TFT_eSprite grav = TFT_eSprite(&M5.Lcd);

//ENV2
Adafruit_BMP280 bme;
Adafruit_SHT31 sht30=Adafruit_SHT31();

float tmp=0.0F;
float hum=0.0F;
float pressure=0.0F;

TFT_eSprite tmp_s = TFT_eSprite(&M5.Lcd);
TFT_eSprite hum_s =TFT_eSprite(&M5.Lcd);
TFT_eSprite press_s=TFT_eSprite(&M5.Lcd);
Graph tmp_graph=Graph(0,40,99,30,&tmp_s);
Graph hum_graph=Graph(101,40,98,30,&hum_s);
Graph press_graph = Graph(201,40,118,30,&press_s);

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Power.begin();
  Wire.begin();

  M5.Lcd.drawFastHLine(0, 20, 320, TFT_GREEN);
  M5.Lcd.drawFastVLine(100,20,50,TFT_GREEN);
  M5.Lcd.drawFastVLine(200,20,50,TFT_GREEN);
  M5.Lcd.drawFastHLine(0,39,320,TFT_NAVY);
  M5.Lcd.drawFastHLine(0, 70, 320, TFT_GREEN);
  M5.Lcd.drawFastHLine(0,152,320,TFT_GREEN);
  M5.Lcd.drawFastHLine(0, 215, 320, TFT_GREEN);

  //GPS
  GPSserial.begin(9600, SERIAL_8N1, 36, 26);
  //GPSserial.begin(9600);
  //3G
  //Serial2.begin(115200, SERIAL_8N1, 16, 17);
  //modem.init();
  // modem.gprsConnect("soracom.io", "sora", "sora");

  //LED
  pixels.begin();
  
  //IMU
  M5.IMU.Init();
  M5.IMU.setGyroFsr(M5.IMU.GFS_250DPS);
  M5.IMU.setAccelFsr(M5.IMU.AFS_8G);
  M5.IMU.setGyroOffset(700, 1100, -300);
  for(int i=0;i<100;i++){
    M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
    gyroX0+=gyroX/100;
    gyroY0+=gyroY/100;
    gyroZ0+=gyroZ/100;
    delay(10);
  }
  //bank graph
  bank.setColorDepth(4);
  bank.createSprite(61, 70);
  bank.fillSprite(TFT_BLUE);
  //G graph
  grav.setColorDepth(3);
  grav.createSprite(60,70);
  grav.fillSprite(TFT_BLUE);
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
}

int getTime(uint8_t mode = 0) {
  return 0;
}

//mode 0:clock 1:laptimer 2:laptimer running
void loop() {
  //GPS encode
  if (GPSserial.available()) {
    gps.encode(GPSserial.read());
    M5.Lcd.setCursor(60, 0);
    if(gps.satellites.value()){
      M5.Lcd.printf("GPS%1d",gps.satellites.value());
    }
  }
  static uint8_t mode = 0;
  //header 3G connection GPS
  //M5.Lcd.print(gps.satellites.value());
  /*if (modem.isNetworkConnected()) {
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("3G");
    }*/

  //LED
  pixels.setPixelColor(0,pixels.Color(0,100,0));
  pixels.show();
  
  //IMU
  //M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  
  //IMU temp data
  M5.IMU.getTempData(&temp);
  M5.Lcd.setCursor(120, 0);
  M5.lcd.print(temp);
  
  //bank graph
  count+=1;
  if (!(count%10)) {
    //bank and gravity sensor
    
    bank.pushSprite(10, 78);
    for (int i = 0; i <= 60; i += 10) {
      bank.drawPixel(i, 0, TFT_BLUE);
    }
    if(!(count%100)){
      bank.drawFastHLine(0,0,60,TFT_BLUE);
      grav.drawFastVLine(0,0,70,TFT_BLUE);
    }
    bank.drawPixel(30 + roll / 3, 0, TFT_GREEN);
    
    grav.pushSprite(250,78);
    grav.drawPixel(0,45-bf_g*30,TFT_GREEN);
    
  }else if(count%10==1){
    //write SD
    static char filename[40] ;
    if(gps.satellites.value()){
    sprintf(filename,"/%04d-%02d-%02d-%02d:00.csv"
            ,gps.date.year(),gps.date.month(),gps.date.day(),gps.time.hour());
    }else{
      sprintf(filename,"/log.csv");
    }
    f=SD.open(filename,FILE_APPEND);
    if(f){
      f.println(String(pitch)+","+String(roll)+","+String(yaw));
    }
    f.close();
  }else if(count%20==2){
    bank.scroll(0, 1);
    grav.scroll(1,0 );
  
  }else if(count==199){
    //ENV2
    count=0;
    pressure=bme.readPressure()*0.0002953;
    tmp=sht30.readTemperature();
    hum=sht30.readHumidity();
    
    //M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(0,22);
    M5.Lcd.printf("%2.1fC",tmp);
    M5.Lcd.setCursor(102,22);
    M5.Lcd.printf("%2.1f%%",hum);
    M5.Lcd.setCursor(202,22);
    M5.Lcd.printf("%2.2finHg",pressure);
    M5.Lcd.setTextSize(2);

    tmp_graph.centerPlot(tmp*30);
    hum_graph.centerPlot(hum*10);
    press_graph.centerPlot(pressure*1000);
  }
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  //roll indicator
  //bank angle 
  M5.Lcd.drawLine(150, 148, 150 + 60 * sin(roll * 6.28 / 360), 148 - 60 * cos(roll * 6.28 / 360), TFT_BLACK);
  //M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  MahonyAHRSupdateIMU((gyroX-gyroX0) * DEG_TO_RAD*0.6, (gyroY-gyroY0) * DEG_TO_RAD*0.6, (gyroZ-gyroZ0) * DEG_TO_RAD*0.6, accX, accY, accZ, &roll, &pitch, &yaw);
  M5.Lcd.drawCircleHelper(150, 148, 70, 0x3, TFT_GREEN);
  M5.Lcd.drawLine(150, 148, 150 + 60 * sin(roll * 6.28 / 360), 148 - 60 * cos(roll * 6.28 / 360), TFT_GREEN);

  //G indicator
  M5.Lcd.drawFastHLine(231,123-bf_g*30,8,TFT_BLACK);
  bf_g=-accY*cos(pitch*DEG_TO_RAD)+accZ*sin(pitch*DEG_TO_RAD);
  M5.Lcd.drawFastHLine(231,123-bf_g*30,8,TFT_GREEN);

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
  delay(1);
}
