#include "tempmon.h"

#define TEMPMON_ROOMTEMP 25.0f

static uint32_t s_hotTemp;    /*!< The value of TEMPMON_TEMPSENSE0[TEMP_VALUE] at room temperature .*/
static uint32_t s_hotCount;   /*!< The value of TEMPMON_TEMPSENSE0[TEMP_VALUE] at the hot temperature.*/
static uint32_t roomCount;   /*!< The value of TEMPMON_TEMPSENSE0[TEMP_VALUE] at the hot temperature.*/

static float s_hotT_ROOM;     /*!< The value of s_hotTemp minus room temperature(25¡æ).*/
static uint32_t s_roomC_hotC; /*!< The value of s_roomCount minus s_hotCount.*/

void initTempMon(uint16_t freq, uint32_t highAlarmTemp, uint32_t panicAlarmTemp, uint32_t lowAlarmTemp)
{
  
  uint32_t calibrationData;
  uint32_t roomCount;
    
  //first power on the temperature sensor
  TEMPMON_TEMPSENSE0 &= ~TMS0_POWER_DOWN_MASK;
  Serial4.print("TEMPMON_TEMPSENSE0 (Pwr Up): ");
  Serial4.println(TEMPMON_TEMPSENSE0, BIN);
  //Serial4.printf("CCM_ANALOG_PLL_USB1=%08lX\n", n);

  //set monitoring frequency
  TEMPMON_TEMPSENSE1 = TMS1_MEASURE_FREQ(freq);
  Serial4.print("Set Freq (TEMPMON_TEMPSENSE1):  ");
  Serial4.print(TMS1_MEASURE_FREQ(freq), HEX); Serial4.println(", ");
  Serial4.println(TEMPMON_TEMPSENSE1, BIN);
  
  //read calibration data
  calibrationData = HW_OCOTP_ANA1;
    s_hotTemp = (uint32_t)(calibrationData & 0xFFU) >> 0x00U;
    s_hotCount = (uint32_t)(calibrationData & 0xFFF00U) >> 0X08U;
    roomCount = (uint32_t)(calibrationData & 0xFFF00000U) >> 0x14U;
  Serial4.printf("CAL DATA (HT,HTC,RC):  %d, %d, %d\r\n", s_hotTemp, s_hotCount, roomCount);
    s_hotT_ROOM = s_hotTemp - TEMPMON_ROOMTEMP;
    s_roomC_hotC = roomCount - s_hotCount;
  Serial4.printf("s_hotRoom, s_roomc_hotC:  %.1f, %d\r\n", s_hotT_ROOM, s_roomC_hotC);

    //time to set alarm temperatures
    tSetTempAlarm(highAlarmTemp, kTEMPMON_HighAlarmMode);
  Serial4.println(TEMPMON_TEMPSENSE1, BIN);
    tSetTempAlarm(panicAlarmTemp, kTEMPMON_PanicAlarmMode);
  Serial4.println(TEMPMON_TEMPSENSE2, BIN);
    tSetTempAlarm(lowAlarmTemp, kTEMPMON_LowAlarmMode);
  Serial4.println(TEMPMON_TEMPSENSE2, BIN);
}

void tSetTempAlarm(uint32_t tempVal, tempmon_alarm_mode alarmMode)
{
    uint32_t tempCodeVal;

    /* Calculate alarm temperature code value */
    tempCodeVal = (uint32_t)(s_hotCount + (s_hotTemp - tempVal) * s_roomC_hotC / s_hotT_ROOM);

    switch (alarmMode)
    {
        case kTEMPMON_HighAlarmMode:
            /* Set high alarm temperature code value */
            TEMPMON_TEMPSENSE0 |= TMS0_ALARM_VALUE(tempCodeVal);
            break;

        case kTEMPMON_PanicAlarmMode:
            /* Set panic alarm temperature code value */
            TEMPMON_TEMPSENSE2 |= TMS02_PANIC_ALARM_VALUE(tempCodeVal);
            break;

        case kTEMPMON_LowAlarmMode:
            /* Set low alarm temperature code value */
            TEMPMON_TEMPSENSE2 |= TMS02_LOW_ALARM_VALUE(tempCodeVal);
            break;

        default:
            break;
            break;
    }
}

float tGetTemp()
{
    uint32_t nmeas;
    float tmeas;

    while (!(TEMPMON_TEMPSENSE0 & 0x4U))
    {
    }

    /* ready to read temperature code value */
    nmeas = (TEMPMON_TEMPSENSE0 & 0xFFF00U) >> 8U;

    /* Calculate temperature */
    tmeas = s_hotTemp - (float)((nmeas - s_hotCount) * s_hotT_ROOM / s_roomC_hotC);

    return tmeas;
}
