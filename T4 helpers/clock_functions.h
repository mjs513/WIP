#include "Arduino.h"

/* External XTAL (OSC) clock frequency. */
extern volatile uint32_t g_xtalFreq;

/* External RTC XTAL clock frequency. */
//volatile uint32_t g_rtcXtalFreq;

#define CCM_CBCDR_PERIPH_CLK_SEL_MASK            (0x2000000U)
#define CCM_CBCDR_PERIPH_CLK_SEL_SHIFT           (25U)
#define CCM_CBCDR_PERIPH_CLK2_PODF_SHIFT         (27U)
//#define CCM_CBCMR_PERIPH_CLK2_SEL_MASK           (0x3000U)  //duplicated in imrxt.h
#define CCM_CBCMR_PERIPH_CLK2_SEL_SHIFT          (12U)
//#define CCM_CBCDR_PERIPH_CLK2_PODF_MASK          (0x38000000U)

//#define CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK      (0xC0000U)
#define CCM_CBCMR_PRE_PERIPH_CLK_SEL_SHIFT       (18U)

//#define CCM_CACRR_ARM_PODF_MASK                  (0x7U)
#define CCM_CACRR_ARM_PODF_SHIFT                 (0U)

//#define CCM_CBCDR_AHB_PODF_MASK                  (0x1C00U)
#define CCM_CBCDR_AHB_PODF_SHIFT                 (10U)

#define CCM_CBCDR_SEMC_PODF_MASK                 (0x70000U)
#define CCM_CBCDR_SEMC_PODF_SHIFT                (16U)
#define CCM_CBCDR_SEMC_CLK_SEL_MASK              (0x40U)
#define CCM_CBCDR_SEMC_CLK_SEL_SHIFT             (6U)
#define CCM_CBCDR_SEMC_ALT_CLK_SEL_MASK          (0x80U)
#define CCM_CBCDR_SEMC_ALT_CLK_SEL_SHIFT         (7U)

//#define CCM_CBCDR_IPG_PODF_MASK                  (0x300U)
#define CCM_CBCDR_IPG_PODF_SHIFT                 (8U)

#define CCM_CSCMR1_PERCLK_CLK_SEL_MASK           (0x40U)
#define CCM_CSCMR1_PERCLK_CLK_SEL_SHIFT          (6U)

#define CCM_CSCMR1_PERCLK_PODF_MASK              (0x3FU)
#define CCM_CSCMR1_PERCLK_PODF_SHIFT             (0U)

#define CCM_ANALOG_PFD_528_PFD0_FRAC_MASK        (0x3FU)
#define CCM_ANALOG_PFD_528_PFD0_FRAC_SHIFT       (0U)
#define CCM_ANALOG_PFD_528_PFD1_FRAC_MASK        (0x3F00U)
#define CCM_ANALOG_PFD_528_PFD1_FRAC_SHIFT       (8U)
#define CCM_ANALOG_PFD_528_PFD2_FRAC_MASK        (0x3F0000U)
#define CCM_ANALOG_PFD_528_PFD2_FRAC_SHIFT       (16U)
#define CCM_ANALOG_PFD_528_PFD3_FRAC_MASK        (0x3F000000U)
#define CCM_ANALOG_PFD_528_PFD3_FRAC_SHIFT       (24U)

#define CCM_ANALOG_PFD_480_PFD0_FRAC_MASK        (0x3FU)
#define CCM_ANALOG_PFD_480_PFD0_FRAC_SHIFT       (0U)
#define CCM_ANALOG_PFD_480_PFD1_FRAC_MASK        (0x3F00U)
#define CCM_ANALOG_PFD_480_PFD1_FRAC_SHIFT       (8U)
#define CCM_ANALOG_PFD_480_PFD2_FRAC_MASK        (0x3F0000U)
#define CCM_ANALOG_PFD_480_PFD2_FRAC_SHIFT       (16U)
#define CCM_ANALOG_PFD_480_PFD3_FRAC_MASK        (0x3F000000U)
#define CCM_ANALOG_PFD_480_PFD3_FRAC_SHIFT       (24U)

