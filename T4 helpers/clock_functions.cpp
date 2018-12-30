/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Extracted functions and defines for new function based on NXP code
 */
#include "clock_functions.h"


volatile uint32_t g_xtalFreq;

static uint32_t CLOCK_GetPeriphClkFreq(void)
{
    uint32_t freq;
    //Serial.print("IF: ");
    //Serial.println(CCM_CBCDR & CCM_CBCDR_PERIPH_CLK_SEL_MASK, HEX);
    //Serial.print("else: ");
    //Serial.println(CCM_CBCDR & CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK, HEX);
    //Serial.print("CCM_CBCMR_PRE_PERIPH_CLK_SEL(0U): ");
    //Serial.println(CCM_CBCMR_PRE_PERIPH_CLK_SEL(0U), HEX);
    //Serial.print("CCM_CBCMR_PRE_PERIPH_CLK_SEL(1U): ");
    //Serial.println(CCM_CBCMR_PRE_PERIPH_CLK_SEL(1U), HEX);
    //Serial.print("CCM_CBCMR_PRE_PERIPH_CLK_SEL(2U): ");
    //Serial.println(CCM_CBCMR_PRE_PERIPH_CLK_SEL(2U), HEX);
    //Serial.print("CCM_CBCMR_PRE_PERIPH_CLK_SEL(3U): ");
    //Serial.println(CCM_CBCMR_PRE_PERIPH_CLK_SEL(3U), HEX);
    /* Periph_clk2_clk ---> Periph_clk */
    if (CCM_CBCDR & CCM_CBCDR_PERIPH_CLK_SEL_MASK)
    {
        switch (CCM_CBCMR & CCM_CBCMR_PERIPH_CLK2_SEL_MASK)
        {
            /* Pll3_sw_clk ---> Periph_clk2_clk ---> Periph_clk */
            case CCM_CBCMR_PERIPH_CLK2_SEL(0U):
                freq = CLOCK_GetPllFreq(kCLOCK_PllUsb1);
                break;

            /* Osc_clk ---> Periph_clk2_clk ---> Periph_clk */
            case CCM_CBCMR_PERIPH_CLK2_SEL(1U):
                freq = CLOCK_GetOscFreq();
                break;

            case CCM_CBCMR_PERIPH_CLK2_SEL(2U):
                freq = CLOCK_GetPllFreq(kCLOCK_PllSys);
                break;

            case CCM_CBCMR_PERIPH_CLK2_SEL(3U):
            default:
                freq = 0U;
                break;
        }

        freq /= (((CCM_CBCDR & CCM_CBCDR_PERIPH_CLK2_PODF_MASK) >> CCM_CBCDR_PERIPH_CLK2_PODF_SHIFT) + 1U);
    }
    /* Pre_Periph_clk ---> Periph_clk */
    else
    {
        switch (CCM_CBCMR & CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK)
        {
            /* PLL2 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(0U):
                //Serial.println("CASE0");
                break;

            /* PLL2 PFD2 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(1U):
                //Serial.println("CASE1");
                freq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd2);
                break;

            /* PLL2 PFD0 ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(2U):
                //Serial.println("CASE2");
                freq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd0);
                break;

            /* PLL1 divided(/2) ---> Pre_Periph_clk ---> Periph_clk */
            case CCM_CBCMR_PRE_PERIPH_CLK_SEL(3U):
                //Serial.println("CASE3");
                //Serial.printf("kCLOCK_PllSys: %d\r\n", kCLOCK_PllArm);
                freq = CLOCK_GetPllFreq(kCLOCK_PllArm) /
                       (((CCM_CACRR & CCM_CACRR_ARM_PODF_MASK) >> CCM_CACRR_ARM_PODF_SHIFT) + 1U);
                break;

            default:
                freq = 0U;
                break;
        }
    }

    return freq;
}

/*!
 * brief Gets the AHB clock frequency.
 *
 * return  The AHB clock frequency value in hertz.
 */
