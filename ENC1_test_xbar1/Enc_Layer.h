
/* ----------------------------------------------------------------------------
   -- ENC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ENC_Peripheral_Access_Layer ENC Peripheral Access Layer
 * @{
 */

/** ENC - Register Layout Typedef */
typedef struct {
  volatile uint16_t CTRL;                              /**< Control Register, offset: 0x0 */
  volatile uint16_t FILT;                              /**< Input Filter Register, offset: 0x2 */
  volatile uint16_t WTR;                               /**< Watchdog Timeout Register, offset: 0x4 */
  volatile uint16_t POSD;                              /**< Position Difference Counter Register, offset: 0x6 */
  volatile  uint16_t POSDH;                             /**< Position Difference Hold Register, offset: 0x8 */
  volatile uint16_t REV;                               /**< Revolution Counter Register, offset: 0xA */
  volatile  uint16_t REVH;                              /**< Revolution Hold Register, offset: 0xC */
  volatile uint16_t UPOS;                              /**< Upper Position Counter Register, offset: 0xE */
  volatile uint16_t LPOS;                              /**< Lower Position Counter Register, offset: 0x10 */
  volatile  uint16_t UPOSH;                             /**< Upper Position Hold Register, offset: 0x12 */
  volatile  uint16_t LPOSH;                             /**< Lower Position Hold Register, offset: 0x14 */
  volatile uint16_t UINIT;                             /**< Upper Initialization Register, offset: 0x16 */
  volatile uint16_t LINIT;                             /**< Lower Initialization Register, offset: 0x18 */
  volatile  uint16_t IMR;                               /**< Input Monitor Register, offset: 0x1A */
  volatile uint16_t TST;                               /**< Test Register, offset: 0x1C */
  volatile uint16_t CTRL2;                             /**< Control 2 Register, offset: 0x1E */
  volatile uint16_t UMOD;                              /**< Upper Modulus Register, offset: 0x20 */
  volatile uint16_t LMOD;                              /**< Lower Modulus Register, offset: 0x22 */
  volatile uint16_t UCOMP;                             /**< Upper Position Compare Register, offset: 0x24 */
  volatile uint16_t LCOMP;                             /**< Lower Position Compare Register, offset: 0x26 */
} ENC_Type;

/* ----------------------------------------------------------------------------
   -- ENC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ENC_Register_Masks ENC Register Masks
 * @{
 */

/*! @name CTRL - Control Register */
/*! @{ */
#define ENC_CTRL_CMPIE_MASK                      (0x1U)
#define ENC_CTRL_CMPIE_SHIFT                     (0U)
/*! CMPIE - Compare Interrupt Enable
 *  0b0..Compare interrupt is disabled
 *  0b1..Compare interrupt is enabled
 */
#define ENC_CTRL_CMPIE(x)                        (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_CMPIE_SHIFT)) & ENC_CTRL_CMPIE_MASK)
#define ENC_CTRL_CMPIRQ_MASK                     (0x2U)
#define ENC_CTRL_CMPIRQ_SHIFT                    (1U)
/*! CMPIRQ - Compare Interrupt Request
 *  0b0..No match has occurred
 *  0b1..COMP match has occurred
 */
#define ENC_CTRL_CMPIRQ(x)                       (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_CMPIRQ_SHIFT)) & ENC_CTRL_CMPIRQ_MASK)
#define ENC_CTRL_WDE_MASK                        (0x4U)
#define ENC_CTRL_WDE_SHIFT                       (2U)
/*! WDE - Watchdog Enable
 *  0b0..Watchdog timer is disabled
 *  0b1..Watchdog timer is enabled
 */
#define ENC_CTRL_WDE(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_WDE_SHIFT)) & ENC_CTRL_WDE_MASK)
#define ENC_CTRL_DIE_MASK                        (0x8U)
#define ENC_CTRL_DIE_SHIFT                       (3U)
/*! DIE - Watchdog Timeout Interrupt Enable
 *  0b0..Watchdog timer interrupt is disabled
 *  0b1..Watchdog timer interrupt is enabled
 */
#define ENC_CTRL_DIE(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_DIE_SHIFT)) & ENC_CTRL_DIE_MASK)
#define ENC_CTRL_DIRQ_MASK                       (0x10U)
#define ENC_CTRL_DIRQ_SHIFT                      (4U)
/*! DIRQ - Watchdog Timeout Interrupt Request
 *  0b0..No interrupt has occurred
 *  0b1..Watchdog timeout interrupt has occurred
 */
