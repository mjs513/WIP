
#define kXBARA1_InputPitTrigger0         56         /**< PIT_TRIGGER0 output assigned to XBARA1_IN56 input. */
#define kXBARA1_OutputAdcEtcXbar0Trig0  103  /**< XBARA1_OUT103 output assigned to ADC_ETC_XBAR0_TRIG0 */

/* ----------------------------------------------------------------------------
   -- ADC_ETC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_ETC_Peripheral_Access_Layer ADC_ETC Peripheral Access Layer
 * @{
 */

/** ADC_ETC - Register Layout Typedef */
typedef struct {
  volatile uint32_t CTRL;                              /**< ADC_ETC Global Control Register, offset: 0x0 */
  volatile uint32_t DONE0_1_IRQ;                       /**< ETC DONE0 and DONE1 IRQ State Register, offset: 0x4 */
  volatile uint32_t DONE2_ERR_IRQ;                     /**< ETC DONE_2 and DONE_ERR IRQ State Register, offset: 0x8 */
  volatile uint32_t DMA_CTRL;                          /**< ETC DMA control Register, offset: 0xC */
  struct {                                         /* offset: 0x10, array step: 0x28 */
    volatile uint32_t TRIGn_CTRL;                        /**< ETC_TRIG0 Control Register..ETC_TRIG7 Control Register, array offset: 0x10, array step: 0x28 */
    volatile uint32_t TRIGn_COUNTER;                     /**< ETC_TRIG0 Counter Register..ETC_TRIG7 Counter Register, array offset: 0x14, array step: 0x28 */
    volatile uint32_t TRIGn_CHAIN_1_0;                   /**< ETC_TRIG Chain 0/1 Register, array offset: 0x18, array step: 0x28 */
    volatile uint32_t TRIGn_CHAIN_3_2;                   /**< ETC_TRIG Chain 2/3 Register, array offset: 0x1C, array step: 0x28 */
    volatile uint32_t TRIGn_CHAIN_5_4;                   /**< ETC_TRIG Chain 4/5 Register, array offset: 0x20, array step: 0x28 */
    volatile uint32_t TRIGn_CHAIN_7_6;                   /**< ETC_TRIG Chain 6/7 Register, array offset: 0x24, array step: 0x28 */
    volatile  uint32_t TRIGn_RESULT_1_0;                  /**< ETC_TRIG Result Data 1/0 Register, array offset: 0x28, array step: 0x28 */
    volatile  uint32_t TRIGn_RESULT_3_2;                  /**< ETC_TRIG Result Data 3/2 Register, array offset: 0x2C, array step: 0x28 */
    volatile  uint32_t TRIGn_RESULT_5_4;                  /**< ETC_TRIG Result Data 5/4 Register, array offset: 0x30, array step: 0x28 */
    volatile  uint32_t TRIGn_RESULT_7_6;                  /**< ETC_TRIG Result Data 7/6 Register, array offset: 0x34, array step: 0x28 */
  } TRIG[8];
} ADC_ETC_Type;

/* ----------------------------------------------------------------------------
   -- ADC_ETC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_ETC_Register_Masks ADC_ETC Register Masks
 * @{
 */

