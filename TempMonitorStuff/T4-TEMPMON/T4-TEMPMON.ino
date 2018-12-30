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
    Serial4.begin(115200);
    delay(2000);
    //NVIC_ENABLE_IRQ(IRQ_TEMPERATURE);
    //NVIC_ENABLE_IRQ(IRQ_TEMPERATURE_PANIC);
    Serial4.println("TEMPMON driver example.");

    frequency = 0x03U;    //updates the temp value at a RTC/3 clock rate
                            //0xFFFF determines a 2 second sample rate period
    highAlarmTemp   = 42U;  //42 degrees C
    lowAlarmTemp    = 40U;
    panicAlarmTemp  = 80U;

    initTempMon(frequency, highAlarmTemp, lowAlarmTemp, panicAlarmTemp);
    tStartMeasure();        //Start measurements
    
    /* Get temperature */
    temperature = tGetTemp();

    Serial4.print("The chip initial temperature is ");
    Serial4.println(temperature);

}

void loop() {
  delay(100);
  temperature = tGetTemp();
  Serial4.printf("temperature is %.1f \r\n", temperature);

}
/*
void temperature_isr(void)
{
    //temperatureReach = true;
    temperature = tGetTemp();
    Serial4.printf("High/Low temp alarm tripped %.1f\r\n", temperature); 
}

void temperature_panic_isr(void)
{
    //temperatureReach = true;
    temperature = tGetTemp();
    Serial4.printf("Panic temp alarm tripped %.1f\r\n", temperature); 
}
*/