uint32_t CLOCK_GetAhbFreq(void)
{
    //Serial.printf("Denom for AHB Clock: %d\r\n", (((CCM_CBCDR & CCM_CBCDR_AHB_PODF_MASK) >> CCM_CBCDR_AHB_PODF_SHIFT) + 1U));
    return CLOCK_GetPeriphClkFreq() / (((CCM_CBCDR & CCM_CBCDR_AHB_PODF_MASK) >> CCM_CBCDR_AHB_PODF_SHIFT) + 1U);
}

/*!
 * brief Gets the SEMC clock frequency.
 *
 * return  The SEMC clock frequency value in hertz.
 */
uint32_t CLOCK_GetSemcFreq(void)
{
    uint32_t freq;

    /* SEMC alternative clock ---> SEMC Clock */
    if (CCM_CBCDR & CCM_CBCDR_SEMC_CLK_SEL_MASK)
    {
        /* PLL3 PFD1 ---> SEMC alternative clock ---> SEMC Clock */
        if (CCM_CBCDR & CCM_CBCDR_SEMC_ALT_CLK_SEL_MASK)
        {
            freq = CLOCK_GetUsb1PfdFreq(kCLOCK_Pfd1);
        }
        /* PLL2 PFD2 ---> SEMC alternative clock ---> SEMC Clock */
        else
        {
            freq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd2);
        }
    }
    /* Periph_clk ---> SEMC Clock */
    else
    {
        freq = CLOCK_GetPeriphClkFreq();
    }

    freq /= (((CCM_CBCDR & CCM_CBCDR_SEMC_PODF_MASK) >> CCM_CBCDR_SEMC_PODF_SHIFT) + 1U);

    return freq;
}

/*!
 * brief Gets the IPG clock frequency.
 *
 * return  The IPG clock frequency value in hertz.
 */
uint32_t CLOCK_GetIpgFreq(void)
{
    return CLOCK_GetAhbFreq() / (((CCM_CBCDR & CCM_CBCDR_IPG_PODF_MASK) >> CCM_CBCDR_IPG_PODF_SHIFT) + 1U);
}

/*!
 * brief Gets the PER clock frequency.
 *
 * return  The PER clock frequency value in hertz.
 */
uint32_t CLOCK_GetPerClkFreq(void)
{
    uint32_t freq;

    /* Osc_clk ---> PER Clock*/
    if (CCM_CSCMR1 & CCM_CSCMR1_PERCLK_CLK_SEL_MASK)
    {
        freq = CLOCK_GetOscFreq();
    }
    /* Periph_clk ---> AHB Clock ---> IPG Clock ---> PER Clock */
    else
    {
        freq = CLOCK_GetFreq(kCLOCK_IpgClk);
    }

    freq /= (((CCM_CSCMR1 & CCM_CSCMR1_PERCLK_PODF_MASK) >> CCM_CSCMR1_PERCLK_PODF_SHIFT) + 1U);

    return freq;
}

/*!
 * brief Gets the clock frequency for a specific clock name.
 *
 * This function checks the current clock configurations and then calculates
 * the clock frequency for a specific clock name defined in clock_name_t.
 *
 * param clockName Clock names defined in clock_name_t
 * return Clock frequency value in hertz
 */
