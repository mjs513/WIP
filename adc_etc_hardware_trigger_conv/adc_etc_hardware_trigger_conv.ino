/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 * Teensy specific coded added to example
 */
 #include "Arduino.h"

#include <stdint.h>
//#include "defines.h"
#include "fsl_adc_etc.h"

#define PRINTF Serial.printf

/*******************************************************************************
 * Definitions
 ******************************************************************************/


//#define ADC_ETC_BASE 0x403B0000
#define ADC_USER_CHANNEL 16U
#define ADC_CHANNEL_GROUP0 0U
#define ADC_CHANNEL_GROUP1 1U

#define ADC_ETC_CHAIN_LENGTH 1U /* Chain length is 2. */
#define ADC_ETC_CHANNEL0 15U
#define ADC_ETC_CHANNEL1 0U

static uint8_t calibrating;


/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_AdcConversionValue0;
volatile uint32_t g_AdcConversionValue1;

/*******************************************************************************
* Code
******************************************************************************/
void ADC_ETC_DONE0_Handler(void)
{
    ADC_ETC_ClearInterruptStatusFlags(ADC_ETC, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done0StatusFlagMask);
    g_AdcConversionValue0 = ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, 0U); /* Get trigger0 chain0 result. */
    asm("dsb");
}

void ADC_ETC_DONE1_Handler(void)
{
    ADC_ETC_ClearInterruptStatusFlags(ADC_ETC, kADC_ETC_Trg0TriggerSource, kADC_ETC_Done1StatusFlagMask);
    g_AdcConversionValue1 = ADC_ETC_GetADCConversionValue(ADC_ETC, 0U, 1U); /* Get trigger0 chain1 result. */
    asm("dsb");
}


void setup(void)
{
  Serial.begin(115200);
  delay(3000);
  
    adc_etc_config_t adcEtcConfig;
    adc_etc_trigger_config_t adcEtcTriggerConfig;
    adc_etc_trigger_chain_config_t adcEtcTriggerChainConfig;

    PRINTF("ADC_ETC_Hardware_Trigger_Conv Example Start!\r\n");

    ADC_Init();
    XBARA_Init();
    PIT_Init(24 * 1000000);

    attachInterruptVector(IRQ_ADC_ETC0, ADC_ETC_DONE0_Handler);
    NVIC_ENABLE_IRQ(IRQ_ADC_ETC0);
    attachInterruptVector(IRQ_ADC_ETC1, ADC_ETC_DONE1_Handler);
    NVIC_ENABLE_IRQ(IRQ_ADC_ETC1);
    
    /* Initialize the ADC_ETC. */
    ADC_ETC_GetDefaultConfig(&adcEtcConfig);
    adcEtcConfig.XBARtriggerMask = 1U; /* Enable the external XBAR trigger0. */
    ADC_ETC_Init(ADC_ETC, &adcEtcConfig);

    /* Set the external XBAR trigger0 configuration. */
    adcEtcTriggerConfig.enableSyncMode = false;
    adcEtcTriggerConfig.enableSWTriggerMode = false;
    adcEtcTriggerConfig.triggerChainLength = ADC_ETC_CHAIN_LENGTH; /* Chain length is 2. */
    
    adcEtcTriggerConfig.triggerPriority = 0U;
    adcEtcTriggerConfig.sampleIntervalDelay = 0U;
    adcEtcTriggerConfig.initialDelay = 0U;
    ADC_ETC_SetTriggerConfig(ADC_ETC, 0U, &adcEtcTriggerConfig);

    /* Set the external XBAR trigger0 chain configuration. */
    adcEtcTriggerChainConfig.enableB2BMode = true;
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U
                                                   << ADC_CHANNEL_GROUP0; /* Select ADC_HC0 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect =
        ADC_ETC_CHANNEL0; /* ADC_HC0 will be triggered to sample Corresponding channel. */
        
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done0InterruptEnable; /* Enable the Done0 interrupt. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 0U,
                                  &adcEtcTriggerChainConfig); /* Configure the trigger0 chain0. */
    adcEtcTriggerChainConfig.ADCHCRegisterSelect = 1U
                                                   << ADC_CHANNEL_GROUP1; /* Select ADC_HC1 register to trigger. */
    adcEtcTriggerChainConfig.ADCChannelSelect =
        ADC_ETC_CHANNEL1; /* ADC_HC1 will be triggered to sample Corresponding channel. */
    adcEtcTriggerChainConfig.InterruptEnable = kADC_ETC_Done1InterruptEnable; /* Enable the Done1 interrupt. */
    ADC_ETC_SetTriggerChainConfig(ADC_ETC, 0U, 1U,
                                  &adcEtcTriggerChainConfig); /* Configure the trigger0 chain1. */
}


void loop()
{
    PRINTF("ADC conversion vaule is %d and %d\r\n", g_AdcConversionValue0, g_AdcConversionValue1);
}

/*!
* @brief Configure ADC to working with ADC_ETC.
*/
void ADC_Init(void)
{
    // init and calibrate with help from core
    analogReadResolution(12);
    analogRead(0);
    analogRead(1);
    ADC1_CFG |= ADC_CFG_ADTRG;   // hardware trigger
    // ADC1_CFG = 0x200b;
    ADC1_HC0 = ADC_USER_CHANNEL;   // ADC_ETC channel
    ADC1_HC1 = ADC_USER_CHANNEL;
    
    /* Clear the CALF and launch the calibration. */
    ADC1_GS = 0x2; /* Clear the CALF. */
    ADC1_GC |= 0x80; /* Launch the calibration. */
    
    calibrating = 1;
    
    wait_for_cal();
}

/*!
* @brief Configure XBARA to work with ADC_ETC.
*/
void XBARA_Init(void)
{
    CCM_CCGR2 |= CCM_CCGR2_XBAR1(CCM_CCGR_ON);   //turn clock on for xbara1

    /* Configure the XBARA signal connections. */
    xbara_connect(kXBARA1_InputPitTrigger0, kXBARA1_OutputAdcEtcXbar0Trig0);
}

/*!
* @brief Configuration PIT to trigger ADC_ETC.
*/
void PIT_Init(uint32_t cycles)
{
  CCM_CCGR1 |= CCM_CCGR1_PIT(CCM_CCGR_ON);
  PIT_MCR = 0;

  IMXRT_PIT_CHANNELS[0].LDVAL = cycles;
  IMXRT_PIT_CHANNELS[0].TCTRL = PIT_TCTRL_TEN;
}

static void wait_for_cal(void)
{
  //printf("wait_for_cal\n");
  while (ADC1_GC & ADC_GC_CAL) ;
  // TODO: check CALF, but what do to about CAL failure?
  calibrating = 0;
  //printf("cal complete\n");
}

void xbara_connect(unsigned int input, unsigned int output)
{
  if (input >= 88) return;
  if (output >= 132) return;
#if 1
  volatile uint16_t *xbar = &XBARA1_SEL0 + (output / 2);
  uint16_t val = *xbar;
  if (!(output & 1)) {
    val = (val & 0xFF00) | input;
  } else {
    val = (val & 0x00FF) | (input << 8);
  }
  *xbar = val;
#else
  // does not work, seems 8 bit access is not allowed
  volatile uint8_t *xbar = (volatile uint8_t *)XBARA1_SEL0;
  xbar[output] = input;
#endif
}