#define CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(x)  (((uint32_t)(((uint32_t)(x)) << CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT_SHIFT)) & CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT_MASK)

#define CCM_ANALOG_MISC2_AUDIO_DIV_MSB(x)        (((uint32_t)(((uint32_t)(x)) << CCM_ANALOG_MISC2_AUDIO_DIV_MSB_SHIFT)) & CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK)
#define CCM_ANALOG_MISC2_AUDIO_DIV_LSB(x)        (((uint32_t)(((uint32_t)(x)) << CCM_ANALOG_MISC2_AUDIO_DIV_LSB_SHIFT)) & CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK)

#define CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(x)  (((uint32_t)(((uint32_t)(x)) << CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT_SHIFT)) & CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT_MASK)
#define CCM_ANALOG_MISC2_VIDEO_DIV(x)            (((uint32_t)(((uint32_t)(x)) << CCM_ANALOG_MISC2_VIDEO_DIV_SHIFT)) & CCM_ANALOG_MISC2_VIDEO_DIV_MASK)

#define CCM_ANALOG_PLL_ARM_ENABLE_SHIFT          (13U)
#define CCM_ANALOG_PLL_SYS_ENABLE_SHIFT          (13U)
#define CCM_ANALOG_PLL_USB1_ENABLE_SHIFT         (13U)
#define CCM_ANALOG_PLL_AUDIO_ENABLE_SHIFT        (13U)
#define CCM_ANALOG_PLL_VIDEO_ENABLE_SHIFT        (13U)
#define CCM_ANALOG_PLL_ENET_ENABLE_SHIFT         (13U)
#define CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN_SHIFT (21U)
#define CCM_ANALOG_PLL_USB2_ENABLE_SHIFT         (13U)

#define CCM_ANALOG_PLL_SYS_DIV_SELECT_MASK       (0x1U)
#define CCM_ANALOG_PLL_ARM_DIV_SELECT_SHIFT      (0U)
#define CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK      (0x2U)
#define CCM_ANALOG_PLL_USB2_DIV_SELECT_MASK      (0x2U)
#define CCM_ANALOG_PLL_USB2_DIV_SELECT_SHIFT     (1U)
#define CCM_ANALOG_PLL_USB1_DIV_SELECT_SHIFT     (1U)
#define CCM_ANALOG_PLL_AUDIO_DIV_SELECT_MASK     (0x7FU)
#define CCM_ANALOG_PLL_AUDIO_DIV_SELECT_SHIFT    (0U)
#define CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT_MASK (0x180000U)
#define CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT_SHIFT (19U)
#define CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK      (0x800000U)
#define CCM_ANALOG_MISC2_AUDIO_DIV_MSB_SHIFT     (23U)
#define CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK      (0x8000U)
#define CCM_ANALOG_MISC2_AUDIO_DIV_LSB_SHIFT     (15U)
#define CCM_ANALOG_PLL_VIDEO_DIV_SELECT_MASK     (0x7FU)
#define CCM_ANALOG_PLL_VIDEO_DIV_SELECT_SHIFT    (0U)
#define CCM_ANALOG_PLL_VIDEO_DIV_SELECT(x)       (((uint32_t)(((uint32_t)(x)) << CCM_ANALOG_PLL_VIDEO_DIV_SELECT_SHIFT)) & CCM_ANALOG_PLL_VIDEO_DIV_SELECT_MASK)
#define CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT_MASK (0x180000U)
#define CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT_SHIFT (19U)
#define CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(x)  (((uint32_t)(((uint32_t)(x)) << CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT_SHIFT)) & CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT_MASK)
#define CCM_ANALOG_MISC2_VIDEO_DIV_MASK          (0xC0000000U)
#define CCM_ANALOG_MISC2_VIDEO_DIV_SHIFT         (30U)
#define CCM_ANALOG_MISC2_VIDEO_DIV(x)            (((uint32_t)(((uint32_t)(x)) << CCM_ANALOG_MISC2_VIDEO_DIV_SHIFT)) & CCM_ANALOG_MISC2_VIDEO_DIV_MASK)
#define CCM_ANALOG_PLL_ENET_DIV_SELECT_MASK      (0x3U)
#define CCM_ANALOG_PLL_ENET_DIV_SELECT_SHIFT     (0U)
#define CCM_ANALOG_PLL_ENET_DIV_SELECT(x)        (((uint32_t)(((uint32_t)(x)) << CCM_ANALOG_PLL_ENET_DIV_SELECT_SHIFT)) & CCM_ANALOG_PLL_ENET_DIV_SELECT_MASK)
#define CCM_ANALOG_PLL_BYPASS_SHIFT (16U)
#define CCM_ANALOG_PLL_BYPASS_CLK_SRC_MASK (0xC000U)
#define CCM_ANALOG_PLL_BYPASS_CLK_SRC_SHIFT (14U)