/*! @name CTRL - ADC_ETC Global Control Register */
/*! @{ */
#define ADC_ETC_CTRL_TRIG_ENABLE_MASK            (0xFFU)
#define ADC_ETC_CTRL_TRIG_ENABLE_SHIFT           (0U)
#define ADC_ETC_CTRL_TRIG_ENABLE(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ETC_CTRL_TRIG_ENABLE_SHIFT)) & ADC_ETC_CTRL_TRIG_ENABLE_MASK)
#define ADC_ETC_CTRL_EXT0_TRIG_ENABLE_MASK       (0x100U)
#define ADC_ETC_CTRL_EXT0_TRIG_ENABLE_SHIFT      (8U)
#define ADC_ETC_CTRL_EXT0_TRIG_ENABLE(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_CTRL_EXT0_TRIG_ENABLE_SHIFT)) & ADC_ETC_CTRL_EXT0_TRIG_ENABLE_MASK)
#define ADC_ETC_CTRL_EXT0_TRIG_PRIORITY_MASK     (0xE00U)
#define ADC_ETC_CTRL_EXT0_TRIG_PRIORITY_SHIFT    (9U)
#define ADC_ETC_CTRL_EXT0_TRIG_PRIORITY(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_CTRL_EXT0_TRIG_PRIORITY_SHIFT)) & ADC_ETC_CTRL_EXT0_TRIG_PRIORITY_MASK)
#define ADC_ETC_CTRL_EXT1_TRIG_ENABLE_MASK       (0x1000U)
#define ADC_ETC_CTRL_EXT1_TRIG_ENABLE_SHIFT      (12U)
#define ADC_ETC_CTRL_EXT1_TRIG_ENABLE(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_CTRL_EXT1_TRIG_ENABLE_SHIFT)) & ADC_ETC_CTRL_EXT1_TRIG_ENABLE_MASK)
#define ADC_ETC_CTRL_EXT1_TRIG_PRIORITY_MASK     (0xE000U)
#define ADC_ETC_CTRL_EXT1_TRIG_PRIORITY_SHIFT    (13U)
#define ADC_ETC_CTRL_EXT1_TRIG_PRIORITY(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_CTRL_EXT1_TRIG_PRIORITY_SHIFT)) & ADC_ETC_CTRL_EXT1_TRIG_PRIORITY_MASK)
#define ADC_ETC_CTRL_PRE_DIVIDER_MASK            (0xFF0000U)
#define ADC_ETC_CTRL_PRE_DIVIDER_SHIFT           (16U)
#define ADC_ETC_CTRL_PRE_DIVIDER(x)              (((uint32_t)(((uint32_t)(x)) << ADC_ETC_CTRL_PRE_DIVIDER_SHIFT)) & ADC_ETC_CTRL_PRE_DIVIDER_MASK)
#define ADC_ETC_CTRL_DMA_MODE_SEL_MASK           (0x20000000U)
#define ADC_ETC_CTRL_DMA_MODE_SEL_SHIFT          (29U)
#define ADC_ETC_CTRL_DMA_MODE_SEL(x)             (((uint32_t)(((uint32_t)(x)) << ADC_ETC_CTRL_DMA_MODE_SEL_SHIFT)) & ADC_ETC_CTRL_DMA_MODE_SEL_MASK)
#define ADC_ETC_CTRL_TSC_BYPASS_MASK             (0x40000000U)
#define ADC_ETC_CTRL_TSC_BYPASS_SHIFT            (30U)
#define ADC_ETC_CTRL_TSC_BYPASS(x)               (((uint32_t)(((uint32_t)(x)) << ADC_ETC_CTRL_TSC_BYPASS_SHIFT)) & ADC_ETC_CTRL_TSC_BYPASS_MASK)
#define ADC_ETC_CTRL_SOFTRST_MASK                (0x80000000U)
#define ADC_ETC_CTRL_SOFTRST_SHIFT               (31U)
#define ADC_ETC_CTRL_SOFTRST(x)                  (((uint32_t)(((uint32_t)(x)) << ADC_ETC_CTRL_SOFTRST_SHIFT)) & ADC_ETC_CTRL_SOFTRST_MASK)
/*! @} */

