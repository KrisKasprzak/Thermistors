/*

  The purpose of this program is create a simple thermistor using an Arduino UNO and an LCD 128 x 64 display

  Code will create a PWM signal for display on the graph
  The display is a Balance world Inc 0.96" Inch Yellow and Blue I2c IIC Serial 128x64 Oled LCD Oled LED Module for Arduino


  Revisions
  rev     date        author      description
  1       12-24-2015  kasprzak    initial creation

  Pin connections
  Arduino   device
  Ground    LCD ground
  +5VDC     Vcc
  A0    input for data read (5 VDC limit)
  A1    input voltage to adjust PWM (optional used here for demo purposes)
  A2
  A3
  A4    SDA (if no SDA pin)
  A5    SCL (if not SCL pin)
  1
  2
  3
  4
  5
  6
  7
  8
  9     PWM output (optional used here for demo purposes, connect to A0)
  10
  11
  12
  13
  SDA   SDA
  SLC   SLC

  Graphics Libraries
  https://github.com/adafruit/Adafruit-GFX-Library
  https://github.com/adafruit/Adafruit_SSD1306

  display
  http://www.amazon.com/Balance-world-Yellow-Arduino-Display/dp/B00ZI01RO0/ref=sr_1_1?ie=UTF8&qid=1448860339&sr=8-1&keywords=balance+world+.96

*/

#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

char buf [30];
int TRpin = A0;
double a1, b1, c1, d1, r2, r1, vo, tempC, tempF, tempK, oldTemp;

void setup()   {

  a1 = 3.354016E-03 ;
  b1 = 2.569850E-04 ;
  c1 = 2.620131E-06 ;
  d1 = 6.383091E-08 ;
  pinMode(TRpin, INPUT);

  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  // init done
  DrawTitles();
  display.display();

}


void loop() {
  // read the temp
  vo = analogRead(TRpin);
  vo = vo / 204.6;

  // voltage divider calculation
  // vo = 5 * r2 /(r1+r2)
  // solve for r2
  // get the exact value for voltage divider r1
  r1 = 9780;
  r2 = ( vo * r1) / (5 - vo);

  //equation from data sheet
  tempK = 1.0 / (a1 + (b1 * (log(r2 / 10000.0))) + (c1 * pow(log(r2 / 10000.0), 2)) + (d1 * pow(log(r2 / 10000.0), 3)));
  tempC  = ((tempK - 273.15) );
  tempF  = (tempC * 1.8000) + 32.00;

  Serial.println(tempF);

  tempF = int(tempF * 10);
  Serial.println(tempF);

  tempF = tempF / 10;
  Serial.println(tempF);

  DrawTemp();


}

void DrawTemp(void) {


  display.setTextSize(4);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 35);
  display.println(oldTemp);
  oldTemp = tempF;

  // write the new one
  display.setTextSize(4);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 35);
  display.println(tempF);

  display.display();
}


void DrawTitles(void) {

  display.setTextSize(2);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.println("Thermistor");

  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 19);
  display.println("Temperature");
  display.display();
}



