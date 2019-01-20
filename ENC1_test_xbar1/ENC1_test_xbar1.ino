#include "Enc_Layer.h"
#include "defines.h"

enc_config_t mEncConfigStruct;
uint32_t mCurPosValue;

void setup()
{
  while(!Serial && millis() < 4000);
  delay(2000);

  CCM_CCGR2 |= CCM_CCGR2_XBAR1(CCM_CCGR_ON);   //turn clock on for xbara1

  CORE_PIN6_CONFIG = 0x11;  //Select mux mode: ALT3 mux port
  CORE_PIN7_CONFIG = 0x11;  //0x03=Input Path is determined by functionality, 
                            //0x13 for 1 enabled
  
  CORE_PIN6_PADCONFIG = 0x10B0;  //pin pad configuration
  CORE_PIN7_PADCONFIG = 0x10B0;
  
  //set as input
  IOMUXC_XBAR1_IN15_SELECT_INPUT = 0x01;  //set both pins as input
  IOMUXC_XBAR1_IN14_SELECT_INPUT = 0x01;

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
  
  xbar_connect(15, 66);
  xbar_connect(14, 67);
  //========================================================================
  //Phase A => pin3
  //Phase B => pin2
    /* Initialize the ENC module. */
    ENC_GetDefaultConfig(&mEncConfigStruct);
    ENC_Init(&mEncConfigStruct);
    ENC_DoSoftwareLoadInitialPositionValue(); /* Update the position counter with initial value. */
}

void loop(){
  
  /* This read operation would capture all the position counter to responding hold registers. */
  mCurPosValue = ENC_GetPositionValue();

  /* Read the position values. */
  Serial.printf("Current position value: %ld\r\n", mCurPosValue);
  Serial.printf("Position differential value: %d\r\n", (int16_t)ENC_GetHoldPositionDifferenceValue());
  Serial.printf("Position revolution value: %d\r\n", ENC_GetHoldRevolutionValue());
  Serial.println();
  
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