uint32_t CLOCK_GetFreq(clock_name_t name)
{
    uint32_t freq;

    switch (name)
    {
        case kCLOCK_CpuClk:
        case kCLOCK_AhbClk:
            freq = CLOCK_GetAhbFreq();
            break;

        case kCLOCK_SemcClk:
            freq = CLOCK_GetSemcFreq();
            break;

        case kCLOCK_IpgClk:
            freq = CLOCK_GetIpgFreq();
            break;

        case kCLOCK_PerClk:
            freq = CLOCK_GetPerClkFreq();
            break;

        case kCLOCK_OscClk:
            freq = CLOCK_GetOscFreq();
            break;
        case kCLOCK_RtcClk:
            freq = CLOCK_GetRtcFreq();
            break;
        case kCLOCK_ArmPllClk:
            freq = CLOCK_GetPllFreq(kCLOCK_PllArm);
            break;
        case kCLOCK_Usb1PllClk:
            freq = CLOCK_GetPllFreq(kCLOCK_PllUsb1);
            break;
        case kCLOCK_Usb1PllPfd0Clk:
            freq = CLOCK_GetUsb1PfdFreq(kCLOCK_Pfd0);
            break;
        case kCLOCK_Usb1PllPfd1Clk:
            freq = CLOCK_GetUsb1PfdFreq(kCLOCK_Pfd1);
            break;
        case kCLOCK_Usb1PllPfd2Clk:
            freq = CLOCK_GetUsb1PfdFreq(kCLOCK_Pfd2);
            break;
        case kCLOCK_Usb1PllPfd3Clk:
            freq = CLOCK_GetUsb1PfdFreq(kCLOCK_Pfd3);
            break;
        case kCLOCK_Usb2PllClk:
            freq = CLOCK_GetPllFreq(kCLOCK_PllUsb2);
            break;
        case kCLOCK_SysPllClk:
            freq = CLOCK_GetPllFreq(kCLOCK_PllSys);
            break;
        case kCLOCK_SysPllPfd0Clk:
            freq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd0);
            break;
        case kCLOCK_SysPllPfd1Clk:
            freq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd1);
            break;
        case kCLOCK_SysPllPfd2Clk:
            freq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd2);
            break;
        case kCLOCK_SysPllPfd3Clk:
            freq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd3);
            break;
        case kCLOCK_EnetPll0Clk:
            freq = CLOCK_GetPllFreq(kCLOCK_PllEnet);
            break;
        case kCLOCK_EnetPll1Clk:
            freq = CLOCK_GetPllFreq(kCLOCK_PllEnet25M);
            break;
        case kCLOCK_AudioPllClk:
            freq = CLOCK_GetPllFreq(kCLOCK_PllAudio);
            break;
        case kCLOCK_VideoPllClk:
            freq = CLOCK_GetPllFreq(kCLOCK_PllVideo);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*!
 * brief Get current PLL output frequency.
 *
 * This function get current output frequency of specific PLL
 *
 * param pll   pll name to get frequency.
 * return The PLL output frequency in hertz.
 */