typedef uint64_t clock_64b_t;

#define CLKPN_FREQ 0U

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{
    kCLOCK_CpuClk = 0x0U,  /*!< CPU clock */
    kCLOCK_AhbClk = 0x1U,  /*!< AHB clock */
    kCLOCK_SemcClk = 0x2U, /*!< SEMC clock */
    kCLOCK_IpgClk = 0x3U,  /*!< IPG clock */
    kCLOCK_PerClk = 0x4U,  /*!< PER clock */

    kCLOCK_OscClk = 0x5U, /*!< OSC clock selected by PMU_LOWPWR_CTRL[OSC_SEL]. */
    kCLOCK_RtcClk = 0x6U, /*!< RTC clock. (RTCCLK) */

    kCLOCK_ArmPllClk = 0x7U, /*!< ARMPLLCLK. */

    kCLOCK_Usb1PllClk = 0x8U,     /*!< USB1PLLCLK. */
    kCLOCK_Usb1PllPfd0Clk = 0x9U, /*!< USB1PLLPDF0CLK. */
    kCLOCK_Usb1PllPfd1Clk = 0xAU, /*!< USB1PLLPFD1CLK. */
    kCLOCK_Usb1PllPfd2Clk = 0xBU, /*!< USB1PLLPFD2CLK. */
    kCLOCK_Usb1PllPfd3Clk = 0xCU, /*!< USB1PLLPFD3CLK. */

    kCLOCK_Usb2PllClk = 0xDU, /*!< USB2PLLCLK. */

    kCLOCK_SysPllClk = 0xEU,      /*!< SYSPLLCLK. */
    kCLOCK_SysPllPfd0Clk = 0xFU,  /*!< SYSPLLPDF0CLK. */
    kCLOCK_SysPllPfd1Clk = 0x10U, /*!< SYSPLLPFD1CLK. */
    kCLOCK_SysPllPfd2Clk = 0x11U, /*!< SYSPLLPFD2CLK. */
    kCLOCK_SysPllPfd3Clk = 0x12U, /*!< SYSPLLPFD3CLK. */

    kCLOCK_EnetPll0Clk = 0x13U, /*!< Enet PLLCLK ref_enetpll0. */
    kCLOCK_EnetPll1Clk = 0x14U, /*!< Enet PLLCLK ref_enetpll1. */

    kCLOCK_AudioPllClk = 0x15U, /*!< Audio PLLCLK. */
    kCLOCK_VideoPllClk = 0x16U, /*!< Video PLLCLK. */
} clock_name_t;

/*! @brief PLL name */
/*!
 * @brief CCM ANALOG tuple macros to map corresponding registers and bit fields.
 */