/*! @name DONE0_1_IRQ - ETC DONE0 and DONE1 IRQ State Register */
/*! @{ */
#define ADC_ETC_DONE0_1_IRQ_TRIG0_DONE0_MASK     (0x1U)
#define ADC_ETC_DONE0_1_IRQ_TRIG0_DONE0_SHIFT    (0U)
#define ADC_ETC_DONE0_1_IRQ_TRIG0_DONE0(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG0_DONE0_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG0_DONE0_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG1_DONE0_MASK     (0x2U)
#define ADC_ETC_DONE0_1_IRQ_TRIG1_DONE0_SHIFT    (1U)
#define ADC_ETC_DONE0_1_IRQ_TRIG1_DONE0(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG1_DONE0_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG1_DONE0_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG2_DONE0_MASK     (0x4U)
#define ADC_ETC_DONE0_1_IRQ_TRIG2_DONE0_SHIFT    (2U)
#define ADC_ETC_DONE0_1_IRQ_TRIG2_DONE0(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG2_DONE0_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG2_DONE0_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG3_DONE0_MASK     (0x8U)
#define ADC_ETC_DONE0_1_IRQ_TRIG3_DONE0_SHIFT    (3U)
#define ADC_ETC_DONE0_1_IRQ_TRIG3_DONE0(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG3_DONE0_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG3_DONE0_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG4_DONE0_MASK     (0x10U)
#define ADC_ETC_DONE0_1_IRQ_TRIG4_DONE0_SHIFT    (4U)
#define ADC_ETC_DONE0_1_IRQ_TRIG4_DONE0(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG4_DONE0_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG4_DONE0_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG5_DONE0_MASK     (0x20U)
#define ADC_ETC_DONE0_1_IRQ_TRIG5_DONE0_SHIFT    (5U)
#define ADC_ETC_DONE0_1_IRQ_TRIG5_DONE0(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG5_DONE0_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG5_DONE0_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG6_DONE0_MASK     (0x40U)
#define ADC_ETC_DONE0_1_IRQ_TRIG6_DONE0_SHIFT    (6U)
#define ADC_ETC_DONE0_1_IRQ_TRIG6_DONE0(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG6_DONE0_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG6_DONE0_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG7_DONE0_MASK     (0x80U)
#define ADC_ETC_DONE0_1_IRQ_TRIG7_DONE0_SHIFT    (7U)
#define ADC_ETC_DONE0_1_IRQ_TRIG7_DONE0(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG7_DONE0_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG7_DONE0_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG0_DONE1_MASK     (0x10000U)
#define ADC_ETC_DONE0_1_IRQ_TRIG0_DONE1_SHIFT    (16U)
#define ADC_ETC_DONE0_1_IRQ_TRIG0_DONE1(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG0_DONE1_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG0_DONE1_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG1_DONE1_MASK     (0x20000U)
#define ADC_ETC_DONE0_1_IRQ_TRIG1_DONE1_SHIFT    (17U)
#define ADC_ETC_DONE0_1_IRQ_TRIG1_DONE1(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG1_DONE1_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG1_DONE1_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG2_DONE1_MASK     (0x40000U)
#define ADC_ETC_DONE0_1_IRQ_TRIG2_DONE1_SHIFT    (18U)
#define ADC_ETC_DONE0_1_IRQ_TRIG2_DONE1(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG2_DONE1_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG2_DONE1_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG3_DONE1_MASK     (0x80000U)
#define ADC_ETC_DONE0_1_IRQ_TRIG3_DONE1_SHIFT    (19U)
#define ADC_ETC_DONE0_1_IRQ_TRIG3_DONE1(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG3_DONE1_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG3_DONE1_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG4_DONE1_MASK     (0x100000U)
#define ADC_ETC_DONE0_1_IRQ_TRIG4_DONE1_SHIFT    (20U)
#define ADC_ETC_DONE0_1_IRQ_TRIG4_DONE1(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG4_DONE1_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG4_DONE1_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG5_DONE1_MASK     (0x200000U)
#define ADC_ETC_DONE0_1_IRQ_TRIG5_DONE1_SHIFT    (21U)
#define ADC_ETC_DONE0_1_IRQ_TRIG5_DONE1(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG5_DONE1_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG5_DONE1_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG6_DONE1_MASK     (0x400000U)
#define ADC_ETC_DONE0_1_IRQ_TRIG6_DONE1_SHIFT    (22U)
#define ADC_ETC_DONE0_1_IRQ_TRIG6_DONE1(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG6_DONE1_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG6_DONE1_MASK)
#define ADC_ETC_DONE0_1_IRQ_TRIG7_DONE1_MASK     (0x800000U)
#define ADC_ETC_DONE0_1_IRQ_TRIG7_DONE1_SHIFT    (23U)
#define ADC_ETC_DONE0_1_IRQ_TRIG7_DONE1(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE0_1_IRQ_TRIG7_DONE1_SHIFT)) & ADC_ETC_DONE0_1_IRQ_TRIG7_DONE1_MASK)
/*! @} */

/*! @name DONE2_ERR_IRQ - ETC DONE_2 and DONE_ERR IRQ State Register */
/*! @{ */
#define ADC_ETC_DONE2_ERR_IRQ_TRIG0_DONE2_MASK   (0x1U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG0_DONE2_SHIFT  (0U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG0_DONE2(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG0_DONE2_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG0_DONE2_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG1_DONE2_MASK   (0x2U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG1_DONE2_SHIFT  (1U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG1_DONE2(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG1_DONE2_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG1_DONE2_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG2_DONE2_MASK   (0x4U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG2_DONE2_SHIFT  (2U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG2_DONE2(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG2_DONE2_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG2_DONE2_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG3_DONE2_MASK   (0x8U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG3_DONE2_SHIFT  (3U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG3_DONE2(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG3_DONE2_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG3_DONE2_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG4_DONE2_MASK   (0x10U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG4_DONE2_SHIFT  (4U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG4_DONE2(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG4_DONE2_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG4_DONE2_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG5_DONE2_MASK   (0x20U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG5_DONE2_SHIFT  (5U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG5_DONE2(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG5_DONE2_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG5_DONE2_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG6_DONE2_MASK   (0x40U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG6_DONE2_SHIFT  (6U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG6_DONE2(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG6_DONE2_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG6_DONE2_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG7_DONE2_MASK   (0x80U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG7_DONE2_SHIFT  (7U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG7_DONE2(x)     (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG7_DONE2_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG7_DONE2_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG0_ERR_MASK     (0x10000U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG0_ERR_SHIFT    (16U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG0_ERR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG0_ERR_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG0_ERR_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG1_ERR_MASK     (0x20000U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG1_ERR_SHIFT    (17U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG1_ERR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG1_ERR_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG1_ERR_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG2_ERR_MASK     (0x40000U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG2_ERR_SHIFT    (18U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG2_ERR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG2_ERR_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG2_ERR_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG3_ERR_MASK     (0x80000U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG3_ERR_SHIFT    (19U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG3_ERR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG3_ERR_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG3_ERR_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG4_ERR_MASK     (0x100000U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG4_ERR_SHIFT    (20U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG4_ERR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG4_ERR_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG4_ERR_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG5_ERR_MASK     (0x200000U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG5_ERR_SHIFT    (21U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG5_ERR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG5_ERR_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG5_ERR_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG6_ERR_MASK     (0x400000U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG6_ERR_SHIFT    (22U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG6_ERR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG6_ERR_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG6_ERR_MASK)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG7_ERR_MASK     (0x800000U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG7_ERR_SHIFT    (23U)
#define ADC_ETC_DONE2_ERR_IRQ_TRIG7_ERR(x)       (((uint32_t)(((uint32_t)(x)) << ADC_ETC_DONE2_ERR_IRQ_TRIG7_ERR_SHIFT)) & ADC_ETC_DONE2_ERR_IRQ_TRIG7_ERR_MASK)


