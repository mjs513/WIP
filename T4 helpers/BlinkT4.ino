/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

#include "clock_functions.h"

// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:
int led = 13;

// the setup routine runs once when you press reset:
void setup() {
  while ( !Serial && millis() < 4000 ) ;

  delay(2000);
  /*set osc clock crystal freq */
  CLOCK_SetXtalFreq(24000000UL);
  
  Serial.printf("System Clock: %d\r\n", CLOCK_GetAhbFreq());
  Serial.printf("IPG Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_IpgClk));
  Serial.printf("RTC Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_RtcClk));
  Serial.printf("USB1pll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
  Serial.printf("Peripheral Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_PerClk));
  Serial.printf("Osc Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_OscClk));
  Serial.printf("Arm Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_ArmPllClk));
  Serial.printf("Usb1PllPfd0 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
  Serial.printf("Usb1PllPfd1 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
  Serial.printf("Usb1PllPfd2 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
  Serial.printf("Usb1PllPfd3 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
  Serial.printf("Usb2Pll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb2PllClk));
  Serial.printf("SysPll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
  Serial.printf("SysPllPfd0 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
  Serial.printf("SysPllPfd1 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
  Serial.printf("SysPllPfd2 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
  Serial.printf("SysPllPfd3 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
  Serial.printf("EnetPll0 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_EnetPll0Clk));
  Serial.printf("EnetPll1 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_EnetPll1Clk));
  Serial.printf("AudioPll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_AudioPllClk));
  Serial.printf("VideoPll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));


  
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
      
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
}