typedef struct {
  volatile uint32_t PLL_ARM;                           /**< Analog ARM PLL control Register, offset: 0x0 */
  volatile uint32_t PLL_ARM_SET;                       /**< Analog ARM PLL control Register, offset: 0x4 */
  volatile uint32_t PLL_ARM_CLR;                       /**< Analog ARM PLL control Register, offset: 0x8 */
  volatile uint32_t PLL_ARM_TOG;                       /**< Analog ARM PLL control Register, offset: 0xC */
  volatile uint32_t PLL_USB1;                          /**< Analog USB1 480MHz PLL Control Register, offset: 0x10 */
  volatile uint32_t PLL_USB1_SET;                      /**< Analog USB1 480MHz PLL Control Register, offset: 0x14 */
  volatile uint32_t PLL_USB1_CLR;                      /**< Analog USB1 480MHz PLL Control Register, offset: 0x18 */
  volatile uint32_t PLL_USB1_TOG;                      /**< Analog USB1 480MHz PLL Control Register, offset: 0x1C */
  volatile uint32_t PLL_USB2;                          /**< Analog USB2 480MHz PLL Control Register, offset: 0x20 */
  volatile uint32_t PLL_USB2_SET;                      /**< Analog USB2 480MHz PLL Control Register, offset: 0x24 */
  volatile uint32_t PLL_USB2_CLR;                      /**< Analog USB2 480MHz PLL Control Register, offset: 0x28 */
  volatile uint32_t PLL_USB2_TOG;                      /**< Analog USB2 480MHz PLL Control Register, offset: 0x2C */
  volatile uint32_t PLL_SYS;                           /**< Analog System PLL Control Register, offset: 0x30 */
  volatile uint32_t PLL_SYS_SET;                       /**< Analog System PLL Control Register, offset: 0x34 */
  volatile uint32_t PLL_SYS_CLR;                       /**< Analog System PLL Control Register, offset: 0x38 */
  volatile uint32_t PLL_SYS_TOG;                       /**< Analog System PLL Control Register, offset: 0x3C */
  volatile uint32_t PLL_SYS_SS;                        /**< 528MHz System PLL Spread Spectrum Register, offset: 0x40 */
       uint8_t RESERVED_0[12];
  volatile uint32_t PLL_SYS_NUM;                       /**< Numerator of 528MHz System PLL Fractional Loop Divider Register, offset: 0x50 */
       uint8_t RESERVED_1[12];
  volatile uint32_t PLL_SYS_DENOM;                     /**< Denominator of 528MHz System PLL Fractional Loop Divider Register, offset: 0x60 */
       uint8_t RESERVED_2[12];
  volatile uint32_t PLL_AUDIO;                         /**< Analog Audio PLL control Register, offset: 0x70 */
  volatile uint32_t PLL_AUDIO_SET;                     /**< Analog Audio PLL control Register, offset: 0x74 */
  volatile uint32_t PLL_AUDIO_CLR;                     /**< Analog Audio PLL control Register, offset: 0x78 */
  volatile uint32_t PLL_AUDIO_TOG;                     /**< Analog Audio PLL control Register, offset: 0x7C */
  volatile uint32_t PLL_AUDIO_NUM;                     /**< Numerator of Audio PLL Fractional Loop Divider Register, offset: 0x80 */
       uint8_t RESERVED_3[12];
  volatile uint32_t PLL_AUDIO_DENOM;                   /**< Denominator of Audio PLL Fractional Loop Divider Register, offset: 0x90 */
       uint8_t RESERVED_4[12];
  volatile uint32_t PLL_VIDEO;                         /**< Analog Video PLL control Register, offset: 0xA0 */
  volatile uint32_t PLL_VIDEO_SET;                     /**< Analog Video PLL control Register, offset: 0xA4 */
  volatile uint32_t PLL_VIDEO_CLR;                     /**< Analog Video PLL control Register, offset: 0xA8 */
  volatile uint32_t PLL_VIDEO_TOG;                     /**< Analog Video PLL control Register, offset: 0xAC */
  volatile uint32_t PLL_VIDEO_NUM;                     /**< Numerator of Video PLL Fractional Loop Divider Register, offset: 0xB0 */
       uint8_t RESERVED_5[12];
  volatile uint32_t PLL_VIDEO_DENOM;                   /**< Denominator of Video PLL Fractional Loop Divider Register, offset: 0xC0 */
       uint8_t RESERVED_6[28];
  volatile uint32_t PLL_ENET;                          /**< Analog ENET PLL Control Register, offset: 0xE0 */
  volatile uint32_t PLL_ENET_SET;                      /**< Analog ENET PLL Control Register, offset: 0xE4 */
  volatile uint32_t PLL_ENET_CLR;                      /**< Analog ENET PLL Control Register, offset: 0xE8 */
  volatile uint32_t PLL_ENET_TOG;                      /**< Analog ENET PLL Control Register, offset: 0xEC */
  volatile uint32_t PFD_480;                           /**< 480MHz Clock (PLL3) Phase Fractional Divider Control Register, offset: 0xF0 */
  volatile uint32_t PFD_480_SET;                       /**< 480MHz Clock (PLL3) Phase Fractional Divider Control Register, offset: 0xF4 */
  volatile uint32_t PFD_480_CLR;                       /**< 480MHz Clock (PLL3) Phase Fractional Divider Control Register, offset: 0xF8 */
  volatile uint32_t PFD_480_TOG;                       /**< 480MHz Clock (PLL3) Phase Fractional Divider Control Register, offset: 0xFC */
  volatile uint32_t PFD_528;                           /**< 528MHz Clock (PLL2) Phase Fractional Divider Control Register, offset: 0x100 */
  volatile uint32_t PFD_528_SET;                       /**< 528MHz Clock (PLL2) Phase Fractional Divider Control Register, offset: 0x104 */
  volatile uint32_t PFD_528_CLR;                       /**< 528MHz Clock (PLL2) Phase Fractional Divider Control Register, offset: 0x108 */
  volatile uint32_t PFD_528_TOG;                       /**< 528MHz Clock (PLL2) Phase Fractional Divider Control Register, offset: 0x10C */
       uint8_t RESERVED_7[64];
  volatile uint32_t MISC0;                             /**< Miscellaneous Register 0, offset: 0x150 */
  volatile uint32_t MISC0_SET;                         /**< Miscellaneous Register 0, offset: 0x154 */
  volatile uint32_t MISC0_CLR;                         /**< Miscellaneous Register 0, offset: 0x158 */
  volatile uint32_t MISC0_TOG;                         /**< Miscellaneous Register 0, offset: 0x15C */
  volatile uint32_t MISC1;                             /**< Miscellaneous Register 1, offset: 0x160 */
  volatile uint32_t MISC1_SET;                         /**< Miscellaneous Register 1, offset: 0x164 */
  volatile uint32_t MISC1_CLR;                         /**< Miscellaneous Register 1, offset: 0x168 */
  volatile uint32_t MISC1_TOG;                         /**< Miscellaneous Register 1, offset: 0x16C */
  volatile uint32_t MISC2;                             /**< Miscellaneous Register 2, offset: 0x170 */
  volatile uint32_t MISC2_SET;                         /**< Miscellaneous Register 2, offset: 0x174 */
  volatile uint32_t MISC2_CLR;                         /**< Miscellaneous Register 2, offset: 0x178 */
  volatile uint32_t MISC2_TOG;                         /**< Miscellaneous Register 2, offset: 0x17C */
} CCM_ANALOG_Type;


