/** Peripheral OCOTP base address */
#define OCOTP_ANA1                               (0x401F44E0u)

  __IO uint32_t ANA1;                              /**< Value of OTP Bank1 Word6 (Analog Info.), offset: 0x4E0 */

  
// 53.3: page 2986  TODO...
#define IMXRT_TEMPMON					(*(IMXRT_REGISTER32_t *)0x400F8180)
#define TEMPMON_TEMPSENSE0          (IMXRT_TEMPMON.offset000)	/**< Tempsensor Control Register 0, offset: 0x180 */
#define TEMPMON_TEMPSENSE0_SET		(IMXRT_TEMPMON.offset004)	/**< Tempsensor Control Register 0, offset: 0x184 */
#define TEMPMON_TEMPSENSE0_CLR		(IMXRT_TEMPMON.offset008)	/**< Tempsensor Control Register 0, offset: 0x188 */
#define TEMPMON_TEMPSENSE0_TOG		(IMXRT_TEMPMON.offset08c)	/**< Tempsensor Control Register 0, offset: 0x18C */
#define TEMPMON_TEMPSENSE1			(IMXRT_TEMPMON.offset090)	/**< Tempsensor Control Register 1, offset: 0x190 */
#define TEMPMON_TEMPSENSE1_SET		(IMXRT_TEMPMON.offset094)	/**< Tempsensor Control Register 1, offset: 0x194 */
#define TEMPMON_TEMPSENSE1_CLR				(IMXRT_TEMPMON.offset098)	/**< Tempsensor Control Register 1, offset: 0x198 */
#define TEMPMON_TEMPSENSE1_TOG				(IMXRT_TEMPMON.offset09C)	/**< Tempsensor Control Register 1, offset: 0x19C */
#define TEMPMON_TEMPSENSE2			(IMXRT_TEMPMON.offset100)	/**< Tempsensor Control Register 2, offset: 0x290 */
#define TEMPMON_TEMPSENSE2_SET		(IMXRT_TEMPMON.offset104)	/**< Tempsensor Control Register 2, offset: 0x294 */
#define TEMPMON_TEMPSENSE2_CLR		(IMXRT_TEMPMON.offset108)	/**< Tempsensor Control Register 2, offset: 0x298 */
#define TEMPMON_TEMPSENSE2_TOG		(IMXRT_TEMPMON.offset10C)	/**< Tempsensor Control Register 2, offset: 0x29C */

/* ----------------------------------------------------------------------------
   -- TEMPMON Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TEMPMON_Register_Masks TEMPMON Register Masks
 * @{
 */

/*! @name TEMPSENSE0 - Tempsensor Control Register 0 */
/*! @{ */
#define TEMPMON_TEMPSENSE0_POWER_DOWN_MASK       (0x1U)
#define TEMPMON_TEMPSENSE0_POWER_DOWN_SHIFT      (0U)
/*! POWER_DOWN
 *  0b0..Enable power to the temperature sensor.
 *  0b1..Power down the temperature sensor.
 */
#define TEMPMON_TEMPSENSE0_POWER_DOWN(x)         (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_POWER_DOWN_SHIFT)) & TEMPMON_TEMPSENSE0_POWER_DOWN_MASK)
#define TEMPMON_TEMPSENSE0_MEASURE_TEMP_MASK     (0x2U)
#define TEMPMON_TEMPSENSE0_MEASURE_TEMP_SHIFT    (1U)
/*! MEASURE_TEMP
 *  0b0..Do not start the measurement process.
 *  0b1..Start the measurement process.
 */
#define TEMPMON_TEMPSENSE0_MEASURE_TEMP(x)       (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_MEASURE_TEMP_SHIFT)) & TEMPMON_TEMPSENSE0_MEASURE_TEMP_MASK)
#define TEMPMON_TEMPSENSE0_FINISHED_MASK         (0x4U)
#define TEMPMON_TEMPSENSE0_FINISHED_SHIFT        (2U)
/*! FINISHED
 *  0b0..Last measurement is not ready yet.
 *  0b1..Last measurement is valid.
 */
