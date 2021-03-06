

#include <SPI.h>
#include <Adafruit_GFX.h>         //Downlaod it here : https://www.electronoobs.com/eng_arduino_Adafruit_GFX.php
#include <Adafruit_PCD8544.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial nodemcu(11,12);
//for gps
static const int RXPin = 9, TXPin = 8;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
 Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

 #define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16



static const unsigned char PROGMEM EN_logo [] = {
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,  
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B01111111, B11111100, B01111100, B00111000, B00000000,  
B00000000, B00000000, B00000000, B01111111, B11111100, B01111100, B00111000, B00000000,
B00000000, B00000000, B00000000, B01110000, B00000000, B01111100, B00111000, B00000000,  
B00000000, B00000000, B00000000, B01110000, B00000000, B01110011, B00111000, B00000000,  

B00000000, B00000000, B00000000, B01110000, B00000000, B01110011, B00111000, B00000000,
B00000000, B00000000, B00000000, B01110000, B00000000, B01110011, B00111000, B00000000,
B00000000, B00000000, B00000000, B01110000, B00000000, B01110011, B00111000, B00000000,  
B00000000, B00000000, B00000000, B01111111, B11000000, B01110000, B11111000, B00000000,  
B00000000, B00000000, B00000000, B01111111, B11000000, B01110000, B11111000, B00000000,  
B00000000, B00000000, B00000000, B01110000, B00000000, B01110000, B00111000, B00000000,
B00000000, B00000000, B00000000, B01110000, B00000000, B01110000, B00111000, B00000000,
B00000000, B00000000, B00000000, B01110000, B00000000, B01110000, B00111000, B00000000,

B00000000, B00000000, B00000000, B01110000, B00000000, B01110000, B00111000, B00000000,  
B00000000, B00000000, B00000000, B01110000, B00000000, B01110000, B00111000, B00000000,
B00000000, B00000000, B00000000, B01110000, B00000000, B01110000, B00111000, B00000000,  
B00000000, B00000000, B00000000, B01111111, B11111100, B01110000, B00111000, B00000000,  
B00000000, B00000000, B00000000, B01111111, B11111100, B01110000, B00111000, B00000000,  
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
};


