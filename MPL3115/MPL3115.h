#ifndef MPL3115_h
#define MPL3115_h

// Freescale Semiconductor MPL3115A2 Precision I2C altimeter 
// http://www.nxp.com/files/sensors/doc/data_sheet/MPL3115A2.pdf
// Register defines courtesy A. Weiss and Nathan Seidle, SparkFun Electronics
#define MPL3115A2_STATUS           0x00
#define MPL3115A2_OUT_P_MSB        0x01
#define MPL3115A2_OUT_P_CSB        0x02
#define MPL3115A2_OUT_P_LSB        0x03
#define MPL3115A2_OUT_T_MSB        0x04
#define MPL3115A2_OUT_T_LSB        0x05
#define MPL3115A2_DR_STATUS        0x06
#define MPL3115A2_OUT_P_DELTA_MSB  0x07
#define MPL3115A2_OUT_P_DELTA_CSB  0x08
#define MPL3115A2_OUT_P_DELTA_LSB  0x09
#define MPL3115A2_OUT_T_DELTA_MSB  0x0A
#define MPL3115A2_OUT_T_DELTA_LSB  0x0B
#define MPL3115A2_WHO_AM_I         0x0C
#define MPL3115A2_F_STATUS         0x0D
#define MPL3115A2_F_DATA           0x0E
#define MPL3115A2_F_SETUP          0x0F
#define MPL3115A2_TIME_DLY         0x10
#define MPL3115A2_SYSMOD           0x11
#define MPL3115A2_INT_SOURCE       0x12
#define MPL3115A2_PT_DATA_CFG      0x13
#define MPL3115A2_BAR_IN_MSB       0x14 // Set at factory to equivalent sea level pressure for measurement location, generally no need to change
#define MPL3115A2_BAR_IN_LSB       0x15 // Set at factory to equivalent sea level pressure for measurement location, generally no need to change
#define MPL3115A2_P_TGT_MSB        0x16
#define MPL3115A2_P_TGT_LSB        0x17
#define MPL3115A2_T_TGT            0x18
#define MPL3115A2_P_WND_MSB        0x19
#define MPL3115A2_P_WND_LSB        0x1A
#define MPL3115A2_T_WND            0x1B
#define MPL3115A2_P_MIN_MSB        0x1C
#define MPL3115A2_P_MIN_CSB        0x1D
#define MPL3115A2_P_MIN_LSB        0x1E
#define MPL3115A2_T_MIN_MSB        0x1F
#define MPL3115A2_T_MIN_LSB        0x20
#define MPL3115A2_P_MAX_MSB        0x21
#define MPL3115A2_P_MAX_CSB        0x22
#define MPL3115A2_P_MAX_LSB        0x23
#define MPL3115A2_T_MAX_MSB        0x24
#define MPL3115A2_T_MAX_LSB        0x25
#define MPL3115A2_CTRL_REG1        0x26
#define MPL3115A2_CTRL_REG2        0x27
#define MPL3115A2_CTRL_REG3        0x28
#define MPL3115A2_CTRL_REG4        0x29
#define MPL3115A2_CTRL_REG5        0x2A
#define MPL3115A2_OFF_P            0x2B
#define MPL3115A2_OFF_T            0x2C
#define MPL3115A2_OFF_H            0x2D

#define MPL3115A2_ADDRESS  0x60  

enum SAMPLERATE {
  OS_6ms = 0, // 6 ms is minimum oversampling interval, corresponds to an oversample ration of 2^0 = 1 
  OS_10ms,
  OS_18ms,
  OS_34ms,
  OS_66ms,
  OS_130ms, // 130 ms oversampling interval, 2^5 = 32 oversample ratio
  OS_258ms,
  OS_512ms
};

enum ST_VALUE {
  ATS_1s = 0, // 6 ms is minimum oversampling interval, corresponds to an oversample ration of 2^0 = 1 
  ATS_2s,
  ATS_4s,
  ATS_8s,
  ATS_16s,
  ATS_32s,
  ATS_64s, // 2^6 = 64 s interval between up to 32 FIFO samples for half an hour of data logging
  ATS_128s,
  ATS_256s,
  ATS_512s,
  ATS_1024s,
  ATS_2048s,
  ATS_4096s,
  ATS_8192s,
  ATS_16384s,
  ATS_32768s  // 2^15 = 32768 s interval between up to 32 FIFO samples = 12 days of data logging!
};


class MPL3115A2
{
	public:
	  MPL3115A2();

	  //Public Functions
	  void begin(); // Gets sensor on the I2C bus.
	  void init();
	    void getBaro();
		void MPL3115A2readAltitude();
		void MPL3115A2readPressure();
		void MPL3115A2toggleOneShot();
		void MPL3115A2SampleRate(uint8_t samplerate);
		void initFIFOMPL3115A2();
		void initRealTimeMPL3115A2();
		void MPL3115A2TimeStep(uint8_t ST_Value);
		void MPL3115A2enableEventflags();
		void MPL3115A2ActiveAltimeterMode();
		void MPL3115A2ActiveBarometerMode();
		void MPL3115A2Reset();
		void MPL3115A2Standby();
		void MPL3115A2Active();
		void setSeaPress(float sea_press_inp);

		// Specify MPL3115 Altimeter settings
		uint8_t SAMPLERATE = OS_34ms;
		uint8_t ST_VALUE   = ATS_4s;
		int AltimeterMode  = 0;        // use to choose between altimeter and barometer modes for FIFO data
		float pressure, temperature, altitude, altimeter_setting_pressure_mb;
		float def_sea_press = 1050.0;

//private:
		void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
		uint8_t readByte(uint8_t address, uint8_t subAddress);
		void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);
};

#endif