#define TEMPMON_TEMPSENSE0_FINISHED(x)           (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_FINISHED_SHIFT)) & TEMPMON_TEMPSENSE0_FINISHED_MASK)
#define TEMPMON_TEMPSENSE0_TEMP_CNT_MASK         (0xFFF00U)
#define TEMPMON_TEMPSENSE0_TEMP_CNT_SHIFT        (8U)
#define TEMPMON_TEMPSENSE0_TEMP_CNT(x)           (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_TEMP_CNT_SHIFT)) & TEMPMON_TEMPSENSE0_TEMP_CNT_MASK)
#define TEMPMON_TEMPSENSE0_ALARM_VALUE_MASK      (0xFFF00000U)
#define TEMPMON_TEMPSENSE0_ALARM_VALUE_SHIFT     (20U)
#define TEMPMON_TEMPSENSE0_ALARM_VALUE(x)        (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE0_ALARM_VALUE_MASK)
/*! @} */

/*! @name TEMPSENSE0_SET - Tempsensor Control Register 0 */
/*! @{ */
#define TEMPMON_TEMPSENSE0_SET_POWER_DOWN_MASK   (0x1U)
#define TEMPMON_TEMPSENSE0_SET_POWER_DOWN_SHIFT  (0U)
/*! POWER_DOWN
 *  0b0..Enable power to the temperature sensor.
 *  0b1..Power down the temperature sensor.
 */
#define TEMPMON_TEMPSENSE0_SET_POWER_DOWN(x)     (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_SET_POWER_DOWN_SHIFT)) & TEMPMON_TEMPSENSE0_SET_POWER_DOWN_MASK)
#define TEMPMON_TEMPSENSE0_SET_MEASURE_TEMP_MASK (0x2U)
#define TEMPMON_TEMPSENSE0_SET_MEASURE_TEMP_SHIFT (1U)
/*! MEASURE_TEMP
 *  0b0..Do not start the measurement process.
 *  0b1..Start the measurement process.
 */
#define TEMPMON_TEMPSENSE0_SET_MEASURE_TEMP(x)   (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_SET_MEASURE_TEMP_SHIFT)) & TEMPMON_TEMPSENSE0_SET_MEASURE_TEMP_MASK)
#define TEMPMON_TEMPSENSE0_SET_FINISHED_MASK     (0x4U)
#define TEMPMON_TEMPSENSE0_SET_FINISHED_SHIFT    (2U)
/*! FINISHED
 *  0b0..Last measurement is not ready yet.
 *  0b1..Last measurement is valid.
 */
#define TEMPMON_TEMPSENSE0_SET_FINISHED(x)       (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_SET_FINISHED_SHIFT)) & TEMPMON_TEMPSENSE0_SET_FINISHED_MASK)
#define TEMPMON_TEMPSENSE0_SET_TEMP_CNT_MASK     (0xFFF00U)
#define TEMPMON_TEMPSENSE0_SET_TEMP_CNT_SHIFT    (8U)
#define TEMPMON_TEMPSENSE0_SET_TEMP_CNT(x)       (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_SET_TEMP_CNT_SHIFT)) & TEMPMON_TEMPSENSE0_SET_TEMP_CNT_MASK)
#define TEMPMON_TEMPSENSE0_SET_ALARM_VALUE_MASK  (0xFFF00000U)
#define TEMPMON_TEMPSENSE0_SET_ALARM_VALUE_SHIFT (20U)
#define TEMPMON_TEMPSENSE0_SET_ALARM_VALUE(x)    (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_SET_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE0_SET_ALARM_VALUE_MASK)
/*! @} */

/*! @name TEMPSENSE0_CLR - Tempsensor Control Register 0 */
/*! @{ */
#define TEMPMON_TEMPSENSE0_CLR_POWER_DOWN_MASK   (0x1U)
#define TEMPMON_TEMPSENSE0_CLR_POWER_DOWN_SHIFT  (0U)
/*! POWER_DOWN
 *  0b0..Enable power to the temperature sensor.
 *  0b1..Power down the temperature sensor.
 */
