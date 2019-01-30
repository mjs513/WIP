/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_adc_etc.h"



/*!
* brief Initialize the ADC_ETC module.
*
* param base ADC_ETC peripheral base address.
* param config Pointer to "adc_etc_config_t" structure.
*/
void ADC_ETC_Init(ADC_ETC_Type *base, const adc_etc_config_t *config)
{
    //assert(NULL != config);

    uint32_t tmp32;


    /* Open clock gate. */
    ///CLOCK_EnableClock(s_adcetcClocks[ADC_ETC_GetInstance(base)]);
    // to do enable adc etc clok
    
    /* Disable software reset. */
    ADC_ETC_DoSoftwareReset(base, false);

    /* Set ADC_ETC_CTRL register. */
    tmp32 = ADC_ETC_CTRL_EXT0_TRIG_PRIORITY(config->TSC0triggerPriority) |
            ADC_ETC_CTRL_EXT1_TRIG_PRIORITY(config->TSC1triggerPriority) |
            ADC_ETC_CTRL_PRE_DIVIDER(config->clockPreDivider) | ADC_ETC_CTRL_TRIG_ENABLE(config->XBARtriggerMask)
            | ADC_ETC_CTRL_DMA_MODE_SEL(config->dmaMode);
    if (config->enableTSCBypass)
    {
        tmp32 |= ADC_ETC_CTRL_TSC_BYPASS_MASK;
    }
    if (config->enableTSC0Trigger)
    {
        tmp32 |= ADC_ETC_CTRL_EXT0_TRIG_ENABLE_MASK;
    }
    if (config->enableTSC1Trigger)
    {
        tmp32 |= ADC_ETC_CTRL_EXT1_TRIG_ENABLE_MASK;
    }
    base->CTRL = tmp32;
}


/*!
* brief Gets an available pre-defined settings for the ADC_ETC's configuration.
* This function initializes the ADC_ETC's configuration structure with available settings. The default values are:
* code
*   config->enableTSCBypass = true;
*   config->enableTSC0Trigger = false;
*   config->enableTSC1Trigger = false;
*   config->TSC0triggerPriority = 0U;
*   config->TSC1triggerPriority = 0U;
*   config->clockPreDivider = 0U;
*   config->XBARtriggerMask = 0U;
* endCode
*
* param config Pointer to "adc_etc_config_t" structure.
*/
void ADC_ETC_GetDefaultConfig(adc_etc_config_t *config)
{
    /* Initializes the configure structure to zero. */
    memset(config, 0, sizeof(*config));
	
    config->enableTSCBypass = true;
    config->enableTSC0Trigger = false;
    config->enableTSC1Trigger = false;
    config->dmaMode = kADC_ETC_TrigDMAWithLatchedSignal;
    config->TSC0triggerPriority = 0U;
    config->TSC1triggerPriority = 0U;
    config->clockPreDivider = 0U;
    config->XBARtriggerMask = 0U;
}

/*!
* brief Set the external XBAR trigger configuration.
*
* param base ADC_ETC peripheral base address.
* param triggerGroup Trigger group index.
* param config Pointer to "adc_etc_trigger_config_t" structure.
*/
void ADC_ETC_SetTriggerConfig(ADC_ETC_Type *base, uint32_t triggerGroup, const adc_etc_trigger_config_t *config)
{
    //assert(triggerGroup < ADC_ETC_TRIGn_CTRL_COUNT);
    //assert(ADC_ETC_TRIGn_COUNTER_COUNT > triggerGroup);

    uint32_t tmp32;

    /* Set ADC_ETC_TRGn_CTRL register. */
    tmp32 = ADC_ETC_TRIGn_CTRL_TRIG_CHAIN(config->triggerChainLength) |
            ADC_ETC_TRIGn_CTRL_TRIG_PRIORITY(config->triggerPriority);
    if (config->enableSyncMode)
    {
        tmp32 |= ADC_ETC_TRIGn_CTRL_SYNC_MODE_MASK;
    }
    if (config->enableSWTriggerMode)
    {
        tmp32 |= ADC_ETC_TRIGn_CTRL_TRIG_MODE_MASK;
    }
    base->TRIG[triggerGroup].TRIGn_CTRL = tmp32;

    /* Set ADC_ETC_TRGn_COUNTER register. */
    tmp32 = ADC_ETC_TRIGn_COUNTER_INIT_DELAY(config->initialDelay) |
            ADC_ETC_TRIGn_COUNTER_SAMPLE_INTERVAL(config->sampleIntervalDelay);
    base->TRIG[triggerGroup].TRIGn_COUNTER = tmp32;
}