/*! @} */

/*! @name TRIGn_CTRL - ETC_TRIG0 Control Register..ETC_TRIG7 Control Register */
/*! @{ */
#define ADC_ETC_TRIGn_CTRL_SW_TRIG_MASK          (0x1U)
#define ADC_ETC_TRIGn_CTRL_SW_TRIG_SHIFT         (0U)
#define ADC_ETC_TRIGn_CTRL_SW_TRIG(x)            (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CTRL_SW_TRIG_SHIFT)) & ADC_ETC_TRIGn_CTRL_SW_TRIG_MASK)
#define ADC_ETC_TRIGn_CTRL_TRIG_MODE_MASK        (0x10U)
#define ADC_ETC_TRIGn_CTRL_TRIG_MODE_SHIFT       (4U)
#define ADC_ETC_TRIGn_CTRL_TRIG_MODE(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CTRL_TRIG_MODE_SHIFT)) & ADC_ETC_TRIGn_CTRL_TRIG_MODE_MASK)
#define ADC_ETC_TRIGn_CTRL_TRIG_CHAIN_MASK       (0x700U)
#define ADC_ETC_TRIGn_CTRL_TRIG_CHAIN_SHIFT      (8U)
#define ADC_ETC_TRIGn_CTRL_TRIG_CHAIN(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CTRL_TRIG_CHAIN_SHIFT)) & ADC_ETC_TRIGn_CTRL_TRIG_CHAIN_MASK)
#define ADC_ETC_TRIGn_CTRL_TRIG_PRIORITY_MASK    (0x7000U)
#define ADC_ETC_TRIGn_CTRL_TRIG_PRIORITY_SHIFT   (12U)
#define ADC_ETC_TRIGn_CTRL_TRIG_PRIORITY(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CTRL_TRIG_PRIORITY_SHIFT)) & ADC_ETC_TRIGn_CTRL_TRIG_PRIORITY_MASK)
#define ADC_ETC_TRIGn_CTRL_SYNC_MODE_MASK        (0x10000U)
#define ADC_ETC_TRIGn_CTRL_SYNC_MODE_SHIFT       (16U)
#define ADC_ETC_TRIGn_CTRL_SYNC_MODE(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CTRL_SYNC_MODE_SHIFT)) & ADC_ETC_TRIGn_CTRL_SYNC_MODE_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_CTRL */
#define ADC_ETC_TRIGn_CTRL_COUNT                 (8U)

/*! @name TRIGn_COUNTER - ETC_TRIG0 Counter Register..ETC_TRIG7 Counter Register */
/*! @{ */
#define ADC_ETC_TRIGn_COUNTER_INIT_DELAY_MASK    (0xFFFFU)
#define ADC_ETC_TRIGn_COUNTER_INIT_DELAY_SHIFT   (0U)
#define ADC_ETC_TRIGn_COUNTER_INIT_DELAY(x)      (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_COUNTER_INIT_DELAY_SHIFT)) & ADC_ETC_TRIGn_COUNTER_INIT_DELAY_MASK)
#define ADC_ETC_TRIGn_COUNTER_SAMPLE_INTERVAL_MASK (0xFFFF0000U)
#define ADC_ETC_TRIGn_COUNTER_SAMPLE_INTERVAL_SHIFT (16U)
#define ADC_ETC_TRIGn_COUNTER_SAMPLE_INTERVAL(x) (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_COUNTER_SAMPLE_INTERVAL_SHIFT)) & ADC_ETC_TRIGn_COUNTER_SAMPLE_INTERVAL_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_COUNTER */
#define ADC_ETC_TRIGn_COUNTER_COUNT              (8U)