enum _clock_pll_clk_src
{
    kCLOCK_PllClkSrc24M = 0U, /*!< Pll clock source 24M */
    kCLOCK_PllSrcClkPN = 1U,  /*!< Pll clock source CLK1_P and CLK1_N */
};

#define CCM_ANALOG_BASE                          (0x400D8000u)
/** Peripheral CCM_ANALOG base pointer */
#define CCM_ANALOG                               ((CCM_ANALOG_Type *)CCM_ANALOG_BASE)
#define CCM_ANALOG_TUPLE_REG_OFF(base, tuple, off) (*((volatile uint32_t *)((uint32_t)base + (((uint32_t)tuple >> 16U) & 0xFFFU) + off)))
#define CCM_ANALOG_TUPLE_REG(base, tuple) CCM_ANALOG_TUPLE_REG_OFF(base, tuple, 0U)
#define CCM_ANALOG_TUPLE_SHIFT(tuple) (((uint32_t)tuple) & 0x1FU)
#define CCM_ANALOG_TUPLE(reg, shift)  ((((uint32_t)(&((CCM_ANALOG_Type *)0U)->reg) & 0xFFFU) << 16U) | (shift))
typedef enum _clock_pll
{
    kCLOCK_PllArm = CCM_ANALOG_TUPLE(PLL_ARM, CCM_ANALOG_PLL_ARM_ENABLE_SHIFT),       /*!< PLL ARM */
    kCLOCK_PllSys = CCM_ANALOG_TUPLE(PLL_SYS, CCM_ANALOG_PLL_SYS_ENABLE_SHIFT),       /*!< PLL SYS */
    kCLOCK_PllUsb1 = CCM_ANALOG_TUPLE(PLL_USB1, CCM_ANALOG_PLL_USB1_ENABLE_SHIFT),    /*!< PLL USB1 */
    kCLOCK_PllAudio = CCM_ANALOG_TUPLE(PLL_AUDIO, CCM_ANALOG_PLL_AUDIO_ENABLE_SHIFT), /*!< PLL Audio */
    kCLOCK_PllVideo = CCM_ANALOG_TUPLE(PLL_VIDEO, CCM_ANALOG_PLL_VIDEO_ENABLE_SHIFT), /*!< PLL Video */
    kCLOCK_PllEnet = CCM_ANALOG_TUPLE(PLL_ENET, CCM_ANALOG_PLL_ENET_ENABLE_SHIFT), /*!< PLL Enet0 */
    kCLOCK_PllEnet25M = CCM_ANALOG_TUPLE(PLL_ENET, CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN_SHIFT), /*!< PLL Enet1 */
    kCLOCK_PllUsb2 = CCM_ANALOG_TUPLE(PLL_USB2, CCM_ANALOG_PLL_USB2_ENABLE_SHIFT), /*!< PLL USB2 */
} clock_pll_t;

