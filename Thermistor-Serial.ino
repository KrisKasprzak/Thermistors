/*

  Voltage divider
  R1 = 10K resistor (Vcc and common point)
  r2 = 10K THERMISTOR (GND and common point)
  Vo = pin A1 (common point


  Pin connections
  Arduino   device
  5V        resistor
  GND       Thermistor
  A0        Voltage divider Vo (common point between termistor and resistor)
  A1
  A2
  A3
  A4
  A5
  1
  2
  3
  4
  5
  6
  7
  8
  9
  10
  11
  12
  13
  SDA
  SLC

*/

int TR_PIN = A0;
double a1, b1, c1, d1, r2, r1, vo, tempC, tempF, tempK;

void setup()
{
  Serial.begin(9600);
  a1 = 3.354016E-03 ;
  b1 = 2.569850E-04 ;
  c1 = 2.620131E-06 ;
  d1 = 6.383091E-08 ;

  r1 = 9720.0;

  pinMode(TR_PIN, INPUT);

}

void loop()
{

  // read the temp
  vo = analogRead(TR_PIN);
  vo = vo / (1023.0 / 5.0);

  // voltage divider calculation
  // vo = 5 * r2 /(r1+r2)
  // solve for r2
  // get the exact value for voltage divider r1

  r2 = ( vo * r1) / (5.0 - vo);

  //equation from data sheet
  tempK = 1.0 / (a1 + (b1 * (log(r2 / 10000.0))) + (c1 * pow(log(r2 / 10000.0), 2.0)) + (d1 * pow(log(r2 / 10000.0), 3.0)));
  tempC  = (tempK - 273.15);
  tempF  = (tempC * 1.8) + 32.0;

  Serial.println("Temp - degK " + String(tempK));
  Serial.println("Temp - degC " + String(tempC));
  Serial.println("Temp - degF " + String(tempF));
  Serial.println(" ");
  delay(1000);

}