#define ENC_CTRL_DIRQ(x)                         (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_DIRQ_SHIFT)) & ENC_CTRL_DIRQ_MASK)
#define ENC_CTRL_XNE_MASK                        (0x20U)
#define ENC_CTRL_XNE_SHIFT                       (5U)
/*! XNE - Use Negative Edge of INDEX Pulse
 *  0b0..Use positive transition edge of INDEX pulse
 *  0b1..Use negative transition edge of INDEX pulse
 */
#define ENC_CTRL_XNE(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_XNE_SHIFT)) & ENC_CTRL_XNE_MASK)
#define ENC_CTRL_XIP_MASK                        (0x40U)
#define ENC_CTRL_XIP_SHIFT                       (6U)
/*! XIP - INDEX Triggered Initialization of Position Counters UPOS and LPOS
 *  0b0..No action
 *  0b1..INDEX pulse initializes the position counter
 */
#define ENC_CTRL_XIP(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_XIP_SHIFT)) & ENC_CTRL_XIP_MASK)
#define ENC_CTRL_XIE_MASK                        (0x80U)
#define ENC_CTRL_XIE_SHIFT                       (7U)
/*! XIE - INDEX Pulse Interrupt Enable
 *  0b0..INDEX pulse interrupt is disabled
 *  0b1..INDEX pulse interrupt is enabled
 */
#define ENC_CTRL_XIE(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_XIE_SHIFT)) & ENC_CTRL_XIE_MASK)
#define ENC_CTRL_XIRQ_MASK                       (0x100U)
#define ENC_CTRL_XIRQ_SHIFT                      (8U)
/*! XIRQ - INDEX Pulse Interrupt Request
 *  0b0..No interrupt has occurred
 *  0b1..INDEX pulse interrupt has occurred
 */
#define ENC_CTRL_XIRQ(x)                         (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_XIRQ_SHIFT)) & ENC_CTRL_XIRQ_MASK)
#define ENC_CTRL_PH1_MASK                        (0x200U)
#define ENC_CTRL_PH1_SHIFT                       (9U)
/*! PH1 - Enable Signal Phase Count Mode
 *  0b0..Use standard quadrature decoder where PHASEA and PHASEB represent a two phase quadrature signal.
 *  0b1..Bypass the quadrature decoder. A positive transition of the PHASEA input generates a count signal. The PHASEB input and the REV bit control the counter direction. If CTRL[REV] = 0, PHASEB = 0, then count up If CTRL[REV] = 0, PHASEB = 1, then count down If CTRL[REV] = 1, PHASEB = 0, then count down If CTRL[REV] = 1, PHASEB = 1, then count up
 */
#define ENC_CTRL_PH1(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_PH1_SHIFT)) & ENC_CTRL_PH1_MASK)
#define ENC_CTRL_REV_MASK                        (0x400U)
#define ENC_CTRL_REV_SHIFT                       (10U)
/*! REV - Enable Reverse Direction Counting
 *  0b0..Count normally
 *  0b1..Count in the reverse direction
 */
#define ENC_CTRL_REV(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_REV_SHIFT)) & ENC_CTRL_REV_MASK)
#define ENC_CTRL_SWIP_MASK                       (0x800U)
#define ENC_CTRL_SWIP_SHIFT                      (11U)
/*! SWIP - Software Triggered Initialization of Position Counters UPOS and LPOS
 *  0b0..No action
 *  0b1..Initialize position counter
 */
#define ENC_CTRL_SWIP(x)                         (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_SWIP_SHIFT)) & ENC_CTRL_SWIP_MASK)
#define ENC_CTRL_HNE_MASK                        (0x1000U)
#define ENC_CTRL_HNE_SHIFT                       (12U)
/*! HNE - Use Negative Edge of HOME Input
 *  0b0..Use positive going edge-to-trigger initialization of position counters UPOS and LPOS
 *  0b1..Use negative going edge-to-trigger initialization of position counters UPOS and LPOS
 */