/*!
* brief Set the external XBAR trigger chain configuration.
* For example, if triggerGroup is set to 0U and chainGroup is set to 1U, which means Trigger0 source's chain1 would be
* configurated.
*
* param base ADC_ETC peripheral base address.
* param triggerGroup Trigger group index. Available number is 0~7.
* param chainGroup Trigger chain group index. Available number is 0~7.
* param config Pointer to "adc_etc_trigger_chain_config_t" structure.
*/
void ADC_ETC_SetTriggerChainConfig(ADC_ETC_Type *base,
                                   uint32_t triggerGroup,
                                   uint32_t chainGroup,
                                   const adc_etc_trigger_chain_config_t *config)
{
    //assert(triggerGroup < ADC_ETC_TRIGn_CTRL_COUNT);

    uint32_t tmp;
    uint32_t tmp32;
    uint8_t mRemainder = chainGroup % 2U;

    /*  Set ADC_ETC_TRIGn_CHAINm register. */
    tmp = ADC_ETC_TRIGn_CHAIN_1_0_HWTS0(config->ADCHCRegisterSelect) |
          ADC_ETC_TRIGn_CHAIN_1_0_CSEL0(config->ADCChannelSelect) |
          ADC_ETC_TRIGn_CHAIN_1_0_IE0(config->InterruptEnable);
    if (config->enableB2BMode)
    {
        tmp |= ADC_ETC_TRIGn_CHAIN_1_0_B2B0_MASK;
    }
    switch (chainGroup / 2U)
    {
        case 0U: /* Configurate trigger chain0 and chain 1. */
            tmp32 = base->TRIG[triggerGroup].TRIGn_CHAIN_1_0;
            if (mRemainder == 0U) /* Chain 0. */
            {
                tmp32 &= ~(ADC_ETC_TRIGn_CHAIN_1_0_CSEL0_MASK | ADC_ETC_TRIGn_CHAIN_1_0_HWTS0_MASK |
                           ADC_ETC_TRIGn_CHAIN_1_0_B2B0_MASK | ADC_ETC_TRIGn_CHAIN_1_0_IE0_MASK);
                tmp32 |= tmp;
            }
            else /* Chain 1. */
            {
                tmp32 &= ~(ADC_ETC_TRIGn_CHAIN_1_0_CSEL1_MASK | ADC_ETC_TRIGn_CHAIN_1_0_HWTS1_MASK |
                           ADC_ETC_TRIGn_CHAIN_1_0_B2B1_MASK | ADC_ETC_TRIGn_CHAIN_1_0_IE1_MASK);
                tmp32 |= (tmp << ADC_ETC_TRIGn_CHAIN_1_0_CSEL1_SHIFT);
            }
            base->TRIG[triggerGroup].TRIGn_CHAIN_1_0 = tmp32;
            break;
        case 1U: /* Configurate trigger chain2 and chain 3. */
            tmp32 = base->TRIG[triggerGroup].TRIGn_CHAIN_3_2;
            if (mRemainder == 0U) /* Chain 2. */
            {
                tmp32 &= ~(ADC_ETC_TRIGn_CHAIN_3_2_CSEL2_MASK | ADC_ETC_TRIGn_CHAIN_3_2_HWTS2_MASK |
                           ADC_ETC_TRIGn_CHAIN_3_2_B2B2_MASK | ADC_ETC_TRIGn_CHAIN_3_2_IE2_MASK);
                tmp32 |= tmp;
            }
            else /* Chain 3. */
            {
                tmp32 &= ~(ADC_ETC_TRIGn_CHAIN_3_2_CSEL3_MASK | ADC_ETC_TRIGn_CHAIN_3_2_HWTS3_MASK |
                           ADC_ETC_TRIGn_CHAIN_3_2_B2B3_MASK | ADC_ETC_TRIGn_CHAIN_3_2_IE3_MASK);
                tmp32 |= (tmp << ADC_ETC_TRIGn_CHAIN_3_2_CSEL3_SHIFT);
            }
            base->TRIG[triggerGroup].TRIGn_CHAIN_3_2 = tmp32;
            break;
        case 2U: /* Configurate trigger chain4 and chain 5. */
            tmp32 = base->TRIG[triggerGroup].TRIGn_CHAIN_5_4;
            if (mRemainder == 0U) /* Chain 4. */
            {
                tmp32 &= ~(ADC_ETC_TRIGn_CHAIN_5_4_CSEL4_MASK | ADC_ETC_TRIGn_CHAIN_5_4_HWTS4_MASK |
                           ADC_ETC_TRIGn_CHAIN_5_4_B2B4_MASK | ADC_ETC_TRIGn_CHAIN_5_4_IE4_MASK);
                tmp32 |= tmp;
            }
            else /* Chain 5. */
            {
                tmp32 &= ~(ADC_ETC_TRIGn_CHAIN_5_4_CSEL5_MASK | ADC_ETC_TRIGn_CHAIN_5_4_HWTS5_MASK |
                           ADC_ETC_TRIGn_CHAIN_5_4_B2B5_MASK | ADC_ETC_TRIGn_CHAIN_5_4_IE5_MASK);
                tmp32 |= (tmp << ADC_ETC_TRIGn_CHAIN_5_4_CSEL5_SHIFT);
            }
            base->TRIG[triggerGroup].TRIGn_CHAIN_5_4 = tmp32;
            break;
        case 3U: /* Configurate trigger chain6 and chain 7. */
            tmp32 = base->TRIG[triggerGroup].TRIGn_CHAIN_7_6;
            if (mRemainder == 0U) /* Chain 6. */
            {
                tmp32 &= ~(ADC_ETC_TRIGn_CHAIN_7_6_CSEL6_MASK | ADC_ETC_TRIGn_CHAIN_7_6_HWTS6_MASK |
                           ADC_ETC_TRIGn_CHAIN_7_6_B2B6_MASK | ADC_ETC_TRIGn_CHAIN_7_6_IE6_MASK);
                tmp32 |= tmp;
            }
            else /* Chain 7. */
            {
                tmp32 &= ~(ADC_ETC_TRIGn_CHAIN_7_6_CSEL7_MASK | ADC_ETC_TRIGn_CHAIN_7_6_HWTS7_MASK |
                           ADC_ETC_TRIGn_CHAIN_7_6_B2B7_MASK | ADC_ETC_TRIGn_CHAIN_7_6_IE7_MASK);
                tmp32 |= (tmp << ADC_ETC_TRIGn_CHAIN_7_6_CSEL7_SHIFT);
            }
            base->TRIG[triggerGroup].TRIGn_CHAIN_7_6 = tmp32;
            break;
        default:
            break;
    }
}