/*! @name TRIGn_CHAIN_1_0 - ETC_TRIG Chain 0/1 Register */
/*! @{ */
#define ADC_ETC_TRIGn_CHAIN_1_0_CSEL0_MASK       (0xFU)
#define ADC_ETC_TRIGn_CHAIN_1_0_CSEL0_SHIFT      (0U)
#define ADC_ETC_TRIGn_CHAIN_1_0_CSEL0(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_1_0_CSEL0_SHIFT)) & ADC_ETC_TRIGn_CHAIN_1_0_CSEL0_MASK)
#define ADC_ETC_TRIGn_CHAIN_1_0_HWTS0_MASK       (0xFF0U)
#define ADC_ETC_TRIGn_CHAIN_1_0_HWTS0_SHIFT      (4U)
#define ADC_ETC_TRIGn_CHAIN_1_0_HWTS0(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_1_0_HWTS0_SHIFT)) & ADC_ETC_TRIGn_CHAIN_1_0_HWTS0_MASK)
#define ADC_ETC_TRIGn_CHAIN_1_0_B2B0_MASK        (0x1000U)
#define ADC_ETC_TRIGn_CHAIN_1_0_B2B0_SHIFT       (12U)
#define ADC_ETC_TRIGn_CHAIN_1_0_B2B0(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_1_0_B2B0_SHIFT)) & ADC_ETC_TRIGn_CHAIN_1_0_B2B0_MASK)
#define ADC_ETC_TRIGn_CHAIN_1_0_IE0_MASK         (0x6000U)
#define ADC_ETC_TRIGn_CHAIN_1_0_IE0_SHIFT        (13U)
#define ADC_ETC_TRIGn_CHAIN_1_0_IE0(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_1_0_IE0_SHIFT)) & ADC_ETC_TRIGn_CHAIN_1_0_IE0_MASK)
#define ADC_ETC_TRIGn_CHAIN_1_0_CSEL1_MASK       (0xF0000U)
#define ADC_ETC_TRIGn_CHAIN_1_0_CSEL1_SHIFT      (16U)
#define ADC_ETC_TRIGn_CHAIN_1_0_CSEL1(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_1_0_CSEL1_SHIFT)) & ADC_ETC_TRIGn_CHAIN_1_0_CSEL1_MASK)
#define ADC_ETC_TRIGn_CHAIN_1_0_HWTS1_MASK       (0xFF00000U)
#define ADC_ETC_TRIGn_CHAIN_1_0_HWTS1_SHIFT      (20U)
#define ADC_ETC_TRIGn_CHAIN_1_0_HWTS1(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_1_0_HWTS1_SHIFT)) & ADC_ETC_TRIGn_CHAIN_1_0_HWTS1_MASK)
#define ADC_ETC_TRIGn_CHAIN_1_0_B2B1_MASK        (0x10000000U)
#define ADC_ETC_TRIGn_CHAIN_1_0_B2B1_SHIFT       (28U)
#define ADC_ETC_TRIGn_CHAIN_1_0_B2B1(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_1_0_B2B1_SHIFT)) & ADC_ETC_TRIGn_CHAIN_1_0_B2B1_MASK)
#define ADC_ETC_TRIGn_CHAIN_1_0_IE1_MASK         (0x60000000U)
#define ADC_ETC_TRIGn_CHAIN_1_0_IE1_SHIFT        (29U)
#define ADC_ETC_TRIGn_CHAIN_1_0_IE1(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_1_0_IE1_SHIFT)) & ADC_ETC_TRIGn_CHAIN_1_0_IE1_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_CHAIN_1_0 */
#define ADC_ETC_TRIGn_CHAIN_1_0_COUNT            (8U)

/*! @name TRIGn_CHAIN_3_2 - ETC_TRIG Chain 2/3 Register */
/*! @{ */
#define ADC_ETC_TRIGn_CHAIN_3_2_CSEL2_MASK       (0xFU)
#define ADC_ETC_TRIGn_CHAIN_3_2_CSEL2_SHIFT      (0U)
#define ADC_ETC_TRIGn_CHAIN_3_2_CSEL2(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_3_2_CSEL2_SHIFT)) & ADC_ETC_TRIGn_CHAIN_3_2_CSEL2_MASK)
#define ADC_ETC_TRIGn_CHAIN_3_2_HWTS2_MASK       (0xFF0U)
#define ADC_ETC_TRIGn_CHAIN_3_2_HWTS2_SHIFT      (4U)
#define ADC_ETC_TRIGn_CHAIN_3_2_HWTS2(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_3_2_HWTS2_SHIFT)) & ADC_ETC_TRIGn_CHAIN_3_2_HWTS2_MASK)
#define ADC_ETC_TRIGn_CHAIN_3_2_B2B2_MASK        (0x1000U)
#define ADC_ETC_TRIGn_CHAIN_3_2_B2B2_SHIFT       (12U)
#define ADC_ETC_TRIGn_CHAIN_3_2_B2B2(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_3_2_B2B2_SHIFT)) & ADC_ETC_TRIGn_CHAIN_3_2_B2B2_MASK)
#define ADC_ETC_TRIGn_CHAIN_3_2_IE2_MASK         (0x6000U)
#define ADC_ETC_TRIGn_CHAIN_3_2_IE2_SHIFT        (13U)
#define ADC_ETC_TRIGn_CHAIN_3_2_IE2(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_3_2_IE2_SHIFT)) & ADC_ETC_TRIGn_CHAIN_3_2_IE2_MASK)
#define ADC_ETC_TRIGn_CHAIN_3_2_CSEL3_MASK       (0xF0000U)
#define ADC_ETC_TRIGn_CHAIN_3_2_CSEL3_SHIFT      (16U)
#define ADC_ETC_TRIGn_CHAIN_3_2_CSEL3(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_3_2_CSEL3_SHIFT)) & ADC_ETC_TRIGn_CHAIN_3_2_CSEL3_MASK)
#define ADC_ETC_TRIGn_CHAIN_3_2_HWTS3_MASK       (0xFF00000U)
#define ADC_ETC_TRIGn_CHAIN_3_2_HWTS3_SHIFT      (20U)
#define ADC_ETC_TRIGn_CHAIN_3_2_HWTS3(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_3_2_HWTS3_SHIFT)) & ADC_ETC_TRIGn_CHAIN_3_2_HWTS3_MASK)
#define ADC_ETC_TRIGn_CHAIN_3_2_B2B3_MASK        (0x10000000U)
#define ADC_ETC_TRIGn_CHAIN_3_2_B2B3_SHIFT       (28U)
#define ADC_ETC_TRIGn_CHAIN_3_2_B2B3(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_3_2_B2B3_SHIFT)) & ADC_ETC_TRIGn_CHAIN_3_2_B2B3_MASK)
#define ADC_ETC_TRIGn_CHAIN_3_2_IE3_MASK         (0x60000000U)
#define ADC_ETC_TRIGn_CHAIN_3_2_IE3_SHIFT        (29U)
#define ADC_ETC_TRIGn_CHAIN_3_2_IE3(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_3_2_IE3_SHIFT)) & ADC_ETC_TRIGn_CHAIN_3_2_IE3_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_CHAIN_3_2 */
#define ADC_ETC_TRIGn_CHAIN_3_2_COUNT            (8U)