#define ENC_CTRL_HNE(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_HNE_SHIFT)) & ENC_CTRL_HNE_MASK)
#define ENC_CTRL_HIP_MASK                        (0x2000U)
#define ENC_CTRL_HIP_SHIFT                       (13U)
/*! HIP - Enable HOME to Initialize Position Counters UPOS and LPOS
 *  0b0..No action
 *  0b1..HOME signal initializes the position counter
 */
#define ENC_CTRL_HIP(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_HIP_SHIFT)) & ENC_CTRL_HIP_MASK)
#define ENC_CTRL_HIE_MASK                        (0x4000U)
#define ENC_CTRL_HIE_SHIFT                       (14U)
/*! HIE - HOME Interrupt Enable
 *  0b0..Disable HOME interrupts
 *  0b1..Enable HOME interrupts
 */
#define ENC_CTRL_HIE(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_HIE_SHIFT)) & ENC_CTRL_HIE_MASK)
#define ENC_CTRL_HIRQ_MASK                       (0x8000U)
#define ENC_CTRL_HIRQ_SHIFT                      (15U)
/*! HIRQ - HOME Signal Transition Interrupt Request
 *  0b0..No interrupt
 *  0b1..HOME signal transition interrupt request
 */
#define ENC_CTRL_HIRQ(x)                         (((uint16_t)(((uint16_t)(x)) << ENC_CTRL_HIRQ_SHIFT)) & ENC_CTRL_HIRQ_MASK)
/*! @} */

/*! @name FILT - Input Filter Register */
/*! @{ */
#define ENC_FILT_FILT_PER_MASK                   (0xFFU)
#define ENC_FILT_FILT_PER_SHIFT                  (0U)
#define ENC_FILT_FILT_PER(x)                     (((uint16_t)(((uint16_t)(x)) << ENC_FILT_FILT_PER_SHIFT)) & ENC_FILT_FILT_PER_MASK)
#define ENC_FILT_FILT_CNT_MASK                   (0x700U)
#define ENC_FILT_FILT_CNT_SHIFT                  (8U)
#define ENC_FILT_FILT_CNT(x)                     (((uint16_t)(((uint16_t)(x)) << ENC_FILT_FILT_CNT_SHIFT)) & ENC_FILT_FILT_CNT_MASK)
/*! @} */

/*! @name WTR - Watchdog Timeout Register */
/*! @{ */
#define ENC_WTR_WDOG_MASK                        (0xFFFFU)
#define ENC_WTR_WDOG_SHIFT                       (0U)
#define ENC_WTR_WDOG(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_WTR_WDOG_SHIFT)) & ENC_WTR_WDOG_MASK)
/*! @} */

/*! @name POSD - Position Difference Counter Register */
/*! @{ */
#define ENC_POSD_POSD_MASK                       (0xFFFFU)
#define ENC_POSD_POSD_SHIFT                      (0U)
#define ENC_POSD_POSD(x)                         (((uint16_t)(((uint16_t)(x)) << ENC_POSD_POSD_SHIFT)) & ENC_POSD_POSD_MASK)
/*! @} */

/*! @name POSDH - Position Difference Hold Register */
/*! @{ */
#define ENC_POSDH_POSDH_MASK                     (0xFFFFU)
#define ENC_POSDH_POSDH_SHIFT                    (0U)
#define ENC_POSDH_POSDH(x)                       (((uint16_t)(((uint16_t)(x)) << ENC_POSDH_POSDH_SHIFT)) & ENC_POSDH_POSDH_MASK)
/*! @} */

/*! @name REV - Revolution Counter Register */
/*! @{ */
#define ENC_REV_REV_MASK                         (0xFFFFU)
#define ENC_REV_REV_SHIFT                        (0U)
#define ENC_REV_REV(x)                           (((uint16_t)(((uint16_t)(x)) << ENC_REV_REV_SHIFT)) & ENC_REV_REV_MASK)
/*! @} */

/*! @name REVH - Revolution Hold Register */
/*! @{ */
#define ENC_REVH_REVH_MASK                       (0xFFFFU)
#define ENC_REVH_REVH_SHIFT                      (0U)
#define ENC_REVH_REVH(x)                         (((uint16_t)(((uint16_t)(x)) << ENC_REVH_REVH_SHIFT)) & ENC_REVH_REVH_MASK)
/*! @} */