#define TEMPMON_TEMPSENSE0_CLR_POWER_DOWN(x)     (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_CLR_POWER_DOWN_SHIFT)) & TEMPMON_TEMPSENSE0_CLR_POWER_DOWN_MASK)
#define TEMPMON_TEMPSENSE0_CLR_MEASURE_TEMP_MASK (0x2U)
#define TEMPMON_TEMPSENSE0_CLR_MEASURE_TEMP_SHIFT (1U)
/*! MEASURE_TEMP
 *  0b0..Do not start the measurement process.
 *  0b1..Start the measurement process.
 */
#define TEMPMON_TEMPSENSE0_CLR_MEASURE_TEMP(x)   (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_CLR_MEASURE_TEMP_SHIFT)) & TEMPMON_TEMPSENSE0_CLR_MEASURE_TEMP_MASK)
#define TEMPMON_TEMPSENSE0_CLR_FINISHED_MASK     (0x4U)
#define TEMPMON_TEMPSENSE0_CLR_FINISHED_SHIFT    (2U)
/*! FINISHED
 *  0b0..Last measurement is not ready yet.
 *  0b1..Last measurement is valid.
 */
#define TEMPMON_TEMPSENSE0_CLR_FINISHED(x)       (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_CLR_FINISHED_SHIFT)) & TEMPMON_TEMPSENSE0_CLR_FINISHED_MASK)
#define TEMPMON_TEMPSENSE0_CLR_TEMP_CNT_MASK     (0xFFF00U)
#define TEMPMON_TEMPSENSE0_CLR_TEMP_CNT_SHIFT    (8U)
#define TEMPMON_TEMPSENSE0_CLR_TEMP_CNT(x)       (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_CLR_TEMP_CNT_SHIFT)) & TEMPMON_TEMPSENSE0_CLR_TEMP_CNT_MASK)
#define TEMPMON_TEMPSENSE0_CLR_ALARM_VALUE_MASK  (0xFFF00000U)
#define TEMPMON_TEMPSENSE0_CLR_ALARM_VALUE_SHIFT (20U)
#define TEMPMON_TEMPSENSE0_CLR_ALARM_VALUE(x)    (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_CLR_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE0_CLR_ALARM_VALUE_MASK)
/*! @} */

/*! @name TEMPSENSE0_TOG - Tempsensor Control Register 0 */
/*! @{ */
#define TEMPMON_TEMPSENSE0_TOG_POWER_DOWN_MASK   (0x1U)
#define TEMPMON_TEMPSENSE0_TOG_POWER_DOWN_SHIFT  (0U)
/*! POWER_DOWN
 *  0b0..Enable power to the temperature sensor.
 *  0b1..Power down the temperature sensor.
 */
#define TEMPMON_TEMPSENSE0_TOG_POWER_DOWN(x)     (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_TOG_POWER_DOWN_SHIFT)) & TEMPMON_TEMPSENSE0_TOG_POWER_DOWN_MASK)
#define TEMPMON_TEMPSENSE0_TOG_MEASURE_TEMP_MASK (0x2U)
#define TEMPMON_TEMPSENSE0_TOG_MEASURE_TEMP_SHIFT (1U)
/*! MEASURE_TEMP
 *  0b0..Do not start the measurement process.
 *  0b1..Start the measurement process.
 */
#define TEMPMON_TEMPSENSE0_TOG_MEASURE_TEMP(x)   (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_TOG_MEASURE_TEMP_SHIFT)) & TEMPMON_TEMPSENSE0_TOG_MEASURE_TEMP_MASK)
#define TEMPMON_TEMPSENSE0_TOG_FINISHED_MASK     (0x4U)
#define TEMPMON_TEMPSENSE0_TOG_FINISHED_SHIFT    (2U)
/*! FINISHED
 *  0b0..Last measurement is not ready yet.
 *  0b1..Last measurement is valid.
 */