/*! @name TRIGn_CHAIN_5_4 - ETC_TRIG Chain 4/5 Register */
/*! @{ */
#define ADC_ETC_TRIGn_CHAIN_5_4_CSEL4_MASK       (0xFU)
#define ADC_ETC_TRIGn_CHAIN_5_4_CSEL4_SHIFT      (0U)
#define ADC_ETC_TRIGn_CHAIN_5_4_CSEL4(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_5_4_CSEL4_SHIFT)) & ADC_ETC_TRIGn_CHAIN_5_4_CSEL4_MASK)
#define ADC_ETC_TRIGn_CHAIN_5_4_HWTS4_MASK       (0xFF0U)
#define ADC_ETC_TRIGn_CHAIN_5_4_HWTS4_SHIFT      (4U)
#define ADC_ETC_TRIGn_CHAIN_5_4_HWTS4(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_5_4_HWTS4_SHIFT)) & ADC_ETC_TRIGn_CHAIN_5_4_HWTS4_MASK)
#define ADC_ETC_TRIGn_CHAIN_5_4_B2B4_MASK        (0x1000U)
#define ADC_ETC_TRIGn_CHAIN_5_4_B2B4_SHIFT       (12U)
#define ADC_ETC_TRIGn_CHAIN_5_4_B2B4(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_5_4_B2B4_SHIFT)) & ADC_ETC_TRIGn_CHAIN_5_4_B2B4_MASK)
#define ADC_ETC_TRIGn_CHAIN_5_4_IE4_MASK         (0x6000U)
#define ADC_ETC_TRIGn_CHAIN_5_4_IE4_SHIFT        (13U)
#define ADC_ETC_TRIGn_CHAIN_5_4_IE4(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_5_4_IE4_SHIFT)) & ADC_ETC_TRIGn_CHAIN_5_4_IE4_MASK)
#define ADC_ETC_TRIGn_CHAIN_5_4_CSEL5_MASK       (0xF0000U)
#define ADC_ETC_TRIGn_CHAIN_5_4_CSEL5_SHIFT      (16U)
#define ADC_ETC_TRIGn_CHAIN_5_4_CSEL5(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_5_4_CSEL5_SHIFT)) & ADC_ETC_TRIGn_CHAIN_5_4_CSEL5_MASK)
#define ADC_ETC_TRIGn_CHAIN_5_4_HWTS5_MASK       (0xFF00000U)
#define ADC_ETC_TRIGn_CHAIN_5_4_HWTS5_SHIFT      (20U)
#define ADC_ETC_TRIGn_CHAIN_5_4_HWTS5(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_5_4_HWTS5_SHIFT)) & ADC_ETC_TRIGn_CHAIN_5_4_HWTS5_MASK)
#define ADC_ETC_TRIGn_CHAIN_5_4_B2B5_MASK        (0x10000000U)
#define ADC_ETC_TRIGn_CHAIN_5_4_B2B5_SHIFT       (28U)
#define ADC_ETC_TRIGn_CHAIN_5_4_B2B5(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_5_4_B2B5_SHIFT)) & ADC_ETC_TRIGn_CHAIN_5_4_B2B5_MASK)
#define ADC_ETC_TRIGn_CHAIN_5_4_IE5_MASK         (0x60000000U)
#define ADC_ETC_TRIGn_CHAIN_5_4_IE5_SHIFT        (29U)
#define ADC_ETC_TRIGn_CHAIN_5_4_IE5(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_5_4_IE5_SHIFT)) & ADC_ETC_TRIGn_CHAIN_5_4_IE5_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_CHAIN_5_4 */
#define ADC_ETC_TRIGn_CHAIN_5_4_COUNT            (8U)

