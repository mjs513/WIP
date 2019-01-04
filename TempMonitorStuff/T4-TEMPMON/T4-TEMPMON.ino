#include "Arduino.h"
#include "tempmon.h"

//Note to self
//(PSU_MISC1 >> 27U) & 1U

uint16_t frequency;      /*!< The temperature measure frequency.*/
uint32_t highAlarmTemp;  /*!< The high alarm temperature.*/
uint32_t panicAlarmTemp; /*!< The panic alarm temperature.*/
uint32_t lowAlarmTemp;   /*!< The low alarm */

float temperature = 0.0f;
volatile bool TempAlarm = false;


void setup() {
  while ( !Serial);
    delay(2000);
    attachInterruptVector(IRQ_TEMPERATURE, &High_Low_Temp_isr);
    NVIC_ENABLE_IRQ(IRQ_TEMPERATURE);
    //attachInterruptVector(IRQ_TEMPERATURE_PANIC, &Panic_Temp_isr);
    //NVIC_ENABLE_IRQ(IRQ_TEMPERATURE_PANIC); 

    Serial.println("TEMPMON driver example.");

    frequency = 0x03U;      //updates the temp value at a RTC/3 clock rate
                            //0xFFFF determines a 2 second sample rate period
    highAlarmTemp   = 49U;  //42 degrees C
    lowAlarmTemp    = 25U;
    panicAlarmTemp  = 90U;

    initTempMon(frequency, lowAlarmTemp, highAlarmTemp, panicAlarmTemp);
    tStartMeasure();        //Start measurements

    /* Get temperature */
    temperature = tGetTemp();

    Serial.print("The chip initial temperature is ");
    Serial.println(temperature);

}

void loop() {
  while(!Serial && millis()<4000);
  delay(1000);
  temperature = tGetTemp();
   
  /* Based on testing the only register that is tripped is
   *  the high temper register in CCM_ANALOG_MISC1/PMU_MISC1)
   *  
   *  
   */

 
  if (TempAlarm && ((temperature - highAlarmTemp > 0)) && (temperature - panicAlarmTemp < 0) )
  {
      TempAlarm = false;

      Serial.printf("HIGH Temperature Alarm %.1f. \r\n", temperature);
  }

  if (TempAlarm && (temperature - lowAlarmTemp < 0))
  {
      TempAlarm = false;

      Serial.printf("LOW Temperature Alarm %.1f. \r\n", temperature);
  }
  
}

void High_Low_Temp_isr(void)
{
    TempAlarm = true;
}
/*
void Panic_Temp_isr(void)
{
    TempAlarm = true;
}
*/
