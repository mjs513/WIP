#include "Enc_Layer.h"
#include "defines.h"

#define PHASEA 1
#define PHASEB 2

#define CORE_XIO_PIN0 IOMUXC_XBAR1_IN17_SELECT_INPUT
#define CORE_XIO_PIN1 IOMUXC_XBAR1_IN16_SELECT_INPUT
#define CORE_XIO_PIN2 IOMUXC_XBAR1_IN06_SELECT_INPUT
#define CORE_XIO_PIN3 IOMUXC_XBAR1_IN07_SELECT_INPUT
#define CORE_XIO_PIN4 IOMUXC_XBAR1_IN08_SELECT_INPUT
#define CORE_XIO_PIN5 IOMUXC_XBAR1_IN09_SELECT_INPUT
#define CORE_XIO_PIN7 IOMUXC_XBAR1_IN14_SELECT_INPUT    
#define CORE_XIO_PIN6 IOMUXC_XBAR1_IN15_SELECT_INPUT

struct xio_pin_input_config_table_struct {
    volatile uint32_t *reg;
};

const struct xio_pin_input_config_table_struct xio_pin_to_info_PGM[] = {
  {&CORE_XIO_PIN0}, {&CORE_XIO_PIN1},
  {&CORE_XIO_PIN2}, {&CORE_XIO_PIN3},
  {&CORE_XIO_PIN4}, {&CORE_XIO_PIN5},
  {&CORE_XIO_PIN6}, {&CORE_XIO_PIN7}
};


enc_config_t mEncConfigStruct;
uint32_t mCurPosValue;

void setup()
{
  while(!Serial && millis() < 4000);
  delay(2000);

  CCM_CCGR2 |= CCM_CCGR2_XBAR1(CCM_CCGR_ON);   //turn clock on for xbara1

  enc_xbara_mapping(6, PHASEA);
  enc_xbara_mapping(7, PHASEB);

  //==========================================================================
  /* XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc1PhaseAInput);
   * XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc1PhaseBInput);
   * XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc1Index);
   * These are the SDK settings
   * kXBARA1_OutputEnc1PhaseAInput   = 66|0x100U,   // XBARA1_OUT66 output assigned to ENC1_PHASE_A_INPUT
   * kXBARA1_OutputEnc1PhaseBInput   = 67|0x100U,   // XBARA1_OUT67 output assigned to ENC1_PHASE_B_INPUT
   * kXBARA1_OutputEnc1Index         = 68|0x100U,   // XBARA1_OUT68 output assigned to ENC1_INDEX 
   * kXBARA1_OutputEnc1Home          = 69|0x100U,   // XBARA1_OUT69 output assigned to ENC1_HOME
   * kXBARA1_OutputEnc1Trigger       = 70|0x100U,   // XBARA1_OUT70 output assigned to ENC1_TRIGGER 
   *
   * kXBARA1_InputIomuxXbarInout06   = 6|0x100U,    // IOMUX_XBAR_INOUT06 output assigned to XBARA1_IN6 input.
   * kXBARA1_InputIomuxXbarInout07   = 7|0x100U,    // IOMUX_XBAR_INOUT07 output assigned to XBARA1_IN7 input.
   * kXBARA1_InputIomuxXbarInout08   = 8|0x100U,    // IOMUX_XBAR_INOUT08 output assigned to XBARA1_IN8 input.
   */

  Serial.print("IOMUXC_GPR_GPR6: "); Serial.println(IOMUXC_GPR_GPR6, BIN);

  delay(2000);
  //========================================================================
  //Phase A => pin3
  //Phase B => pin2
    /* Initialize the ENC module. */
    ENC_GetDefaultConfig(&mEncConfigStruct);
    ENC_Init(&mEncConfigStruct);
    ENC_DoSoftwareLoadInitialPositionValue(); /* Update the position counter with initial value. */
}

uint32_t old_position = 0;

void loop(){
  
  /* This read operation would capture all the position counter to responding hold registers. */
  mCurPosValue = ENC_GetPositionValue();

  if(mCurPosValue != old_position){
    /* Read the position values. */
    Serial.printf("Current position value: %ld\r\n", mCurPosValue);
    Serial.printf("Position differential value: %d\r\n", (int16_t)ENC_GetHoldPositionDifferenceValue());
    Serial.printf("Position revolution value: %d\r\n", ENC_GetHoldRevolutionValue());
    Serial.println();
  }

  old_position = mCurPosValue;
}


void enc_xbara_mapping(uint8_t pin, uint8_t PHASE){ 

  const struct digital_pin_bitband_and_config_table_struct *p;
  const struct xio_pin_input_config_table_struct *x;
  uint32_t pinmode, mask;
  
  uint8_t xbara1_mux[] = {1, 0, 0, 0, 0, 0, 1, 1};
  uint16_t xbara1_IO[] = {17, 16, 6, 7, 8, 9, 15, 14};
  
  if (pin >= 8) return;         //first 8 pins are xbara1 io pins
  p = digital_pin_to_info_PGM + pin;
  //mux is ctrl config for pin
  //pad is pad config
  //pinmode = *(p->reg + 1);
  
  //Pin ctrl configuration for encoder/xbara1
  if(pin == 4 || pin == 5) {
    *(p->mux) = 0x03;
  } else {
    *(p->mux) = 0x01;
  }
  
  //Pad configuration for encoder/xbara1
  *(p->pad) = 0x10B0;
  
  x = xio_pin_to_info_PGM + pin;
  *(x->reg) = xbara1_mux[pin];

  //XBARA1 Connection to encoder
  if(PHASE == 1) xbar_connect(xbara1_IO[pin], 66);
  if(PHASE == 2) xbar_connect(xbara1_IO[pin], 67);

}
void xbar_connect(unsigned int input, unsigned int output)
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