#define TEMPMON_TEMPSENSE0_TOG_FINISHED(x)       (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_TOG_FINISHED_SHIFT)) & TEMPMON_TEMPSENSE0_TOG_FINISHED_MASK)
#define TEMPMON_TEMPSENSE0_TOG_TEMP_CNT_MASK     (0xFFF00U)
#define TEMPMON_TEMPSENSE0_TOG_TEMP_CNT_SHIFT    (8U)
#define TEMPMON_TEMPSENSE0_TOG_TEMP_CNT(x)       (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_TOG_TEMP_CNT_SHIFT)) & TEMPMON_TEMPSENSE0_TOG_TEMP_CNT_MASK)
#define TEMPMON_TEMPSENSE0_TOG_ALARM_VALUE_MASK  (0xFFF00000U)
#define TEMPMON_TEMPSENSE0_TOG_ALARM_VALUE_SHIFT (20U)
#define TEMPMON_TEMPSENSE0_TOG_ALARM_VALUE(x)    (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE0_TOG_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE0_TOG_ALARM_VALUE_MASK)
/*! @} */

/*! @name TEMPSENSE1 - Tempsensor Control Register 1 */
/*! @{ */
#define TEMPMON_TEMPSENSE1_MEASURE_FREQ_MASK     (0xFFFFU)
#define TEMPMON_TEMPSENSE1_MEASURE_FREQ_SHIFT    (0U)
#define TEMPMON_TEMPSENSE1_MEASURE_FREQ(x)       (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE1_MEASURE_FREQ_SHIFT)) & TEMPMON_TEMPSENSE1_MEASURE_FREQ_MASK)
/*! @} */

/*! @name TEMPSENSE1_SET - Tempsensor Control Register 1 */
/*! @{ */
#define TEMPMON_TEMPSENSE1_SET_MEASURE_FREQ_MASK (0xFFFFU)
#define TEMPMON_TEMPSENSE1_SET_MEASURE_FREQ_SHIFT (0U)
#define TEMPMON_TEMPSENSE1_SET_MEASURE_FREQ(x)   (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE1_SET_MEASURE_FREQ_SHIFT)) & TEMPMON_TEMPSENSE1_SET_MEASURE_FREQ_MASK)
/*! @} */

/*! @name TEMPSENSE1_CLR - Tempsensor Control Register 1 */
/*! @{ */
#define TEMPMON_TEMPSENSE1_CLR_MEASURE_FREQ_MASK (0xFFFFU)
#define TEMPMON_TEMPSENSE1_CLR_MEASURE_FREQ_SHIFT (0U)
#define TEMPMON_TEMPSENSE1_CLR_MEASURE_FREQ(x)   (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE1_CLR_MEASURE_FREQ_SHIFT)) & TEMPMON_TEMPSENSE1_CLR_MEASURE_FREQ_MASK)
/*! @} */

/*! @name TEMPSENSE1_TOG - Tempsensor Control Register 1 */
/*! @{ */
#define TEMPMON_TEMPSENSE1_TOG_MEASURE_FREQ_MASK (0xFFFFU)
#define TEMPMON_TEMPSENSE1_TOG_MEASURE_FREQ_SHIFT (0U)
#define TEMPMON_TEMPSENSE1_TOG_MEASURE_FREQ(x)   (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE1_TOG_MEASURE_FREQ_SHIFT)) & TEMPMON_TEMPSENSE1_TOG_MEASURE_FREQ_MASK)
/*! @} */

/*! @name TEMPSENSE2 - Tempsensor Control Register 2 */
/*! @{ */
#define TEMPMON_TEMPSENSE2_LOW_ALARM_VALUE_MASK  (0xFFFU)
#define TEMPMON_TEMPSENSE2_LOW_ALARM_VALUE_SHIFT (0U)
#define TEMPMON_TEMPSENSE2_LOW_ALARM_VALUE(x)    (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE2_LOW_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE2_LOW_ALARM_VALUE_MASK)
#define TEMPMON_TEMPSENSE2_PANIC_ALARM_VALUE_MASK (0xFFF0000U)
#define TEMPMON_TEMPSENSE2_PANIC_ALARM_VALUE_SHIFT (16U)
#define TEMPMON_TEMPSENSE2_PANIC_ALARM_VALUE(x)  (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE2_PANIC_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE2_PANIC_ALARM_VALUE_MASK)
/*! @} */