/*! @name UPOS - Upper Position Counter Register */
/*! @{ */
#define ENC_UPOS_POS_MASK                        (0xFFFFU)
#define ENC_UPOS_POS_SHIFT                       (0U)
#define ENC_UPOS_POS(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_UPOS_POS_SHIFT)) & ENC_UPOS_POS_MASK)
/*! @} */

/*! @name LPOS - Lower Position Counter Register */
/*! @{ */
#define ENC_LPOS_POS_MASK                        (0xFFFFU)
#define ENC_LPOS_POS_SHIFT                       (0U)
#define ENC_LPOS_POS(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_LPOS_POS_SHIFT)) & ENC_LPOS_POS_MASK)
/*! @} */

/*! @name UPOSH - Upper Position Hold Register */
/*! @{ */
#define ENC_UPOSH_POSH_MASK                      (0xFFFFU)
#define ENC_UPOSH_POSH_SHIFT                     (0U)
#define ENC_UPOSH_POSH(x)                        (((uint16_t)(((uint16_t)(x)) << ENC_UPOSH_POSH_SHIFT)) & ENC_UPOSH_POSH_MASK)
/*! @} */

/*! @name LPOSH - Lower Position Hold Register */
/*! @{ */
#define ENC_LPOSH_POSH_MASK                      (0xFFFFU)
#define ENC_LPOSH_POSH_SHIFT                     (0U)
#define ENC_LPOSH_POSH(x)                        (((uint16_t)(((uint16_t)(x)) << ENC_LPOSH_POSH_SHIFT)) & ENC_LPOSH_POSH_MASK)
/*! @} */

/*! @name UINIT - Upper Initialization Register */
/*! @{ */
#define ENC_UINIT_INIT_MASK                      (0xFFFFU)
#define ENC_UINIT_INIT_SHIFT                     (0U)
#define ENC_UINIT_INIT(x)                        (((uint16_t)(((uint16_t)(x)) << ENC_UINIT_INIT_SHIFT)) & ENC_UINIT_INIT_MASK)
/*! @} */

/*! @name LINIT - Lower Initialization Register */
/*! @{ */
#define ENC_LINIT_INIT_MASK                      (0xFFFFU)
#define ENC_LINIT_INIT_SHIFT                     (0U)
#define ENC_LINIT_INIT(x)                        (((uint16_t)(((uint16_t)(x)) << ENC_LINIT_INIT_SHIFT)) & ENC_LINIT_INIT_MASK)
/*! @} */

/*! @name IMR - Input Monitor Register */
/*! @{ */
#define ENC_IMR_HOME_MASK                        (0x1U)
#define ENC_IMR_HOME_SHIFT                       (0U)
#define ENC_IMR_HOME(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_IMR_HOME_SHIFT)) & ENC_IMR_HOME_MASK)
#define ENC_IMR_INDEX_MASK                       (0x2U)
#define ENC_IMR_INDEX_SHIFT                      (1U)
#define ENC_IMR_INDEX(x)                         (((uint16_t)(((uint16_t)(x)) << ENC_IMR_INDEX_SHIFT)) & ENC_IMR_INDEX_MASK)
#define ENC_IMR_PHB_MASK                         (0x4U)
#define ENC_IMR_PHB_SHIFT                        (2U)
#define ENC_IMR_PHB(x)                           (((uint16_t)(((uint16_t)(x)) << ENC_IMR_PHB_SHIFT)) & ENC_IMR_PHB_MASK)
#define ENC_IMR_PHA_MASK                         (0x8U)
#define ENC_IMR_PHA_SHIFT                        (3U)
#define ENC_IMR_PHA(x)                           (((uint16_t)(((uint16_t)(x)) << ENC_IMR_PHA_SHIFT)) & ENC_IMR_PHA_MASK)
#define ENC_IMR_FHOM_MASK                        (0x10U)
#define ENC_IMR_FHOM_SHIFT                       (4U)
#define ENC_IMR_FHOM(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_IMR_FHOM_SHIFT)) & ENC_IMR_FHOM_MASK)
#define ENC_IMR_FIND_MASK                        (0x20U)
#define ENC_IMR_FIND_SHIFT                       (5U)
#define ENC_IMR_FIND(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_IMR_FIND_SHIFT)) & ENC_IMR_FIND_MASK)
#define ENC_IMR_FPHB_MASK                        (0x40U)
#define ENC_IMR_FPHB_SHIFT                       (6U)
#define ENC_IMR_FPHB(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_IMR_FPHB_SHIFT)) & ENC_IMR_FPHB_MASK)
#define ENC_IMR_FPHA_MASK                        (0x80U)
#define ENC_IMR_FPHA_SHIFT                       (7U)
#define ENC_IMR_FPHA(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_IMR_FPHA_SHIFT)) & ENC_IMR_FPHA_MASK)
/*! @} */