/*! @brief PLL PFD name */
typedef enum _clock_pfd
{
    kCLOCK_Pfd0 = 0U, /*!< PLL PFD0 */
    kCLOCK_Pfd1 = 1U, /*!< PLL PFD1 */
    kCLOCK_Pfd2 = 2U, /*!< PLL PFD2 */
    kCLOCK_Pfd3 = 3U, /*!< PLL PFD3 */
} clock_pfd_t;

/*!
 * @brief Gets the OSC clock frequency.
 *
 * This function will return the external XTAL OSC frequency if it is selected as the source of OSC,
 * otherwise internal 24MHz RC OSC frequency will be returned.
 *
 * @param osc   OSC type to get frequency.
 *
 * @return  Clock frequency; If the clock is invalid, returns 0.
 */
static inline uint32_t CLOCK_GetOscFreq(void)
{
    return (XTALOSC24M_LOWPWR_CTRL & 0x10U) ? 24000000UL : g_xtalFreq;
}

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint32_t CLOCK_GetPeriphClkFreq(void);
uint32_t CLOCK_GetAhbFreq(void);
uint32_t CLOCK_GetSemcFreq(void);
uint32_t CLOCK_GetIpgFreq(void);
uint32_t CLOCK_GetPerClkFreq(void);
uint32_t CLOCK_GetFreq(clock_name_t name);
uint32_t CLOCK_GetPllFreq(clock_pll_t pll);
uint32_t CLOCK_GetSysPfdFreq(clock_pfd_t pfd);
uint32_t CLOCK_GetUsb1PfdFreq(clock_pfd_t pfd);

static inline uint32_t CLOCK_GetRtcFreq(void)
{
    return 32768U;
}

static inline bool CLOCK_IsPllEnabled(CCM_ANALOG_Type *base, clock_pll_t pll)
{
    return (bool)(CCM_ANALOG_TUPLE_REG(CCM_ANALOG, pll) & (1U << CCM_ANALOG_TUPLE_SHIFT(pll)));
}

static inline uint32_t CLOCK_GetPllBypassRefClk(CCM_ANALOG_Type *base, clock_pll_t pll)
{
    return (((CCM_ANALOG_TUPLE_REG(CCM_ANALOG, pll) & CCM_ANALOG_PLL_BYPASS_CLK_SRC_MASK) >>
             CCM_ANALOG_PLL_BYPASS_CLK_SRC_SHIFT) == kCLOCK_PllClkSrc24M) ?
               CLOCK_GetOscFreq() :
               CLKPN_FREQ;
}

static inline bool CLOCK_IsPllBypassed(CCM_ANALOG_Type *base, clock_pll_t pll)
{
    return (bool)(CCM_ANALOG_TUPLE_REG(CCM_ANALOG, pll) & (1U << CCM_ANALOG_PLL_BYPASS_SHIFT));
}