/*! @name TRIGn_CHAIN_7_6 - ETC_TRIG Chain 6/7 Register */
/*! @{ */
#define ADC_ETC_TRIGn_CHAIN_7_6_CSEL6_MASK       (0xFU)
#define ADC_ETC_TRIGn_CHAIN_7_6_CSEL6_SHIFT      (0U)
#define ADC_ETC_TRIGn_CHAIN_7_6_CSEL6(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_7_6_CSEL6_SHIFT)) & ADC_ETC_TRIGn_CHAIN_7_6_CSEL6_MASK)
#define ADC_ETC_TRIGn_CHAIN_7_6_HWTS6_MASK       (0xFF0U)
#define ADC_ETC_TRIGn_CHAIN_7_6_HWTS6_SHIFT      (4U)
#define ADC_ETC_TRIGn_CHAIN_7_6_HWTS6(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_7_6_HWTS6_SHIFT)) & ADC_ETC_TRIGn_CHAIN_7_6_HWTS6_MASK)
#define ADC_ETC_TRIGn_CHAIN_7_6_B2B6_MASK        (0x1000U)
#define ADC_ETC_TRIGn_CHAIN_7_6_B2B6_SHIFT       (12U)
#define ADC_ETC_TRIGn_CHAIN_7_6_B2B6(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_7_6_B2B6_SHIFT)) & ADC_ETC_TRIGn_CHAIN_7_6_B2B6_MASK)
#define ADC_ETC_TRIGn_CHAIN_7_6_IE6_MASK         (0x6000U)
#define ADC_ETC_TRIGn_CHAIN_7_6_IE6_SHIFT        (13U)
#define ADC_ETC_TRIGn_CHAIN_7_6_IE6(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_7_6_IE6_SHIFT)) & ADC_ETC_TRIGn_CHAIN_7_6_IE6_MASK)
#define ADC_ETC_TRIGn_CHAIN_7_6_CSEL7_MASK       (0xF0000U)
#define ADC_ETC_TRIGn_CHAIN_7_6_CSEL7_SHIFT      (16U)
#define ADC_ETC_TRIGn_CHAIN_7_6_CSEL7(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_7_6_CSEL7_SHIFT)) & ADC_ETC_TRIGn_CHAIN_7_6_CSEL7_MASK)
#define ADC_ETC_TRIGn_CHAIN_7_6_HWTS7_MASK       (0xFF00000U)
#define ADC_ETC_TRIGn_CHAIN_7_6_HWTS7_SHIFT      (20U)
#define ADC_ETC_TRIGn_CHAIN_7_6_HWTS7(x)         (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_7_6_HWTS7_SHIFT)) & ADC_ETC_TRIGn_CHAIN_7_6_HWTS7_MASK)
#define ADC_ETC_TRIGn_CHAIN_7_6_B2B7_MASK        (0x10000000U)
#define ADC_ETC_TRIGn_CHAIN_7_6_B2B7_SHIFT       (28U)
#define ADC_ETC_TRIGn_CHAIN_7_6_B2B7(x)          (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_7_6_B2B7_SHIFT)) & ADC_ETC_TRIGn_CHAIN_7_6_B2B7_MASK)
#define ADC_ETC_TRIGn_CHAIN_7_6_IE7_MASK         (0x60000000U)
#define ADC_ETC_TRIGn_CHAIN_7_6_IE7_SHIFT        (29U)
#define ADC_ETC_TRIGn_CHAIN_7_6_IE7(x)           (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_CHAIN_7_6_IE7_SHIFT)) & ADC_ETC_TRIGn_CHAIN_7_6_IE7_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_CHAIN_7_6 */
#define ADC_ETC_TRIGn_CHAIN_7_6_COUNT            (8U)

/*! @name TRIGn_RESULT_1_0 - ETC_TRIG Result Data 1/0 Register */
/*! @{ */
#define ADC_ETC_TRIGn_RESULT_1_0_DATA0_MASK      (0xFFFU)
#define ADC_ETC_TRIGn_RESULT_1_0_DATA0_SHIFT     (0U)
#define ADC_ETC_TRIGn_RESULT_1_0_DATA0(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_RESULT_1_0_DATA0_SHIFT)) & ADC_ETC_TRIGn_RESULT_1_0_DATA0_MASK)
#define ADC_ETC_TRIGn_RESULT_1_0_DATA1_MASK      (0xFFF0000U)
#define ADC_ETC_TRIGn_RESULT_1_0_DATA1_SHIFT     (16U)
#define ADC_ETC_TRIGn_RESULT_1_0_DATA1(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_RESULT_1_0_DATA1_SHIFT)) & ADC_ETC_TRIGn_RESULT_1_0_DATA1_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_RESULT_1_0 */
#define ADC_ETC_TRIGn_RESULT_1_0_COUNT           (8U)