/*! @name TST - Test Register */
/*! @{ */
#define ENC_TST_TEST_COUNT_MASK                  (0xFFU)
#define ENC_TST_TEST_COUNT_SHIFT                 (0U)
#define ENC_TST_TEST_COUNT(x)                    (((uint16_t)(((uint16_t)(x)) << ENC_TST_TEST_COUNT_SHIFT)) & ENC_TST_TEST_COUNT_MASK)
#define ENC_TST_TEST_PERIOD_MASK                 (0x1F00U)
#define ENC_TST_TEST_PERIOD_SHIFT                (8U)
#define ENC_TST_TEST_PERIOD(x)                   (((uint16_t)(((uint16_t)(x)) << ENC_TST_TEST_PERIOD_SHIFT)) & ENC_TST_TEST_PERIOD_MASK)
#define ENC_TST_QDN_MASK                         (0x2000U)
#define ENC_TST_QDN_SHIFT                        (13U)
/*! QDN - Quadrature Decoder Negative Signal
 *  0b0..Leaves quadrature decoder signal in a positive direction
 *  0b1..Generates a negative quadrature decoder signal
 */
#define ENC_TST_QDN(x)                           (((uint16_t)(((uint16_t)(x)) << ENC_TST_QDN_SHIFT)) & ENC_TST_QDN_MASK)
#define ENC_TST_TCE_MASK                         (0x4000U)
#define ENC_TST_TCE_SHIFT                        (14U)
/*! TCE - Test Counter Enable
 *  0b0..Test count is not enabled
 *  0b1..Test count is enabled
 */
#define ENC_TST_TCE(x)                           (((uint16_t)(((uint16_t)(x)) << ENC_TST_TCE_SHIFT)) & ENC_TST_TCE_MASK)
#define ENC_TST_TEN_MASK                         (0x8000U)
#define ENC_TST_TEN_SHIFT                        (15U)
/*! TEN - Test Mode Enable
 *  0b0..Test module is not enabled
 *  0b1..Test module is enabled
 */
#define ENC_TST_TEN(x)                           (((uint16_t)(((uint16_t)(x)) << ENC_TST_TEN_SHIFT)) & ENC_TST_TEN_MASK)
/*! @} */

/*! @name CTRL2 - Control 2 Register */
/*! @{ */
#define ENC_CTRL2_UPDHLD_MASK                    (0x1U)
#define ENC_CTRL2_UPDHLD_SHIFT                   (0U)
/*! UPDHLD - Update Hold Registers
 *  0b0..Disable updates of hold registers on rising edge of TRIGGER
 *  0b1..Enable updates of hold registers on rising edge of TRIGGER
 */
#define ENC_CTRL2_UPDHLD(x)                      (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_UPDHLD_SHIFT)) & ENC_CTRL2_UPDHLD_MASK)
#define ENC_CTRL2_UPDPOS_MASK                    (0x2U)
#define ENC_CTRL2_UPDPOS_SHIFT                   (1U)
/*! UPDPOS - Update Position Registers
 *  0b0..No action for POSD, REV, UPOS and LPOS on rising edge of TRIGGER
 *  0b1..Clear POSD, REV, UPOS and LPOS on rising edge of TRIGGER
 */
#define ENC_CTRL2_UPDPOS(x)                      (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_UPDPOS_SHIFT)) & ENC_CTRL2_UPDPOS_MASK)
#define ENC_CTRL2_MOD_MASK                       (0x4U)
#define ENC_CTRL2_MOD_SHIFT                      (2U)
/*! MOD - Enable Modulo Counting
 *  0b0..Disable modulo counting
 *  0b1..Enable modulo counting
 */
