git#include "EmonLib.h"             // Include Emon Library
#include <SoftwareSerial.h>
#include "U8glib.h"

// SPI Com: CLK=6,DIN=13,CE=11,DC=12,RST = 10
U8GLIB_PCD8544 u8g(6, 13, 11, 12, 10);

SoftwareSerial rabinSerial(8, 9); // RX, TX

EnergyMonitor emon1;             // Create an instance

float    amp, watt;
//int   amp,watt,volt;
double   unit;

float      volt;
double units;

float realPower       = emon1.realPower;        //extract Real Power into variable
//long int realPower       = emon1.realPower;        //extract Real Power into variable
float apparentPower   = emon1.apparentPower;    //extract Apparent Power into variable
float powerFActor     = emon1.powerFactor;      //extract Power Factor into Variable
float Vrms   = emon1.Vrms;             //extract Vrms into Variable
float Irms            = emon1.Irms;             //extract Irms into Variable


////////////////////////////////////////////////////////////////////////////////////////////////
float V_meter = 220;
float Ref_analog = 720.00;
float sum = 0.0;
float AVS_voltage = 0.0;
////////////////////////////////////////////////////////////////////////////////////////////////


void setup()  {
  emon1.calcVI(20, 2000);        // Calculate all. No.of half wavelengths (crossings), time-out
  //  emon1.voltage(1, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  // emon1.voltage(2, 144.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.voltage(2, 250, 1.7);
  //  emon1.current(0, 111.1);       // Current: input pin, calibration.
  emon1.current(1, 80);       // Current: input pin, calibration.

  Serial.begin(9600);
  rabinSerial.begin(9600);
  Serial.println("Watt Meter..2015");
  //  display_frame();
}
void loop()
{
  //    emon1.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  //    realPower = emon1.realPower;
  Irms  = emon1.Irms;
  amp = get_current();
  if (amp < 0.21)
  { amp = 0;
  }
  calV();
  Vrms  = AVS_voltage ;   //emon1.Vrms;
  realPower = amp * Vrms;
  display_value();
  show_terminal();
}

/**********SUB Function*************************/
void calV() {
    for (int i = 0; i < 10; i++) {
    sum += analogRead(A2);
    delay(1);
  }
  sum /= 10;
  AVS_voltage = (220 / 720.00) * sum;
}

float get_current()
{
  return emon1.calcIrms(1480);
}

void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void box_frame(uint8_t a) {
  //  u8g.drawFrame(x,y,w,h);
  u8g.drawFrame(0, 0, 84, 48);
  //  u8g.drawLine(x1, y1, x2, y2);
  u8g.drawLine(3, 20, 80, 20);
  u8g.drawLine(42, 0, 42, 20);

  u8g.drawStr( 2, 10, "I=     V=   ");
  u8g.drawStr( 4, 30, "P=    ");

  //  u8g.drawBox(x,y,w,h);
  u8g.drawBox(55, 25, 3, 15);
  u8g.drawBox(60, 25, 3, 13);
  u8g.drawBox(65, 25, 3, 10);
  u8g.drawBox(70, 25, 3, 5);
}

void display_frame() {

  u8g.firstPage();
  do {
    u8g_prepare();
    box_frame(0);
  } while ( u8g.nextPage() );
}

void display_value()
{
  u8g_prepare();
  u8g.firstPage();
  do {
    //  u8g.drawFrame(x,y,w,h);
    u8g.drawFrame(0, 0, 84, 48); //สี่เหลี่ยม
    //  u8g.drawLine(x1, y1, x2, y2);
    //    u8g.drawLine(3, 20, 80, 20);
    u8g.drawFrame(1, 1, 40, 20); //สี่เหลี่ยม
    u8g.drawFrame(41, 1, 44, 20); //สี่เหลี่ยม

    //   u8g.drawLine(40, 3, 40, 16); // เส้นแนวตั้ง


    //  u8g.drawBox(x,y,w,h); bar graph
    /*  u8g.drawBox(55,5,3,15);
      u8g.drawBox(60,5,3,13);
      u8g.drawBox(65,5,3,10);
      u8g.drawBox(70,5,3,5);
    */
    u8g.setFontPosTop();
    u8g.setFont(u8g_font_6x10);   //4x6  ,5x8  ,6x10 ok
    u8g.setPrintPos(2, 15);
    //  u8g.print(Irms,1);
    u8g.print(amp, 2);
    u8g.print(" A.");
    u8g.setPrintPos(45, 15);
    u8g.print(Vrms, 0);
    u8g.print(" V.");

    u8g.setFontPosTop();
    u8g.setFont(u8g_font_10x20);
    u8g.setPrintPos(2, 40);
    u8g.print(realPower, 0);
    u8g.setFont(u8g_font_6x12);
    u8g.print(" W.");

  } while ( u8g.nextPage() );
}

void show_terminal()
{
  // Serial.println("*******************************");
  Serial.print("Irms=");
  //   Serial.print(Irms);
  realPower = realPower / 100;
  Serial.print(realPower);
  Serial.print("\r");

  rabinSerial.print("Irms=");
  //   Serial.print(Irms);
  realPower = realPower / 100;
  rabinSerial.print(realPower);
  rabinSerial.print("\r");

  /*
     Serial.print("realPower=");
     Serial.println(realPower);
     Serial.print("Vrms=");
     Serial.println(Vrms);
  */
}
