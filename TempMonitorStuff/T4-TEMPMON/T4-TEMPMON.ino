#include "Arduino.h"
#include "tempmon.h"

//DEMO_TEMP_LOW_HIGH_IRQn  63
//DEMO_TEMP_PANIC_IRQn  64
//IRQ_TEMPERATURE =       63
//IRQ_TEMPERATURE_PANIC = 64
//NVIC_ENABLE_IRQ(n)

uint16_t frequency;      /*!< The temperature measure frequency.*/
uint32_t highAlarmTemp;  /*!< The high alarm temperature.*/
uint32_t panicAlarmTemp; /*!< The panic alarm temperature.*/
uint32_t lowAlarmTemp;   /*!< The low alarm */

float temperature = 0.0f;
volatile bool temperatureReach = false;


void setup() {
  while ( !Serial);
    delay(2000);
    attachInterruptVector(IRQ_TEMPERATURE, &High_Low_Temp_isr);
    NVIC_ENABLE_IRQ(IRQ_TEMPERATURE);
    attachInterruptVector(IRQ_TEMPERATURE_PANIC, &Panic_Temp_isr);
    NVIC_ENABLE_IRQ(IRQ_TEMPERATURE_PANIC); 

    Serial.println("TEMPMON driver example.");

    frequency = 0x03U;    //updates the temp value at a RTC/3 clock rate
                            //0xFFFF determines a 2 second sample rate period
    highAlarmTemp   = 75U;  //42 degrees C
    lowAlarmTemp    = 25U;
    panicAlarmTemp  = 85U;

    initTempMon(frequency, highAlarmTemp, lowAlarmTemp, panicAlarmTemp);
    tStartMeasure();        //Start measurements

    /* Get temperature */
    temperature = tGetTemp();

    Serial.print("The chip initial temperature is ");
    Serial.println(temperature);

}

void loop() {
  delay(1000);
  temperature = tGetTemp();
  Serial.printf("temperature is %.1f \r\n", temperature);
  //Serial.print("CCM_ANALOG_MISC1 bit low: ");
  //Serial.println(CCM_ANALOG_MISC1,BIN);
  /*  Checking if alarm was tripped  
  Serial.print("CCM_ANALOG_MISC1 bit low: ");
  Serial.println((CCM_ANALOG_MISC1 >> 28U) & 1U);

  Serial.print("CCM_ANALOG_MISC1 bit high: ");
  Serial.println((CCM_ANALOG_MISC1 >> 29U) & 1U);

  Serial.print("CCM_ANALOG_MISC1 bit Panic: ");
  Serial.println((CCM_ANALOG_MISC1 >> 27U) & 1U);
  Serial.println();
  */
}

void High_Low_Temp_isr(void)
{
    //temperatureReach = true;
    temperature = tGetTemp();
    Serial.printf("High/Low temp alarm tripped %.1f\r\n", temperature); 
}

void Panic_Temp_isr(void)
{
    //temperatureReach = true;
    temperature = tGetTemp();
    Serial.printf("Panic temp alarm tripped %.1f\r\n", temperature); 
}
