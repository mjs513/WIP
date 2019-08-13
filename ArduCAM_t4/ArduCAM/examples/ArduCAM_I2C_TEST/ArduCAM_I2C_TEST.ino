// ArduCAM demo (C)2013 Lee
// web: http://www.ArduCAM.com
// This program is a demo of how to communicate with camera modules
// via I2C interface and send read data back to Serial Monitor in Arduino IDE.
//
// This demo was made for Omnivision OV5642 sensor.
// 1. Receive commands from Serial Monitor
// 2. Read Product ID from OV5642 registers
// 3. Send back to Serial Monitor.
// This program requires the ArduCAM V3.0.0 (or above) library and Rev.C ArduCAM shield
// and use Arduino IDE 1.5.2 compiler

#include <UTFT_SPI.h>
#include <SD.h>
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"

// set pin 10 as the slave select for the ArduCAM shield:
const int SPI_CS = 10;

ArduCAM myCAM(OV2640, SPI_CS);
//UTFT myGLCD(SPI_CS);
uint8_t vid,pid;
uint8_t temp; 

void setup()
{
  Wire.begin();

  Serial.begin(115200);
  Serial.println(F("ArduCAM Start!")); 
  // set the SPI_CS as an output:
  //pinMode(SPI_CS, OUTPUT);
  //digitalWrite(SPI_CS, HIGH);
  // initialize SPI:
  //SPI.begin(); 
    
  //Reset the CPLD
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100);

  myCAM.set_mode(MCU2LCD_MODE);
  delay(2000);
  //myGLCD.InitLCD();

  while(1){
    //Check if the camera module type is OV2640
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
      Serial.println(F("Can't find OV2640 module!"));
      delay(1000); continue;
    }else{
      Serial.println(F("OV2640 detected.")); break;
    }
  }

  myCAM.set_format(JPEG);
  myCAM.InitCAM();

}

void loop()
{
}
