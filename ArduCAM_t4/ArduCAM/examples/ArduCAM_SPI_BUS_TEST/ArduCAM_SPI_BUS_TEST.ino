// ArduCAM demo (C)2013 Lee
// web: http://www.ArduCAM.com
// This program is a demo of how to test the ArduCAM shield SPI communication
// to check out if bus has errors.
//
// This demo was made for Omnivision OV5642 sensor.
// 1. Write ArduCHIP internal test registers.
// 2. Read out ArduCHIP internal test registers and send back to Serial Monitor
// 3. Read out ArduCHIP internal Revision registers and send back to Serial Monitor
// This program requires the ArduCAM V3.0.0 (or above) library and Rev.C ArduCAM shield
// and use Arduino IDE 1.5.2 compiler


#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <SD.h>

// set pin 10 as the slave select for the ArduCAM shield:
const int slaveSelectPin = 7;

#define ARDUCHIP_TEST1         0x00  //TEST register
#define ARDUCHIP_TEST2         0x01  //TEST register
#define ARDUCHIP_REV           0x40  //ArduCHIP revision

ArduCAM myCAM(OV2640,slaveSelectPin);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Test START..."); 
  // initialize SPI:
  // set the SPI_CS as an output:
  pinMode(slaveSelectPin, OUTPUT);
  digitalWrite(slaveSelectPin, HIGH);
  // initialize SPI:
  SPI.begin(); 
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  
  myCAM.InitCAM();

}

void loop()
{
  uint8_t temp1,temp2,revision;

  temp1 = myCAM.read_reg(ARDUCHIP_TEST1);                //Read from test1 register 
  temp2 = myCAM.read_reg(ARDUCHIP_TEST2);                //Read from test1 register
  Serial.println(temp1,HEX);
  Serial.println(temp2,HEX);
  delay(1000);
  myCAM.write_reg(ARDUCHIP_TEST1, 0xAA);		 //Write to test1 register by 0x55
  myCAM.write_reg(ARDUCHIP_TEST2, 0x55);		 //Write to test1 register by 0xaa
  delay(1000);
  temp1 = myCAM.read_reg(ARDUCHIP_TEST1);                //Read from test1 register 
  temp2 = myCAM.read_reg(ARDUCHIP_TEST2);                //Read from test1 register
  Serial.print("Test1 (S/B 0xAA): "); Serial.println(temp1,HEX);
  Serial.print("Test1 (S/B 0x55: "); Serial.println(temp2, HEX);
  delay(1000);
  revision = myCAM.read_reg(ARDUCHIP_REV);              //Read from REV register
  Serial.print("Revision: "); Serial.println(revision, HEX);
  delay(1000);

}

   
