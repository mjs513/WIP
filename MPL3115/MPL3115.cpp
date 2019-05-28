#include <Wire.h>

#include "MPL3115.h"
#include "Arduino.h"

MPL3115A2::MPL3115A2()
{
  //Set initial values for private vars
}

//Begin
/*******************************************************************************************/
//Start I2C communication
void MPL3115A2::begin(void)
{
  Wire.begin();
  init();
}

void MPL3115A2::init(void)
{
		MPL3115A2Reset();                // Start off by resetting all registers to the default
		Serial.println("MPL Reset");
		initRealTimeMPL3115A2();         // initialize the altimeter for realtime data acquisition if communication is OK
		Serial.println("MPL Initialized");
		MPL3115A2SampleRate(SAMPLERATE); // Set oversampling ratio
		Serial.println("MPL Samplerate");
		MPL3115A2enableEventflags();     // Set data ready enable
		Serial.println("MPL3115A2 event flags enabled...");
}

void MPL3115A2::getBaro(){
    MPL3115A2ActiveAltimeterMode(); 
    MPL3115A2readAltitude();  // Read the altitude

    MPL3115A2ActiveBarometerMode(); 
    MPL3115A2readPressure();  // Read the pressure
    
    const int station_elevation_m = def_sea_press*0.3048; // Accurate for the roof on my house; convert from feet to meters

    float baroin = pressure/100; // pressure is now in millibars

    // Formula to correct absolute pressure in millbars to "altimeter pressure" in inches of mercury 
    // comparable to weather report pressure
    float part1 = baroin - 0.3; //Part 1 of formula
    const float part2 = 0.0000842288;
    float part3 = pow(part1, 0.190284);
    float part4 = (float)station_elevation_m / part3;
    float part5 = (1.0 + (part2 * part4));
    float part6 = pow(part5, 5.2553026);
    altimeter_setting_pressure_mb = part1 * part6; // Output is now in adjusted millibars
    baroin = altimeter_setting_pressure_mb * 0.02953;
}

//===================================================
//MPL3115A2 Sensor Calls
//===================================================
void MPL3115A2::MPL3115A2readAltitude() // Get altitude in meters and temperature in centigrade
{
  uint8_t rawData[5];  // msb/csb/lsb pressure and msb/lsb temperature stored in five contiguous registers 

// We can read the data either by polling or interrupt; see data sheet for relative advantages
// First we try hardware interrupt, which should take less power, etc.
// while (digitalRead(int1Pin) == LOW); // Wait for interrupt pin int1Pin to go HIGH
// digitalWrite(int1Pin, LOW);  // Reset interrupt pin int1Pin
 while((readByte(MPL3115A2_ADDRESS, MPL3115A2_INT_SOURCE) & 0x80) == 0); // Check that the interrupt source is a data ready interrupt
// or use a polling method
// Check data read status; if PTDR (bit 4) not set, then
// toggle OST bit to cause sensor to immediately take a reading
// Setting the one shot toggle is the way to get faster than 1 Hz data read rates
// while ((readByte(MPL3115A2_ADDRESS, MPL3115A2_STATUS) & 0x08) == 0);  MPL3115A2toggleOneShot(); 
  
  readBytes(MPL3115A2_ADDRESS, MPL3115A2_OUT_P_MSB, 5, &rawData[0]);  // Read the five raw data registers into data array

// Altitude bytes-whole altitude contained defined by msb, csb, and first two bits of lsb, fraction by next two bits of lsb
  uint8_t msbA = rawData[0];
  uint8_t csbA = rawData[1];
  uint8_t lsbA = rawData[2];
// Temperature bytes
  uint8_t msbT = rawData[3];
  uint8_t lsbT = rawData[4];

 // Calculate altitude 
  int16_t altitude_whole = ((int16_t)msbA << 8 | (int16_t)csbA ) ; // Construct signed 16-bit whole number altitude
 
  lsbA &= 0xF0; // Keep only bits 5 - 7, the fractional altitude
  lsbA >>= 4; // Shift to get the fractional altitude
  float altitude_frac = (float) lsbA/16.0; // Convert to fractional altitude in meters

  altitude = (float) (altitude_whole) + altitude_frac; // Combine whole and fractional parts to get entire pressure in Pascal

 // Calculate temperature 
  int16_t temperature_whole = ((int16_t)msbT << 8 | lsbT ) ; // Construct signed 16-bit whole number temperature
  temperature_whole >>= 8;
 
  lsbT &= 0xF0; // Keep only bits 5 - 7, the fractional temperature
  lsbT >>= 4; // Shift to get the fractional temperature
  float temperature_frac = (float) lsbT/16.0; // Convert to fractional temperature in Centigrade

  temperature = (float) (temperature_whole) + temperature_frac; // Combine whole and fractional parts to get entire temperature in Centigrade
}