/*! @name TRIGn_RESULT_3_2 - ETC_TRIG Result Data 3/2 Register */
/*! @{ */
#define ADC_ETC_TRIGn_RESULT_3_2_DATA2_MASK      (0xFFFU)
#define ADC_ETC_TRIGn_RESULT_3_2_DATA2_SHIFT     (0U)
#define ADC_ETC_TRIGn_RESULT_3_2_DATA2(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_RESULT_3_2_DATA2_SHIFT)) & ADC_ETC_TRIGn_RESULT_3_2_DATA2_MASK)
#define ADC_ETC_TRIGn_RESULT_3_2_DATA3_MASK      (0xFFF0000U)
#define ADC_ETC_TRIGn_RESULT_3_2_DATA3_SHIFT     (16U)
#define ADC_ETC_TRIGn_RESULT_3_2_DATA3(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_RESULT_3_2_DATA3_SHIFT)) & ADC_ETC_TRIGn_RESULT_3_2_DATA3_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_RESULT_3_2 */
#define ADC_ETC_TRIGn_RESULT_3_2_COUNT           (8U)

/*! @name TRIGn_RESULT_5_4 - ETC_TRIG Result Data 5/4 Register */
/*! @{ */
#define ADC_ETC_TRIGn_RESULT_5_4_DATA4_MASK      (0xFFFU)
#define ADC_ETC_TRIGn_RESULT_5_4_DATA4_SHIFT     (0U)
#define ADC_ETC_TRIGn_RESULT_5_4_DATA4(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_RESULT_5_4_DATA4_SHIFT)) & ADC_ETC_TRIGn_RESULT_5_4_DATA4_MASK)
#define ADC_ETC_TRIGn_RESULT_5_4_DATA5_MASK      (0xFFF0000U)
#define ADC_ETC_TRIGn_RESULT_5_4_DATA5_SHIFT     (16U)
#define ADC_ETC_TRIGn_RESULT_5_4_DATA5(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_RESULT_5_4_DATA5_SHIFT)) & ADC_ETC_TRIGn_RESULT_5_4_DATA5_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_RESULT_5_4 */
#define ADC_ETC_TRIGn_RESULT_5_4_COUNT           (8U)

/*! @name TRIGn_RESULT_7_6 - ETC_TRIG Result Data 7/6 Register */
/*! @{ */
#define ADC_ETC_TRIGn_RESULT_7_6_DATA6_MASK      (0xFFFU)
#define ADC_ETC_TRIGn_RESULT_7_6_DATA6_SHIFT     (0U)
#define ADC_ETC_TRIGn_RESULT_7_6_DATA6(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_RESULT_7_6_DATA6_SHIFT)) & ADC_ETC_TRIGn_RESULT_7_6_DATA6_MASK)
#define ADC_ETC_TRIGn_RESULT_7_6_DATA7_MASK      (0xFFF0000U)
#define ADC_ETC_TRIGn_RESULT_7_6_DATA7_SHIFT     (16U)
#define ADC_ETC_TRIGn_RESULT_7_6_DATA7(x)        (((uint32_t)(((uint32_t)(x)) << ADC_ETC_TRIGn_RESULT_7_6_DATA7_SHIFT)) & ADC_ETC_TRIGn_RESULT_7_6_DATA7_MASK)
/*! @} */

/* The count of ADC_ETC_TRIGn_RESULT_7_6 */
#define ADC_ETC_TRIGn_RESULT_7_6_COUNT           (8U)


/*!
 * @}
 */ /* end of group ADC_ETC_Register_Masks */


/* ADC_ETC - Peripheral instance base addresses */
/** Peripheral ADC_ETC base address */
#define ADC_ETC_BASE                             (0x403B0000u)
/** Peripheral ADC_ETC base pointer */
#define ADC_ETC                                  ((ADC_ETC_Type *)ADC_ETC_BASE)
/** Array initializer of ADC_ETC peripheral base addresses */
#define ADC_ETC_BASE_ADDRS                       { ADC_ETC_BASE }
/** Array initializer of ADC_ETC peripheral base pointers */
#define ADC_ETC_BASE_PTRS                        { ADC_ETC }
/** Interrupt vectors for the ADC_ETC peripheral type */
#define ADC_ETC_IRQS                             { { ADC_ETC_IRQ0_IRQn, ADC_ETC_IRQ1_IRQn, ADC_ETC_IRQ2_IRQn } }
#define ADC_ETC_FAULT_IRQS                       { ADC_ETC_ERROR_IRQ_IRQn }
/*!
 * @}
 */ /* end of group ADC_ETC_Peripheral_Access_Layer */