static const unsigned char PROGMEM  LOGO[] =
{
/* 0X00,0X01,0X40,0X00,0X40,0X00, */
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X3F,0XE0,0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0XF0,0X00,0X00,0X00,0X00,0X00,
0X01,0XFF,0XFC,0X00,0X00,0X00,0X00,0X00,0X03,0XFF,0XFE,0X00,0X00,0X00,0X00,0X00,
0X03,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,0X07,0XFF,0XFF,0X00,0X00,0X00,0X00,0X00,
0X0F,0XFF,0XFF,0X80,0X00,0X00,0X00,0X00,0X0F,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,
0X1F,0XFF,0XFF,0XC0,0X00,0X00,0X00,0X00,0X1F,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,
0X1F,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,0X3F,0XFF,0XFF,0XE0,0X00,0X00,0X00,0X00,
0X3F,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,0X7F,0XFF,0XFF,0XF0,0X00,0X00,0X00,0X00,
0X7F,0XFF,0XF8,0X70,0X00,0X00,0X00,0X00,0X60,0XFF,0XC0,0X70,0X00,0X00,0X00,0X00,
0X60,0X00,0X00,0X70,0X00,0X00,0X00,0X00,0X60,0X00,0X00,0X70,0X00,0X02,0X00,0X00,
0X40,0X00,0X00,0X33,0XDF,0XCF,0XF0,0X00,0X40,0X00,0X00,0X33,0XAF,0XFB,0XF8,0X00,
0X40,0X00,0X00,0X33,0XEF,0XFB,0XF8,0X1C,0X40,0X01,0X00,0X33,0XEF,0XFB,0XFC,0X1F,
0X80,0X00,0X00,0X33,0XDF,0XFF,0XFC,0X0F,0X80,0X00,0X00,0X1B,0XF0,0X07,0XFC,0X1F,
0XBF,0X00,0XFE,0X0B,0XF0,0X07,0XFE,0X3F,0X82,0X41,0X70,0X1B,0XFF,0XF7,0XFE,0X3F,
0X83,0X41,0X70,0X0B,0X7F,0XF7,0XDD,0X3E,0X87,0X40,0XA0,0X03,0X7F,0XF7,0XDF,0X3E,
0X80,0X00,0X00,0X0B,0XFF,0XF7,0XCF,0X3E,0X80,0X00,0X00,0X0B,0XFF,0X77,0XCE,0X9E,
0X00,0X00,0X00,0X2B,0X70,0X03,0X84,0X1E,0X40,0X00,0X00,0X32,0XF0,0X07,0XC0,0XBE,
0X00,0X00,0X00,0X70,0XF0,0X0C,0XC6,0XFE,0X20,0X00,0X00,0X42,0X5F,0XFF,0XC3,0XFE,
0X20,0X00,0X00,0X43,0X3F,0XFF,0X83,0XFC,0X20,0X00,0X00,0X41,0X7F,0XFF,0X83,0XFC,
0X20,0X04,0X00,0X43,0XFF,0XFF,0X83,0XFC,0X10,0X00,0X00,0X42,0XFF,0XF0,0X81,0XFC,
0X10,0X00,0X00,0X80,0X00,0X00,0X01,0XFC,0X00,0X3F,0X00,0X80,0X00,0X00,0X00,0X7C,
0X08,0X40,0X00,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X00,0X00,0X00,0X00,0X00,
0X04,0X00,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X40,0X00,0X00,0X00,0X00,
0X03,0X00,0X10,0XC0,0X00,0X00,0X00,0X00,0X01,0X00,0X23,0X80,0X00,0X00,0X00,0X00,
0X00,0X80,0X5F,0X00,0X00,0X00,0X00,0X00,0X00,0X7F,0XEB,0X40,0X00,0X00,0X00,0X00,
0X00,0X01,0X8F,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X9F,0X40,0X00,0X00,0X00,0X00,
0X00,0X00,0XEE,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X9F,0X60,0X00,0X00,0X00,0X00,
0X00,0X00,0XEF,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X77,0XE0,0X00,0X00,0X00,0X00,
0X00,0X00,0X78,0X40,0X00,0X00,0X00,0X00,0X00,0X00,0X67,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X0D,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

int sensor1 = A0;
int sensor2 = A1; 
 

int sdata1 = 0; // sensor1 data
int sdata2 = 0; // sensor2 data
int sdata3 = 0; // sensor3 data for speed from gps

String cdata; // complete data, consisting of sensors values

void setup()
{
  Serial.begin(9600); 
  nodemcu.begin(9600);
  ss.begin(GPSBaud);
  display.begin(); // init done
  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);// clears the screen and buffer

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  
  

}
int speedv;
void loop()
{

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);

  while (ss.available())
   {
      gps.encode(ss.read());
      if (gps.location.isUpdated())
      { 
        speedv = gps.speed.kmph();  
      }
   }
    
    display.setTextSize(3);
    display.setCursor(0,5); 
    display.print(speedv);
    //Serial.println(speedv);
    display.setTextSize(1);
    display.setCursor(56,19);
    display.print("km/h");
    display.display();
    
    sdata1 = analogRead(sensor1);
    sdata2 = analogRead(sensor2);
    sdata3 = speedv;//speed
    cdata = cdata + sdata1+","+sdata2+","+sdata3; // comma will be used a delimeter
   //cdata = cdata + sdata1+","+sdata2; // comma will be used a delimeter
   Serial.println(cdata); 
   nodemcu.println(cdata);
   delay(60000); // 1 minute delay
   cdata = ""; 

}