/*! @name TEMPSENSE2_SET - Tempsensor Control Register 2 */
/*! @{ */
#define TEMPMON_TEMPSENSE2_SET_LOW_ALARM_VALUE_MASK (0xFFFU)
#define TEMPMON_TEMPSENSE2_SET_LOW_ALARM_VALUE_SHIFT (0U)
#define TEMPMON_TEMPSENSE2_SET_LOW_ALARM_VALUE(x) (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE2_SET_LOW_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE2_SET_LOW_ALARM_VALUE_MASK)
#define TEMPMON_TEMPSENSE2_SET_PANIC_ALARM_VALUE_MASK (0xFFF0000U)
#define TEMPMON_TEMPSENSE2_SET_PANIC_ALARM_VALUE_SHIFT (16U)
#define TEMPMON_TEMPSENSE2_SET_PANIC_ALARM_VALUE(x) (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE2_SET_PANIC_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE2_SET_PANIC_ALARM_VALUE_MASK)
/*! @} */

/*! @name TEMPSENSE2_CLR - Tempsensor Control Register 2 */
/*! @{ */
#define TEMPMON_TEMPSENSE2_CLR_LOW_ALARM_VALUE_MASK (0xFFFU)
#define TEMPMON_TEMPSENSE2_CLR_LOW_ALARM_VALUE_SHIFT (0U)
#define TEMPMON_TEMPSENSE2_CLR_LOW_ALARM_VALUE(x) (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE2_CLR_LOW_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE2_CLR_LOW_ALARM_VALUE_MASK)
#define TEMPMON_TEMPSENSE2_CLR_PANIC_ALARM_VALUE_MASK (0xFFF0000U)
#define TEMPMON_TEMPSENSE2_CLR_PANIC_ALARM_VALUE_SHIFT (16U)
#define TEMPMON_TEMPSENSE2_CLR_PANIC_ALARM_VALUE(x) (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE2_CLR_PANIC_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE2_CLR_PANIC_ALARM_VALUE_MASK)
/*! @} */

/*! @name TEMPSENSE2_TOG - Tempsensor Control Register 2 */
/*! @{ */
#define TEMPMON_TEMPSENSE2_TOG_LOW_ALARM_VALUE_MASK (0xFFFU)
#define TEMPMON_TEMPSENSE2_TOG_LOW_ALARM_VALUE_SHIFT (0U)
#define TEMPMON_TEMPSENSE2_TOG_LOW_ALARM_VALUE(x) (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE2_TOG_LOW_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE2_TOG_LOW_ALARM_VALUE_MASK)
#define TEMPMON_TEMPSENSE2_TOG_PANIC_ALARM_VALUE_MASK (0xFFF0000U)
#define TEMPMON_TEMPSENSE2_TOG_PANIC_ALARM_VALUE_SHIFT (16U)
#define TEMPMON_TEMPSENSE2_TOG_PANIC_ALARM_VALUE(x) (((uint32_t)(((uint32_t)(x)) << TEMPMON_TEMPSENSE2_TOG_PANIC_ALARM_VALUE_SHIFT)) & TEMPMON_TEMPSENSE2_TOG_PANIC_ALARM_VALUE_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group TEMPMON_Register_Masks */


/* TEMPMON - Peripheral instance base addresses */
/** Peripheral TEMPMON base address */
#define TEMPMON_BASE                             (0x400D8000u)
/** Peripheral TEMPMON base pointer */
#define TEMPMON                                  ((TEMPMON_Type *)TEMPMON_BASE)
/** Array initializer of TEMPMON peripheral base addresses */
#define TEMPMON_BASE_ADDRS                       { TEMPMON_BASE }
/** Array initializer of TEMPMON peripheral base pointers */
#define TEMPMON_BASE_PTRS                        { TEMPMON }

/*!
 * @}
 */ /* end of group TEMPMON_Peripheral_Access_Layer */

