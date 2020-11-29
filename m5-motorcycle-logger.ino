//IMU must define before M5stack.h
//M5stack gray old
//#define M5STACK_MPU9250
#define M5STACK_MPU6886

#include<M5Stack.h>
#include<Wire.h>

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
HardwareSerial GPSserial(1);

//3G
//TinyGsm modem(Serial2);
//TinyGsmClient ctx(modem);

//LED
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(10,15,NEO_GRB+NEO_KHZ800);

//IMU
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float lastpitch = 0.0F;

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
float last_tmp=0.0F;
float last_hum=0.0F;
float last_press=0.0F;

TFT_eSprite tmp_s = TFT_eSprite(&M5.Lcd);
TFT_eSprite hum_s =TFT_eSprite(&M5.Lcd);
TFT_eSprite press_s=TFT_eSprite(&M5.Lcd);


void setup() {
  // put your setup code here, to run once:
  M5.begin();
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
  M5.IMU.setGyroOffset(800, 980, -248);
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

  tmp_s.setColorDepth(3);
  hum_s.setColorDepth(3);
  press_s.setColorDepth(3);
  tmp_s.createSprite(99,30);
  hum_s.createSprite(98,30);
  press_s.createSprite(118,30);
  tmp_s.fillSprite(TFT_BLUE);
  hum_s.fillSprite(TFT_BLUE);
  press_s.fillSprite(TFT_BLUE);

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
    M5.Lcd.print("GPS");
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
    bank.drawPixel(30 + pitch / 3, 0, TFT_GREEN);
    bank.scroll(0, 1);

    grav.pushSprite(250,78);
    grav.drawPixel(0,45-accZ*10,TFT_GREEN);
    grav.scroll(1,0 );
  }else if(count==199){
    count=0;
    tmp_s.pushSprite(0,40);
    hum_s.pushSprite(101,40);
    press_s.pushSprite(201,40);
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

    tmp_s.scroll(1,(tmp-last_tmp)*30);
    hum_s.scroll(1,(hum-last_hum)*10);
    press_s.scroll(1,(pressure-last_press)*1000);
    tmp_s.drawPixel(0,15,TFT_GREEN);
    hum_s.drawPixel(0,15,TFT_GREEN);
    press_s.drawPixel(0,15,TFT_GREEN);

    last_tmp=tmp;
    last_hum=hum;
    last_press=pressure;
  }
  //roll indicator
  //bank angle pitch=roll
  M5.Lcd.drawLine(150, 148, 150 + 60 * sin(pitch * 6.28 / 360), 148 - 60 * cos(pitch * 6.28 / 360), TFT_BLACK);
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  M5.Lcd.drawCircleHelper(150, 148, 70, 0x3, TFT_GREEN);
  M5.Lcd.drawLine(150, 148, 150 + 60 * sin(pitch * 6.28 / 360), 148 - 60 * cos(pitch * 6.28 / 360), TFT_GREEN);
  //G indicator
  M5.Lcd.drawFastHLine(231,123-accZ*10,8,TFT_BLACK);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  M5.Lcd.drawFastHLine(231,123-accZ*10,8,TFT_GREEN);
  

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
}