/*!
* brief Gets the interrupt status flags of external XBAR and TSC triggers.
*
* param base ADC_ETC peripheral base address.
* param sourceIndex trigger source index.
*
* return Status flags mask of trigger. Refer to "_adc_etc_status_flag_mask".
*/
uint32_t ADC_ETC_GetInterruptStatusFlags(ADC_ETC_Type *base, adc_etc_external_trigger_source_t sourceIndex)
{
    uint32_t tmp32 = 0U;

    if (((base->DONE0_1_IRQ) & (ADC_ETC_DONE0_1_IRQ_TRIG0_DONE0_MASK << sourceIndex)) != 0U)
    {
        tmp32 |= kADC_ETC_Done0StatusFlagMask; /* Customized DONE0 status flags mask, which is defined in fsl_adc_etc.h
                                                  file. */
    }
    if (((base->DONE0_1_IRQ) & (ADC_ETC_DONE0_1_IRQ_TRIG0_DONE1_MASK << sourceIndex)) != 0U)
    {
        tmp32 |= kADC_ETC_Done1StatusFlagMask; /* Customized DONE1 status flags mask, which is defined in fsl_adc_etc.h
                                                  file. */
    }
    if (((base->DONE2_ERR_IRQ) & (ADC_ETC_DONE2_ERR_IRQ_TRIG0_DONE2_MASK << sourceIndex)) != 0U)
    {
        tmp32 |= kADC_ETC_Done2StatusFlagMask; /* Customized DONE2 status flags mask, which is defined in fsl_adc_etc.h
                                                  file. */
    }
    if (((base->DONE2_ERR_IRQ) & (ADC_ETC_DONE2_ERR_IRQ_TRIG0_ERR_MASK << sourceIndex)) != 0U)
    {
        tmp32 |= kADC_ETC_ErrorStatusFlagMask; /* Customized ERROR status flags mask, which is defined in fsl_adc_etc.h
                                                  file. */
    }
    return tmp32;
}