void MPL3115A2::MPL3115A2readPressure()
{
  uint8_t  rawData[5];  // msb/csb/lsb pressure and msb/lsb temperature stored in five contiguous registers

// We can read the data either by polling or interrupt; see data sheet for relative advantages
// First we try hardware interrupt, which should take less power, etc.
// while (digitalRead(int1Pin) == LOW); // Wait for interrupt pin int1Pin to go HIGH
// digitalWrite(int1Pin, LOW);  // Reset interrupt pin int1Pin
 while((readByte(MPL3115A2_ADDRESS, MPL3115A2_INT_SOURCE) & 0x80) == 0); // Check that the interrupt source is a data ready interrupt
// or use a polling method
// Check data read status; if PTDR (bit 4) not set, then
// toggle OST bit to cause sensor to immediately take a reading
// Setting the one shot toggle is the way to get faster than 1 Hz data read rates
 //while ((readByte(MPL3115A2_ADDRESS, MPL3115A2_STATUS) & 0x08) == 0);  MPL3115A2toggleOneShot(); 
 
  readBytes(MPL3115A2_ADDRESS, MPL3115A2_OUT_P_MSB, 5, &rawData[0]);  // Read the five raw data registers into data array

// Pressure bytes
  uint8_t msbP = rawData[0];
  uint8_t csbP = rawData[1];
  uint8_t lsbP = rawData[2];
// Temperature bytes
  uint8_t msbT = rawData[3];
  uint8_t lsbT = rawData[4]; 
 
// Calculate pressure 
   int32_t pressure_whole =   ((int32_t)msbP << 16 |  (int32_t)csbP << 8 |  (int32_t)lsbP) ; // Construct whole number pressure
  pressure_whole >>= 6; // Only two most significant bits of lsbP contribute to whole pressure; its an 18-bit number
 
  lsbP &= 0x30; // Keep only bits 5 and 6, the fractional pressure
  lsbP >>= 4; // Shift to get the fractional pressure in terms of quarters of a Pascal
  float pressure_frac = (float) lsbP/4.0; // Convert numbers of fractional quarters to fractional pressure n Pasacl

  pressure = (float) (pressure_whole) + pressure_frac; // Combine whole and fractional parts to get entire pressure in Pascal

 // Calculate temperature 
  int16_t temperature_whole = ((int16_t)msbT << 8 | lsbT ) ; // Construct signed 16-bit whole number temperature
  temperature_whole >>= 8;

  lsbT &= 0xF0; // Keep only bits 5 - 7, the fractional temperature
  lsbT >>= 4; // Shift to get the fractional temperature
  float temperature_frac = (float) lsbT/16.0; // Convert to fractional temperature in Centigrade

  temperature = (float) (temperature_whole) + temperature_frac; // Combine whole and fractional parts to get entire temperature in Centigrade
  
  Serial.print(pressure/100.); Serial.print(", "); Serial.println(temperature);
}

/**
 * Sets sea level pressure
 * 
*/
void MPL3115A2::setSeaPress(float sea_press_inp) {

	def_sea_press = sea_press_inp;
}

