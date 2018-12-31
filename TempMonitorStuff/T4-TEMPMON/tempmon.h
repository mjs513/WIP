#include "Arduino.h"


#define TMS0_POWER_DOWN_MASK        (0x1U)
#define TMS0_POWER_DOWN_SHIFT       (0U)

#define TMS1_MEASURE_FREQ(x)        (((uint32_t)(((uint32_t)(x)) << 0U)) & 0xFFFFU)

#define TMS0_ALARM_VALUE(x)         (((uint32_t)(((uint32_t)(x)) << 20U)) & 0xFFF00000U)
#define TMS02_LOW_ALARM_VALUE(x)    (((uint32_t)(((uint32_t)(x)) << 0U)) & 0xFFFU)
#define TMS02_PANIC_ALARM_VALUE(x)  (((uint32_t)(((uint32_t)(x)) << 16U)) & 0xFFF0000U)

typedef enum _tempmon_alarm_mode
{
    kTEMPMON_HighAlarmMode = 0U,  /*!< The high alarm temperature interrupt mode.*/
    kTEMPMON_PanicAlarmMode = 1U, /*!< The panic alarm temperature interrupt mode.*/
    kTEMPMON_LowAlarmMode = 2U,   /*!< The low alarm temperature interrupt mode.*/
} tempmon_alarm_mode;

void initTempMon(uint16_t freq, uint32_t highAlarmTemp, uint32_t panicAlarmTemp, uint32_t lowAlarmTemp);
void tSetTempAlarm(uint32_t tempVal, tempmon_alarm_mode alarmMode);

static inline void tStartMeasure()
{
    TEMPMON_TEMPSENSE0 |= 0x2U;
}

static inline void tStopMeasure()
{
    TEMPMON_TEMPSENSE0 &= ~0x2U;
}

static inline void tPowerDown()
{
    TEMPMON_TEMPSENSE0 |= 0x1U;
}

float tGetTemp();
 