#define ENC_CTRL2_MOD(x)                         (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_MOD_SHIFT)) & ENC_CTRL2_MOD_MASK)
#define ENC_CTRL2_DIR_MASK                       (0x8U)
#define ENC_CTRL2_DIR_SHIFT                      (3U)
/*! DIR - Count Direction Flag
 *  0b0..Last count was in the down direction
 *  0b1..Last count was in the up direction
 */
#define ENC_CTRL2_DIR(x)                         (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_DIR_SHIFT)) & ENC_CTRL2_DIR_MASK)
#define ENC_CTRL2_RUIE_MASK                      (0x10U)
#define ENC_CTRL2_RUIE_SHIFT                     (4U)
/*! RUIE - Roll-under Interrupt Enable
 *  0b0..Roll-under interrupt is disabled
 *  0b1..Roll-under interrupt is enabled
 */
#define ENC_CTRL2_RUIE(x)                        (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_RUIE_SHIFT)) & ENC_CTRL2_RUIE_MASK)
#define ENC_CTRL2_RUIRQ_MASK                     (0x20U)
#define ENC_CTRL2_RUIRQ_SHIFT                    (5U)
/*! RUIRQ - Roll-under Interrupt Request
 *  0b0..No roll-under has occurred
 *  0b1..Roll-under has occurred
 */
#define ENC_CTRL2_RUIRQ(x)                       (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_RUIRQ_SHIFT)) & ENC_CTRL2_RUIRQ_MASK)
#define ENC_CTRL2_ROIE_MASK                      (0x40U)
#define ENC_CTRL2_ROIE_SHIFT                     (6U)
/*! ROIE - Roll-over Interrupt Enable
 *  0b0..Roll-over interrupt is disabled
 *  0b1..Roll-over interrupt is enabled
 */
#define ENC_CTRL2_ROIE(x)                        (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_ROIE_SHIFT)) & ENC_CTRL2_ROIE_MASK)
#define ENC_CTRL2_ROIRQ_MASK                     (0x80U)
#define ENC_CTRL2_ROIRQ_SHIFT                    (7U)
/*! ROIRQ - Roll-over Interrupt Request
 *  0b0..No roll-over has occurred
 *  0b1..Roll-over has occurred
 */
#define ENC_CTRL2_ROIRQ(x)                       (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_ROIRQ_SHIFT)) & ENC_CTRL2_ROIRQ_MASK)
#define ENC_CTRL2_REVMOD_MASK                    (0x100U)
#define ENC_CTRL2_REVMOD_SHIFT                   (8U)
/*! REVMOD - Revolution Counter Modulus Enable
 *  0b0..Use INDEX pulse to increment/decrement revolution counter (REV).
 *  0b1..Use modulus counting roll-over/under to increment/decrement revolution counter (REV).
 */
#define ENC_CTRL2_REVMOD(x)                      (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_REVMOD_SHIFT)) & ENC_CTRL2_REVMOD_MASK)
#define ENC_CTRL2_OUTCTL_MASK                    (0x200U)
#define ENC_CTRL2_OUTCTL_SHIFT                   (9U)
/*! OUTCTL - Output Control
 *  0b0..POSMATCH pulses when a match occurs between the position counters (POS) and the compare value (COMP).
 *  0b1..POSMATCH pulses when the UPOS, LPOS, REV, or POSD registers are read.
 */
#define ENC_CTRL2_OUTCTL(x)                      (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_OUTCTL_SHIFT)) & ENC_CTRL2_OUTCTL_MASK)
#define ENC_CTRL2_SABIE_MASK                     (0x400U)
#define ENC_CTRL2_SABIE_SHIFT                    (10U)
/*! SABIE - Simultaneous PHASEA and PHASEB Change Interrupt Enable
 *  0b0..Simultaneous PHASEA and PHASEB change interrupt disabled.
 *  0b1..Simultaneous PHASEA and PHASEB change interrupt enabled.
 */
#define ENC_CTRL2_SABIE(x)                       (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_SABIE_SHIFT)) & ENC_CTRL2_SABIE_MASK)
#define ENC_CTRL2_SABIRQ_MASK                    (0x800U)
#define ENC_CTRL2_SABIRQ_SHIFT                   (11U)
/*! SABIRQ - Simultaneous PHASEA and PHASEB Change Interrupt Request
 *  0b0..No simultaneous change of PHASEA and PHASEB has occurred.
 *  0b1..A simultaneous change of PHASEA and PHASEB has occurred.
 */
