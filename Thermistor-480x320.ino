/*

  This program provides cartesian type graph function

  It requires and Arduino Mega (or UNO) and an Adafruit 3.5" TFT 320x480 + Touchscreen Breakout Board
  https://learn.adafruit.com/adafruit-3-5-color-320x480-tft-touchscreen-breakout/overview

  Note: other LCD's could be used but will require some modifications to this code

  Adafruit libraries
  https://github.com/adafruit/Adafruit_HX8357_Library/archive/master.zip
  https://github.com/adafruit/Adafruit-GFX-Library/archive/master.zip

  optional touch screen libraries
  https://github.com/adafruit/Touch-Screen-Library/archive/master.zip

  Revisions
  rev     date        author      description
  1       12-24-2015  kasprzak    initial creation


  This pin setting will also operate the SD card

  Pin settings

  Arduino   device
  5V        Vin
  GND       GND
  A0
  A1
  A2         Y+ (for touch screen use)
  A3         X- (for touch screen use)
  A4
  A5
  1
  2
  3
  4         CCS (42 for mega)
  5
  6
  7         Y- (44 for mega)
  8         X+ (46 for mega)
  9         DC (48 on mega * change define)
  10        CS (53 for mega * change define)
  11        MOSI (51 for mega)
  12        MISO  (50 for mega)
  13        CLK (SCK) (52 for mega)
  44        Y- (for touch screen only)
  46        X+ (for touch screen only)
  48        DC
  SDA
  SLC

*/
// #include "TouchScreen.h"
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"

#define TFT_CS 53
#define TFT_DC 48
#define TFT_RST 8 // RST can be set to -1 if you tie it to Arduino's reset
#define SD_CCS 42


#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN   0xBFF7
#define LTCYAN    0xC7FF
#define LTRED     0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW  0xFFF8
#define LTORANGE  0xFE73
#define LTPINK    0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY    0xE71C

#define BLUE      0x001F
#define TEAL      0x0438
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
#define PINK      0xF81F
#define PURPLE    0x8010
#define GREY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000

#define DKBLUE    0x000D
#define DKTEAL    0x020C
#define DKGREEN   0x03E0
#define DKCYAN    0x03EF
#define DKRED     0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW  0x8400
#define DKORANGE  0x8200
#define DKPINK    0x9009
#define DKPURPLE  0x4010
#define DKGREY    0x4A49
#define TS_MINX 110
#define TS_MINY 80
#define TS_MAXX 900
#define TS_MAXY 940
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 44
#define XP 46

double idletime , currtime , prevtime;
int TRpin = A0;
double temp, x, y;

Adafruit_HX8357 display = Adafruit_HX8357(TFT_CS, TFT_DC);

boolean display1 = true;
double ox , oy ;

void setup() {
  Serial.begin(9600);

  display.begin(HX8357D);
  display.fillScreen(BLACK);
  display.setRotation(3);
  pinMode(TRpin, INPUT);

  x = 0;

}


void loop(void) {

  currtime = millis() / ( 1000.0);
  idletime = currtime - prevtime;


  if (idletime >= 1 & currtime <= 60 ) {

    temp = GetTemp( TRpin);

    Graph(display, x, temp, 60, 270, 390, 220, 0, 60, 5 , 75, 95, 2, "Temperature Trend", "Time (sec)", "Temp (deg F)", DKBLUE, RED, YELLOW, WHITE, BLACK, display1);
    prevtime = currtime;
    x++;
  }

}

/*

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

  huge arguement list
  &d name of your display object
  x = x data point
  y = y datapont
  gx = x graph location (lower left)
  gy = y graph location (lower left)
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis
  xhi = upper bound of x asis
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis
  yhi = upper bound of y asis
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x asis label
  ylabel = y asis label
  gcolor = graph line colors
  acolor = axi ine colors
  pcolor = color of your plotted data
  tcolor = text color
  bcolor = background color
  &redraw = flag to redraw graph on fist call only
*/


void Graph(Adafruit_HX8357 & d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi, double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor, unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, boolean & redraw) {

  double ydiv, xdiv;
  double i;
  double temp;
  int rot, newrot;
  static int cRot = 0;
  if (redraw == true) {

    redraw = false;
    // initialize old x and old y in order to draw the first point of the graph
    // but save the transformed value
    // note my transform funcition is the same as the map function, except the map uses long and we need doubles
    ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    // draw y scale
    for ( i = ylo; i <= yhi; i += yinc) {
      // compute the transform
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;

      if (i == 0) {
        d.drawLine(gx, temp, gx + w, temp, acolor);
      }
      else {
        d.drawLine(gx, temp, gx + w, temp, gcolor);
      }
      // draw the axis labels
      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(gx - 40, temp + 5);
      // precision is default Arduino--this could really use some format control
      d.println(i);
    }
    // draw x scale
    for (i = xlo; i <= xhi; i += xinc) {

      // compute the transform
      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      if (i == 0) {
        d.drawLine(temp, gy, temp, gy - h, acolor);
      }
      else {
        d.drawLine(temp, gy, temp, gy - h, gcolor);
      }
      // draw the x axis labels
      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(temp - 010, gy + 15);
      // precision is default Arduino--this could really use some format control
      d.println(i, 1);
    }

    //now draw the graph labels
    d.setTextSize(2);
    d.setTextColor(tcolor, bcolor);
    d.setCursor(gx , gy - h - 30);
    d.println(title);

    d.setTextSize(1);
    d.setTextColor(acolor, bcolor);
    d.setCursor(gx , gy + 33);
    d.println(xlabel);

    cRot = d.getRotation();

    d.setRotation(abs(cRot - 5));

    d.setTextSize(1);
    d.setTextColor(acolor, bcolor);
    d.setCursor(gx - 20, gy - h - 45);
    d.println(ylabel);
    d.setRotation(cRot);
  }

  // the coordinates are now drawn, plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized above
  x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
  y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  d.drawLine(ox, oy, x, y, pcolor);
  // it's up to you but drawing 2 more lines to give the graph some thickness
  d.drawLine(ox, oy + 1, x, y + 1, pcolor);
  d.drawLine(ox, oy - 1, x, y - 1, pcolor);
  ox = x;
  oy = y;

}

/*

  End of graphing function

*/


double GetTemp(int pin) {
  int i;
  double a1, b1, c1, d1, r2, r1, volts, tempC, tempF, tempK;
  a1 = 3.354016E-03 ;
  b1 = 2.569850E-04 ;
  c1 = 2.620131E-06 ;
  d1 = 6.383091E-08 ;

  // read the temp
  volts =   analogRead(pin);
  volts = volts / 204.6;

  // voltage divider calculation
  // vo = 5 * r2 /(r1+r2)
  // solve for r2
  // get the exact value for voltage divider r1
  r1 = 9730;
  r2 = ( volts * r1) / (5.0 - volts);

  //equation from data sheet
  tempK = 1.0 / (a1 + (b1 * (log(r2 / 10000.0))) + (c1 * pow(log(r2 / 10000.0), 2)) + (d1 * pow(log(r2 / 10000.0), 3)));
  tempC  = ((tempK - 273.15) );
  tempF  = (tempC * 1.8000) + 32.00;
  return tempF;

}