/*
=====================================================================================================
Define functions according to 
"Data Manipulation and Basic Settings of the MPL3115A2 Command Line Interface Drive Code"
by Miguel Salhuana
Freescale Semiconductor Application Note AN4519 Rev 0.1, 08/2012
=====================================================================================================
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Clears then sets OST bit which causes the sensor to immediately take another reading
void MPL3115A2::MPL3115A2toggleOneShot()
{
    MPL3115A2Active();  // Set to active to start reading
    uint8_t c = readByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1);
    writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1, c & ~(1<<1)); // Clear OST (bit 1)
    c = readByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1);
    writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1, c | (1<<1)); // Set OST bit to 1
}
    
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Set the Outputting Sample Rate
void MPL3115A2::MPL3115A2SampleRate(uint8_t samplerate)
{
  MPL3115A2Standby();  // Must be in standby to change registers

  uint8_t c = readByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1);
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1, c & ~(0x38)); // Clear OSR bits 3,4,5
  if(samplerate < 8) { // OSR between 1 and 7
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1, c | (samplerate << 3));  // Write OSR to bits 3,4,5
  }
  
  MPL3115A2Active();  // Set to active to start reading
 }
 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Initialize the MPL3115A2 registers for FIFO mode
void MPL3115A2::initFIFOMPL3115A2()
{
  // Clear all interrupts by reading the data output registers
  uint8_t temp;
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_P_MSB);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_P_CSB);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_P_LSB);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_T_MSB);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_T_LSB);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_F_STATUS);
  
   MPL3115A2Standby();  // Must be in standby to change registers
  
  // Set CTRL_REG4 register to configure interupt enable
  // Enable data ready interrupt (bit 7), enable FIFO (bit 6), enable pressure window (bit 5), temperature window (bit 4),
  // pressure threshold (bit 3), temperature threshold (bit 2), pressure change (bit 1) and temperature change (bit 0)
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG4, 0x40);  // enable FIFO
  
  //  Configure INT 1 for data ready, all other (inc. FIFO) interrupts to INT2
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG5, 0x80); 
  
  // Set CTRL_REG3 register to configure interupt signal type
  // Active HIGH, push-pull interupts INT1 and INT 2
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG3, 0x22); 
  
  // Set FIFO mode
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_F_SETUP, 0x00); // Clear FIFO mode
// In overflow mode, when FIFO fills up, no more data is taken until the FIFO registers are read
// In watermark mode, the oldest data is overwritten by new data until the FIFO registers are read
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_F_SETUP, 0x80); // Set F_MODE to interrupt when overflow = 32 reached
//  writeByte(MPL3115A2_ADDRESS, F_SETUP, 0x60); // Set F_MODE to accept 32 data samples and interrupt when watermark = 32 reached

  MPL3115A2Active();  // Set to active to start reading
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Initialize the MPL3115A2 for realtime data collection 
void MPL3115A2::initRealTimeMPL3115A2()
{
  // Clear all interrupts by reading the data output registers
  uint8_t temp;
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_P_MSB);
	Serial.println(temp,HEX);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_P_CSB);
    Serial.println(temp,HEX);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_P_LSB);
    Serial.println(temp,HEX);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_T_MSB);
    Serial.println(temp,HEX);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_OUT_T_LSB);
    Serial.println(temp,HEX);
  temp = readByte(MPL3115A2_ADDRESS, MPL3115A2_F_STATUS);
    Serial.println(temp,HEX);

  
   MPL3115A2Standby();  // Must be in standby to change registers
  
  // Set CTRL_REG4 register to configure interupt enable
  // Enable data ready interrupt (bit 7), enable FIFO (bit 6), enable pressure window (bit 5), temperature window (bit 4),
  // pressure threshold (bit 3), temperature threshold (bit 2), pressure change (bit 1) and temperature change (bit 0)
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG4, 0x80);  
  
  //  Configure INT 1 for data ready, all other interrupts to INT2
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG5, 0x80); 
  
  // Set CTRL_REG3 register to configure interupt signal type
  // Active HIGH, push-pull interupts INT1 and INT 2
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG3, 0x22); 
  
  // Set FIFO mode
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_F_SETUP, 0x00); // disable FIFO mode
  
  MPL3115A2Active();  // Set to active to start reading
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Set the Auto Acquisition Time Step
void MPL3115A2::MPL3115A2TimeStep(uint8_t ST_Value)
{
 MPL3115A2Standby(); // First put device in standby mode to allow write to registers
 
 uint8_t c = readByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG2); // Read contents of register CTRL_REG2
 if (ST_Value <= 0xF) {
 writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG2, (c | ST_Value)); // Set time step n from 0x0 to 0xF (bits 0 - 3) for time intervals from 1 to 32768 (2^n) seconds
 }
 
 MPL3115A2Active(); // Set to active to start reading
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Enable the pressure and temperature event flags
 // Bit 2 is general data ready event mode on new Pressure/Altitude or temperature data
 // Bit 1 is event flag on new Pressure/Altitude data
 // Bit 0 is event flag on new Temperature data
void MPL3115A2::MPL3115A2enableEventflags()
{
  MPL3115A2Standby();  // Must be in standby to change registers
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_PT_DATA_CFG, 0x07); //Enable all three pressure and temperature event flags
  MPL3115A2Active();  // Set to active to start reading
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Enter Active Altimeter mode
void MPL3115A2::MPL3115A2ActiveAltimeterMode()
{
 MPL3115A2Standby(); // First put device in standby mode to allow write to registers
 uint8_t c = readByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1); // Read contents of register CTRL_REG1
 writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1, c | (0x80)); // Set ALT (bit 7) to 1
 MPL3115A2Active(); // Set to active to start reading
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Enter Active Barometer mode
void MPL3115A2::MPL3115A2ActiveBarometerMode()
{
 MPL3115A2Standby(); // First put device in standby mode to allow write to registers
 uint8_t c = readByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1); // Read contents of register CTRL_REG1
 writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1, c & ~(0x80)); // Set ALT (bit 7) to 0
 MPL3115A2Active(); // Set to active to start reading
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Software resets the MPL3115A2.
// It must be in standby to change most register settings
void MPL3115A2::MPL3115A2Reset()
{
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1, 0x04); // Set RST (bit 2) to 1
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Sets the MPL3115A2 to standby mode.
// It must be in standby to change most register settings
void MPL3115A2::MPL3115A2Standby()
{
  uint8_t c = readByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1); // Read contents of register CTRL_REG1
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1, c & ~(0x01)); // Set SBYB (bit 0) to 0
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Sets the MPL3115A2 to active mode.
// Needs to be in this mode to output data
void MPL3115A2::MPL3115A2Active()
{
  uint8_t c = readByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1); // Read contents of register CTRL_REG1
  writeByte(MPL3115A2_ADDRESS, MPL3115A2_CTRL_REG1, c | 0x01); // Set SBYB (bit 0) to 1
}

// I2C read/write functions 
void MPL3115A2::writeByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
  Wire.beginTransmission(address);  // Initialize the Tx buffer
  Wire.write(subAddress);           // Put slave register address in Tx buffer
  Wire.write(data);                 // Put data in Tx buffer
  Wire.endTransmission();           // Send the Tx buffer
}

uint8_t MPL3115A2::readByte(uint8_t address, uint8_t subAddress)
{
  uint8_t data; // `data` will store the register data   
  Wire.beginTransmission(address);         // Initialize the Tx buffer
  Wire.write(subAddress);                  // Put slave register address in Tx buffer
  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  //Wire.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, (size_t) 1);   // Read one byte from slave register address 
  data = Wire.read();                      // Fill Rx buffer with result
  return data;                             // Return data read from slave register
}

void MPL3115A2::readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{  
  Wire.beginTransmission(address);   // Initialize the Tx buffer
  Wire.write(subAddress);            // Put slave register address in Tx buffer
  Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
  //Wire.endTransmission(I2C_NOSTOP);             // Send the Tx buffer, but send a restart to keep connection alive  uint8_t i = 0;
        uint8_t i = 0;
        Wire.requestFrom(address, (size_t) count);  // Read bytes from slave register address 
  while (Wire.available()) {
        dest[i++] = Wire.read(); }         // Put read results in the Rx buffer

}