#define ENC_CTRL2_SABIRQ(x)                      (((uint16_t)(((uint16_t)(x)) << ENC_CTRL2_SABIRQ_SHIFT)) & ENC_CTRL2_SABIRQ_MASK)
/*! @} */

/*! @name UMOD - Upper Modulus Register */
/*! @{ */
#define ENC_UMOD_MOD_MASK                        (0xFFFFU)
#define ENC_UMOD_MOD_SHIFT                       (0U)
#define ENC_UMOD_MOD(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_UMOD_MOD_SHIFT)) & ENC_UMOD_MOD_MASK)
/*! @} */

/*! @name LMOD - Lower Modulus Register */
/*! @{ */
#define ENC_LMOD_MOD_MASK                        (0xFFFFU)
#define ENC_LMOD_MOD_SHIFT                       (0U)
#define ENC_LMOD_MOD(x)                          (((uint16_t)(((uint16_t)(x)) << ENC_LMOD_MOD_SHIFT)) & ENC_LMOD_MOD_MASK)
/*! @} */

/*! @name UCOMP - Upper Position Compare Register */
/*! @{ */
#define ENC_UCOMP_COMP_MASK                      (0xFFFFU)
#define ENC_UCOMP_COMP_SHIFT                     (0U)
#define ENC_UCOMP_COMP(x)                        (((uint16_t)(((uint16_t)(x)) << ENC_UCOMP_COMP_SHIFT)) & ENC_UCOMP_COMP_MASK)
/*! @} */

/*! @name LCOMP - Lower Position Compare Register */
/*! @{ */
#define ENC_LCOMP_COMP_MASK                      (0xFFFFU)
#define ENC_LCOMP_COMP_SHIFT                     (0U)
#define ENC_LCOMP_COMP(x)                        (((uint16_t)(((uint16_t)(x)) << ENC_LCOMP_COMP_SHIFT)) & ENC_LCOMP_COMP_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group ENC_Register_Masks */


/* ENC - Peripheral instance base addresses */
/** Peripheral ENC1 base address */
#define ENC1_BASE                                (0x403C8000u)
/** Peripheral ENC1 base pointer */
#define ENC1                                     ((ENC_Type *)ENC1_BASE)
/** Peripheral ENC2 base address */
#define ENC2_BASE                                (0x403CC000u)
/** Peripheral ENC2 base pointer */
#define ENC2                                     ((ENC_Type *)ENC2_BASE)
/** Peripheral ENC3 base address */
#define ENC3_BASE                                (0x403D0000u)
/** Peripheral ENC3 base pointer */
#define ENC3                                     ((ENC_Type *)ENC3_BASE)
/** Peripheral ENC4 base address */
#define ENC4_BASE                                (0x403D4000u)
/** Peripheral ENC4 base pointer */
#define ENC4                                     ((ENC_Type *)ENC4_BASE)
/** Array initializer of ENC peripheral base addresses */
#define ENC_BASE_ADDRS                           { 0u, ENC1_BASE, ENC2_BASE, ENC3_BASE, ENC4_BASE }
/** Array initializer of ENC peripheral base pointers */
#define ENC_BASE_PTRS                            { (ENC_Type *)0u, ENC1, ENC2, ENC3, ENC4 }
/** Interrupt vectors for the ENC peripheral type */
#define ENC_COMPARE_IRQS                         { NotAvail_IRQn, ENC1_IRQn, ENC2_IRQn, ENC3_IRQn, ENC4_IRQn }
#define ENC_HOME_IRQS                            { NotAvail_IRQn, ENC1_IRQn, ENC2_IRQn, ENC3_IRQn, ENC4_IRQn }
#define ENC_WDOG_IRQS                            { NotAvail_IRQn, ENC1_IRQn, ENC2_IRQn, ENC3_IRQn, ENC4_IRQn }
#define ENC_INDEX_IRQS                           { NotAvail_IRQn, ENC1_IRQn, ENC2_IRQn, ENC3_IRQn, ENC4_IRQn }
#define ENC_INPUT_SWITCH_IRQS                    { NotAvail_IRQn, ENC1_IRQn, ENC2_IRQn, ENC3_IRQn, ENC4_IRQn }

/*!
 * @}
 */ /* end of group ENC_Peripheral_Access_Layer */