uint32_t CLOCK_GetPllFreq(clock_pll_t pll)
{
    uint32_t freq;
    uint32_t divSelect;
    clock_64b_t freqTmp;

    const uint32_t enetRefClkFreq[] = {
        25000000U,  /* 25M */
        50000000U,  /* 50M */
        100000000U, /* 100M */
        125000000U  /* 125M */
    };

    /* check if PLL is enabled */
    if (!CLOCK_IsPllEnabled(CCM_ANALOG, pll))
    {
        //Serial.printf("Clock not enabled!\r\n");
        return 0U;
    }

    /* get pll reference clock */
    freq = CLOCK_GetPllBypassRefClk(CCM_ANALOG, pll);
    //Serial.printf("pll reference clock: %d\r\n", freq);

    /* check if pll is bypassed */
    if (CLOCK_IsPllBypassed(CCM_ANALOG, pll))
    {
        return freq;
    }
    ////Serial.printf("Switching to get clock %d \r\n", pll);
    switch (pll)
    {
        case kCLOCK_PllArm:
            //Serial.println("got to kCLOCK_PLLARM");
            freq = ((freq * ((CCM_ANALOG_PLL_ARM & CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK) >>
                             CCM_ANALOG_PLL_ARM_DIV_SELECT_SHIFT)) >>
                    1U);
            break;
        case kCLOCK_PllSys:
            //Serial.println("got to kCLOCK_PllSys");
            /* PLL output frequency = Fref * (DIV_SELECT + NUM/DENOM). */
            freqTmp = ((clock_64b_t)freq * ((clock_64b_t)(CCM_ANALOG_PLL_SYS_NUM))) /
                      ((clock_64b_t)(CCM_ANALOG_PLL_SYS_DENOM));

            if (CCM_ANALOG_PLL_SYS & CCM_ANALOG_PLL_SYS_DIV_SELECT_MASK)
            {
                freq *= 22U;
            }
            else
            {
                freq *= 20U;
            }

            freq += (uint32_t)freqTmp;
            break;

        case kCLOCK_PllUsb1:
        //Serial.println("got to kCLOCK_PllUsb1");
            freq = (freq * ((CCM_ANALOG_PLL_USB1 & CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK) ? 22U : 20U));
            break;

        case kCLOCK_PllAudio:
            //Serial.println("got to kCLOCK_PllAudio");
            /* PLL output frequency = Fref * (DIV_SELECT + NUM/DENOM). */
            divSelect =
                (CCM_ANALOG_PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_DIV_SELECT_MASK) >> CCM_ANALOG_PLL_AUDIO_DIV_SELECT_SHIFT;

            freqTmp = ((clock_64b_t)freq * ((clock_64b_t)(CCM_ANALOG_PLL_AUDIO_NUM))) /
                      ((clock_64b_t)(CCM_ANALOG_PLL_AUDIO_DENOM));

            freq = freq * divSelect + (uint32_t)freqTmp;

            /* AUDIO PLL output = PLL output frequency / POSTDIV. */

            /*
             * Post divider:
             *
             * PLL_AUDIO[POST_DIV_SELECT]:
             * 0x00: 4
             * 0x01: 2
             * 0x02: 1
             *
             * MISC2[AUDO_DIV]:
             * 0x00: 1
             * 0x01: 2
             * 0x02: 1
             * 0x03: 4
             */
            switch (CCM_ANALOG_PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT_MASK)
            {
                case CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(0U):     //Not there
                    freq = freq >> 2U;
                    break;

                case CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(1U):
                    freq = freq >> 1U;
                    break;

                default:
                    break;
            }

            switch (CCM_ANALOG_MISC2 & (CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK | CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK))
            {
                case CCM_ANALOG_MISC2_AUDIO_DIV_MSB(1) | CCM_ANALOG_MISC2_AUDIO_DIV_LSB(1):  //not there
                    freq >>= 2U;
                    break;

                case CCM_ANALOG_MISC2_AUDIO_DIV_MSB(0) | CCM_ANALOG_MISC2_AUDIO_DIV_LSB(1):  //not there
                    freq >>= 1U;
                    break;

                default:
                    break;
            }
            break;

        case kCLOCK_PllVideo:
        //Serial.println("got to kCLOCK_PllVideo");
            /* PLL output frequency = Fref * (DIV_SELECT + NUM/DENOM). */
            divSelect =
                (CCM_ANALOG_PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_DIV_SELECT_MASK) >> CCM_ANALOG_PLL_VIDEO_DIV_SELECT_SHIFT;

            freqTmp = ((clock_64b_t)freq * ((clock_64b_t)(CCM_ANALOG_PLL_VIDEO_NUM))) /
                      ((clock_64b_t)(CCM_ANALOG_PLL_VIDEO_DENOM));

            freq = freq * divSelect + (uint32_t)freqTmp;

            /* VIDEO PLL output = PLL output frequency / POSTDIV. */

            /*
             * Post divider:
             *
             * PLL_VIDEO[POST_DIV_SELECT]:
             * 0x00: 4
             * 0x01: 2
             * 0x02: 1
             *
             * MISC2[VIDEO_DIV]:
             * 0x00: 1
             * 0x01: 2
             * 0x02: 1
             * 0x03: 4
             */
            switch (CCM_ANALOG_PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT_MASK)
            {
                case CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(0U):  //not there
                    freq = freq >> 2U;
                    break;

                case CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(1U):
                    freq = freq >> 1U;
                    break;

                default:
                    break;
            }

            switch (CCM_ANALOG_MISC2 & CCM_ANALOG_MISC2_VIDEO_DIV_MASK)
            {
                case CCM_ANALOG_MISC2_VIDEO_DIV(3):  //not there
                    freq >>= 2U;
                    break;

                case CCM_ANALOG_MISC2_VIDEO_DIV(1):
                    freq >>= 1U;
                    break;

                default:
                    break;
            }
            break;
        case kCLOCK_PllEnet:
        
            divSelect =
                (CCM_ANALOG_PLL_ENET & CCM_ANALOG_PLL_ENET_DIV_SELECT_MASK) >> CCM_ANALOG_PLL_ENET_DIV_SELECT_SHIFT;
            freq = enetRefClkFreq[divSelect];
            break;

        case kCLOCK_PllEnet25M:
        //Serial.println("got to kCLOCK_PllEnet25M");
            /* ref_enetpll1 if fixed at 25MHz. */
            freq = 25000000UL;
            break;

        case kCLOCK_PllUsb2:
        //Serial.println("got to kCLOCK_PllUsb2");
            freq = (freq * ((CCM_ANALOG_PLL_USB2 & CCM_ANALOG_PLL_USB2_DIV_SELECT_MASK) ? 22U : 20U));
            break;
        default:
            //Serial.println("DEFAULT HIT ON GETTING FREQ");
            freq = 0U;
            break;
    }

    return freq;
}