/*!
* brief Clears the ADC_ETC's interrupt status falgs.
*
* param base ADC_ETC peripheral base address.
* param sourceIndex trigger source index.
* param mask Status flags mask of trigger. Refer to "_adc_etc_status_flag_mask".
*/
void ADC_ETC_ClearInterruptStatusFlags(ADC_ETC_Type *base, adc_etc_external_trigger_source_t sourceIndex, uint32_t mask)
{
    if (0U != (mask & kADC_ETC_Done0StatusFlagMask)) /* Write 1 to clear DONE0 status flags. */
    {
        base->DONE0_1_IRQ = (ADC_ETC_DONE0_1_IRQ_TRIG0_DONE0_MASK << sourceIndex);
    }
    if (0U != (mask & kADC_ETC_Done1StatusFlagMask)) /* Write 1 to clear DONE1 status flags. */
    {
        base->DONE0_1_IRQ = (ADC_ETC_DONE0_1_IRQ_TRIG0_DONE1_MASK << sourceIndex);
    }
    if (0U != (mask & kADC_ETC_Done2StatusFlagMask)) /* Write 1 to clear DONE2 status flags. */
    {
        base->DONE2_ERR_IRQ = (ADC_ETC_DONE2_ERR_IRQ_TRIG0_DONE2_MASK << sourceIndex);
    }
    if (0U != (mask & kADC_ETC_ErrorStatusFlagMask)) /* Write 1 to clear ERROR status flags. */
    {
        base->DONE2_ERR_IRQ = (ADC_ETC_DONE2_ERR_IRQ_TRIG0_ERR_MASK << sourceIndex);
    }
}

/*!
* brief Get ADC conversion result from external XBAR sources.
* For example, if triggerGroup is set to 0U and chainGroup is set to 1U, which means the API would
* return Trigger0 source's chain1 conversion result.
*
* param base ADC_ETC peripheral base address.
* param triggerGroup Trigger group index. Available number is 0~7.
* param chainGroup Trigger chain group index. Available number is 0~7.
* return ADC conversion result value.
*/
uint32_t ADC_ETC_GetADCConversionValue(ADC_ETC_Type *base, uint32_t triggerGroup, uint32_t chainGroup)
{
    //assert(triggerGroup < ADC_ETC_TRIGn_RESULT_1_0_COUNT);

    uint32_t mADCResult;
    uint8_t mRemainder = chainGroup % 2U;

    switch (chainGroup / 2U)
    {
        case 0U:
            if (0U == mRemainder)
            {
                mADCResult = ADC_ETC_TRIGn_RESULT_1_0_DATA0_MASK & (base->TRIG[triggerGroup].TRIGn_RESULT_1_0);
            }
            else
            {
                mADCResult = (base->TRIG[triggerGroup].TRIGn_RESULT_1_0) >> ADC_ETC_TRIGn_RESULT_1_0_DATA1_SHIFT;
            }
            break;
        case 1U:
            if (0U == mRemainder)
            {
                mADCResult = ADC_ETC_TRIGn_RESULT_3_2_DATA2_MASK & (base->TRIG[triggerGroup].TRIGn_RESULT_3_2);
            }
            else
            {
                mADCResult = (base->TRIG[triggerGroup].TRIGn_RESULT_3_2) >> ADC_ETC_TRIGn_RESULT_3_2_DATA3_SHIFT;
            }
            break;
        case 2U:
            if (0U == mRemainder)
            {
                mADCResult = ADC_ETC_TRIGn_RESULT_5_4_DATA4_MASK & (base->TRIG[triggerGroup].TRIGn_RESULT_5_4);
            }
            else
            {
                mADCResult = (base->TRIG[triggerGroup].TRIGn_RESULT_5_4) >> ADC_ETC_TRIGn_RESULT_5_4_DATA5_SHIFT;
            }
            break;
        case 3U:
            if (0U == mRemainder)
            {
                mADCResult = ADC_ETC_TRIGn_RESULT_7_6_DATA6_MASK & (base->TRIG[triggerGroup].TRIGn_RESULT_7_6);
            }
            else
            {
                mADCResult = (base->TRIG[triggerGroup].TRIGn_RESULT_7_6) >> ADC_ETC_TRIGn_RESULT_7_6_DATA7_SHIFT;
            }
            break;
        default:
            return 0U;
    }
    return mADCResult;
}
