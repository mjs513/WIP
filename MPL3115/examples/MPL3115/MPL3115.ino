#include <Wire.h>
#include "MPL3115.h"

MPL3115A2 myPressure;

void setup() {
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output

  myPressure.begin(); // Get sensor online


}

void loop() {
  myPressure.getBaro();
  delay(100);
}
