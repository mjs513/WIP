#include "imxrt_flexcan.h"
#include "FlexCAN_T4.h"
#include "clock_functions.h"

//FlexCAN_T4 Can2 = FlexCAN_T4(FLEXCan2_BASE);

void setup() {
  Serial.begin(115200);
  delay(4000);
  
  /*set osc clock crystal freq */
  CLOCK_SetXtalFreq(24000000UL);

  Can2.setRX();
  Can2.setTX();
  //  Can2.setMaxMB(10);
  Can2.setBaudRate(1000000);
  Can2.enableFIFO(1);
  Can2.enableFIFOInterrupt();
  Can2.setMB(MB8, RX, STD);
  Can2.enableMBInterrupt(MB8);
  Can2.onReceive(canSniff);
  //  Can0.enableMBInterrupt(MB0);
  // MCR POR == 1501560847

  //  FLEXCANb_MCR(_baseAddress) |= (FLEXCANb_MCR(_baseAddress) & ~0x3F) | 0x3F;

  //  FLEXCANb_MCR(_baseAddress) &= ~FLEXCAN_MCR_HALT;
  //  while (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FRZ_ACK);

  
  //printClocks();
}

void loop() {
  Can2.events();
  CAN_message_t msg;
  Serial.print("CANREAD?: "); Serial.println(Can2.read(msg));
  //FLEXCANb_MCR(FLEXCan2_BASE) = FLEXCANb_MCR(FLEXCan2_BASE) & ~0x40000000;  CAN_message_t msg;
  msg.len = 8;
  for ( uint8_t i = 0; i < 8; i++ ) msg.buf[i] = i + 1;
  msg.id = 0x40;
  Can2.write(msg);

  delay(1000);
  Serial.print("MCR: "); Serial.println( FLEXCAN2_MCR &= ~0x40000000  );
  Serial.print("RXMGMASK: "); Serial.println( FLEXCANb_RXMGMASK(FLEXCAN2_BASE)  );
  Serial.print("RXFGMASK: "); Serial.println( FLEXCANb_RXFGMASK(FLEXCAN2_BASE)  );
  Serial.print("FLEXCANb_IFLAG1: "); Serial.println( FLEXCANb_IFLAG1(FLEXCAN2_BASE)  );
  Serial.print("FLEXCANb_IFLAG2: "); Serial.println( FLEXCANb_IFLAG2(FLEXCAN2_BASE)  );
  Serial.print("FLEXCANb_IMASK1: "); Serial.println( FLEXCANb_IMASK1(FLEXCAN2_BASE)  );
  Serial.print("???: "); Serial.println( IOMUXC_FLEXCAN2_RX_SELECT_INPUT  );

  Serial.print("FLEXCANb_MAXMB_SIZE(_baseAddress): "); Serial.println( FLEXCANb_MAXMB_SIZE(FLEXCAN2_BASE)  );
  Serial.print("MBOFFSET: "); Serial.println( Can2.mailboxOffset()  );
  Can2.mailboxStatus();
  //  Can2.currentMasks();

  Serial.print( CORE_PIN0_CONFIG,BIN ); Serial.print(" : "); Serial.println(CORE_PIN1_CONFIG, BIN);
  //  wdt.loop(); // feed the dog, uncomment to test callback
}


void canSniff(const CAN_message_t &msg) { // global callback
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" REMOTE: "); Serial.print(msg.flags.remote);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}


void printClocks(){
  Serial.printf("System Clock: %d\r\n", CLOCK_GetAhbFreq());
  Serial.printf("IPG Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_IpgClk));
  Serial.printf("Semc Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
  Serial.printf("RTC Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_RtcClk));
  Serial.printf("USB1pll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
  Serial.printf("Peripheral Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_PerClk));
  Serial.printf("Osc Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_OscClk));
  Serial.printf("Arm Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_ArmPllClk));
  Serial.printf("Usb1PllPfd0 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
  Serial.printf("Usb1PllPfd1 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
  Serial.printf("Usb1PllPfd2 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
  Serial.printf("Usb1PllPfd3 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
  Serial.printf("Usb2Pll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_Usb2PllClk));
  Serial.printf("SysPll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
  Serial.printf("SysPllPfd0 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
  Serial.printf("SysPllPfd1 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
  Serial.printf("SysPllPfd2 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
  Serial.printf("SysPllPfd3 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
  Serial.printf("EnetPll0 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_EnetPll0Clk));
  Serial.printf("EnetPll1 Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_EnetPll1Clk));
  Serial.printf("AudioPll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_AudioPllClk));
  Serial.printf("VideoPll Clock: %d\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
}