/*!
 * brief Get current System PLL PFD output frequency.
 *
 * This function get current output frequency of specific System PLL PFD
 *
 * param pfd   pfd name to get frequency.
 * return The PFD output frequency in hertz.
 */
uint32_t CLOCK_GetSysPfdFreq(clock_pfd_t pfd)
{
    uint32_t freq = CLOCK_GetPllFreq(kCLOCK_PllSys);

    switch (pfd)
    {
        case kCLOCK_Pfd0:
            freq /= ((CCM_ANALOG_PFD_528 & CCM_ANALOG_PFD_528_PFD0_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD0_FRAC_SHIFT);
            break;

        case kCLOCK_Pfd1:
            freq /= ((CCM_ANALOG_PFD_528 & CCM_ANALOG_PFD_528_PFD1_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD1_FRAC_SHIFT);
            break;

        case kCLOCK_Pfd2:
            freq /= ((CCM_ANALOG_PFD_528 & CCM_ANALOG_PFD_528_PFD2_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD2_FRAC_SHIFT);
            break;

        case kCLOCK_Pfd3:
            freq /= ((CCM_ANALOG_PFD_528 & CCM_ANALOG_PFD_528_PFD3_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD3_FRAC_SHIFT);
            break;

        default:
            freq = 0U;
            break;
    }
    freq *= 18U;

    return freq;
}

/*!
 * brief Get current USB1 PLL PFD output frequency.
 *
 * This function get current output frequency of specific USB1 PLL PFD
 *
 * param pfd   pfd name to get frequency.
 * return The PFD output frequency in hertz.
 */
uint32_t CLOCK_GetUsb1PfdFreq(clock_pfd_t pfd)
{
    uint32_t freq = CLOCK_GetPllFreq(kCLOCK_PllUsb1);

    switch (pfd)
    {
        case kCLOCK_Pfd0:
            freq /= ((CCM_ANALOG_PFD_480 & CCM_ANALOG_PFD_480_PFD0_FRAC_MASK) >> CCM_ANALOG_PFD_480_PFD0_FRAC_SHIFT);
            break;

        case kCLOCK_Pfd1:
            freq /= ((CCM_ANALOG_PFD_480 & CCM_ANALOG_PFD_480_PFD1_FRAC_MASK) >> CCM_ANALOG_PFD_480_PFD1_FRAC_SHIFT);
            break;

        case kCLOCK_Pfd2:
            freq /= ((CCM_ANALOG_PFD_480 & CCM_ANALOG_PFD_480_PFD2_FRAC_MASK) >> CCM_ANALOG_PFD_480_PFD2_FRAC_SHIFT);
            break;

        case kCLOCK_Pfd3:
            freq /= ((CCM_ANALOG_PFD_480 & CCM_ANALOG_PFD_480_PFD3_FRAC_MASK) >> CCM_ANALOG_PFD_480_PFD3_FRAC_SHIFT);
            break;

        default:
            freq = 0U;
            break;
    }
    freq *= 18U;

    return freq;
}
