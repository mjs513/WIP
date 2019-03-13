#include "FlexCAN_T4.h"
#include "imxrt_flexcan.h"
#include "Arduino.h"

FlexCAN_T4 Can1 = FlexCAN_T4(FLEXCAN2_BASE);
FlexCAN_T4 Can2 = FlexCAN_T4(FLEXCAN2_BASE);


_MB_ptr FlexCAN_T4::_CAN1MBhandlers[64] = { nullptr };
_MB_ptr FlexCAN_T4::_CAN1GLOBALhandler = nullptr;
_MB_ptr FlexCAN_T4::_CAN2MBhandlers[64] = { nullptr };
_MB_ptr FlexCAN_T4::_CAN2GLOBALhandler = nullptr;

Circular_Buffer<uint8_t, FLEXCAN_RX_BUFFER_SIZE, sizeof(CAN_message_t)> FlexCAN_T4::flexcanRxBuffer;
Circular_Buffer<uint8_t, FLEXCAN_TX1_BUFFER_SIZE, sizeof(CAN_message_t)> FlexCAN_T4::flexcanTx1Buffer;
Circular_Buffer<uint8_t, FLEXCAN_TX2_BUFFER_SIZE, sizeof(CAN_message_t)> FlexCAN_T4::flexcanTx2Buffer;
bool FlexCAN_T4::can_events = 0;

void can1_message_isr (void) {
  Can1.FlexCAN_T4_message_ISR();
}
void can2_message_isr (void) {
  Can2.FlexCAN_T4_message_ISR();
}

FlexCAN_T4::FlexCAN_T4(uint32_t base) {
  _baseAddress = base;
  //clock settings for flexcan
  CCM_CSCMR2 = (CCM_CSCMR2 & 0xFFFFFC03) | CCM_CSCMR2_CAN_CLK_SEL(2) |CCM_CSCMR2_CAN_CLK_PODF(3);
  Serial.print("CCM_CSCMR2: ");Serial.println(CCM_CSCMR2, BIN);
  
  if ( _baseAddress == FLEXCAN1_BASE ) {
  CCM_CCGR0 |= CCM_CCGR0_CAN1(CCM_CCGR_ON); 
  CCM_CCGR0 |= CCM_CCGR0_CAN1_SERIAL(CCM_CCGR_ON);
  Serial.print("CCM_CCGR0: ");Serial.println(CCM_CCGR0, BIN);
    
    _VectorsRam[16 + IRQ_CAN1] = can1_message_isr;
    NVIC_IRQ = IRQ_CAN1;
    //CCM_CCGR0 |= (0xF << 18);
  }
  else if ( _baseAddress == FLEXCAN2_BASE ) {

  //CCM_CCGR0 |= (0xF << 18);
  CCM_CCGR0 |= CCM_CCGR0_CAN2(CCM_CCGR_ON); 
  CCM_CCGR0 |= CCM_CCGR0_CAN2_SERIAL(CCM_CCGR_ON);
  Serial.print("CCM_CCGR0: ");Serial.println(CCM_CCGR0, BIN);

    //-------------------------------------//
    //rx configure pin for flexcan
    CORE_PIN0_CONFIG = 0x10;        // alt 0 is can2
    IOMUXC_FLEXCAN2_RX_SELECT_INPUT = 0x01;
    Serial.print("IOMUXC_FLEXCAN2_RX_SELECT_INPUT: ");Serial.println(IOMUXC_FLEXCAN2_RX_SELECT_INPUT, BIN);
  
    //pin1, can2TX B0_02
    CORE_PIN1_CONFIG = 0x10;
    uint32_t fastio =  IOMUXC_PAD_SRE | IOMUXC_PAD_DSE(3) | IOMUXC_PAD_SPEED(3);
    CORE_PIN0_PADCONFIG = 0xB0;
    CORE_PIN1_PADCONFIG = 0xB0;  // 0x1A081, 0x10B0

    Serial.print("Core PINS: "); Serial.print(CORE_PIN0_CONFIG, BIN);
    Serial.print(", "); Serial.println(CORE_PIN1_CONFIG, BIN);
    
    _VectorsRam[16 + IRQ_CAN2] = can2_message_isr;
    NVIC_IRQ = IRQ_CAN2;
  }

    FLEXCAN_Enable(true);
    FLEXCAN_Reset();

  while (!(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FRZ_ACK));
  FLEXCANb_MCR(_baseAddress) |= FLEXCAN_MCR_SRX_DIS; /* Disable self-reception */

  disableFIFO();
  disableFIFOInterrupt();

  FLEXCANb_MCR(_baseAddress) |= FLEXCAN_MCR_IRMQ; // individual mailbox masking
  FLEXCANb_MCR(_baseAddress) |= FLEXCAN_MCR_AEN; // TX ABORT FEATURE
  FLEXCANb_MCR(_baseAddress) |= FLEXCAN_MCR_LPRIO_EN; // TX PRIORITY FEATURE
  FLEXCANb_MCR(_baseAddress) |= FLEXCAN_MCR_LPRIO_EN; // TX PRIORITY FEATURE
  FLEXCANb_MCR(_baseAddress) &= ~FLEXCAN_MCR_SUPV;
  FLEXCANb_CTRL2(_baseAddress) |= FLEXCAN_CTRL2_RRS | // store remote frames
                                  FLEXCAN_CTRL2_MRP; // mailbox > FIFO priority.

  FLEXCANb_MCR(_baseAddress) &= ~FLEXCAN_MCR_HALT; // start the CAN //
  Serial.print("FLEXCANb_MCR0: ");Serial.println(FLEXCANb_MCR(_baseAddress), BIN);
  
  while (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FRZ_ACK);
  while (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_NOT_RDY); // wait until ready /
  
  Serial.print("FLEXCANb_MCR1: ");Serial.println(FLEXCANb_MCR(_baseAddress), BIN);

  //setRX(); setTX();

  //NVIC_SET_PRIORITY(NVIC_IRQ, 1); // set interrupt priority /
  NVIC_ENABLE_IRQ(NVIC_IRQ); // enable message interrupt /

  FLEXCANb_MCR(_baseAddress) &= ~FLEXCAN_CTRL_BOFF_REC;
}

void FlexCAN_T4::FLEXCAN_Enable(bool enable)
{
    if (enable)
    {
        FLEXCAN2_MCR &= ~0x80000000U;
        Serial.print("FLEXCAN2_MCR &= ~0x80000000U\r\n");
        Serial.println(FLEXCAN2_MCR, BIN);
        /* Wait FlexCAN exit from low-power mode. */
        while (FLEXCAN2_MCR & 0x100000U)
        {
        }
    }
    else
    {
        FLEXCAN2_MCR |= 0x80000000U;
        /* Wait FlexCAN enter low-power mode. */
        while (!(FLEXCAN2_MCR & 0x100000U))
        {
        }
    }
}



void FlexCAN_T4::FLEXCAN_Reset()
{
    /* The module must should be first exit from low power
     * mode, and then soft reset can be applied.
     */
    //Serial.println("IN RESET");
    FLEXCAN2_MCR &= 0x80000000U;
    //Serial.print("FLEXCAN2_MCR &= 0x80000000U:  ");
    //Serial.println(FLEXCAN2_MCR, BIN);  //no change

    uint8_t i;

    /* Wait until FlexCAN exit from any Low Power Mode. */
    while ((FLEXCAN2_MCR & 0x100000U))
    {
      //Serial.println("NOT EXITING");
    }

    /* Assert Soft Reset Signal. */
    //Serial.println("Assert Soft Reset Signal  ");
    FLEXCAN2_MCR |= 0x2000000U;
    //Serial.print("FLEXCAN2_MCR |= 0x2000000U: ");
    //Serial.println(FLEXCAN2_MCR, BIN);

    /* Wait until FlexCAN reset completes. */
    while (FLEXCAN2_MCR & 0x2000000U)
    {
      //Serial.print("NOT reset completes:  ");
      //Serial.println(FLEXCAN2_MCR, BIN);
    }

    //Serial.println("RESETING REGISTERS");
    /* Reset MCR rigister. */
    FLEXCAN2_MCR |= 0x200000U | CAN_MCR_MAXMB(16);


    //Serial.println("Reset CTRL1 and CTRL2 rigister");
    /* Reset CTRL1 and CTRL2 rigister. */
    FLEXCAN2_CTRL1 = 0x80U;
    FLEXCAN2_CTRL2 = CAN_CTRL2_TASD(0x16) | 0x20000U | 0x10000U;

    /* Clean all individual Rx Mask of Message Buffers. */
    //for (i = 0; i < FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBERn(base); i++)
    //{
    //    FLEXCAN2_RXIMR[i] = 0x3FFFFFFF;
    //}

    /* Clean Global Mask of Message Buffers. */
    FLEXCAN2_RXMGMASK = 0x3FFFFFFF;
    /* Clean Global Mask of Message Buffer 14. */
    FLEXCAN2_RX14MASK = 0x3FFFFFFF;
    /* Clean Global Mask of Message Buffer 15. */
    FLEXCAN2_RX15MASK = 0x3FFFFFFF;
    /* Clean Global Mask of Rx FIFO. */
    FLEXCAN2_RXFGMASK = 0x3FFFFFFF;

    /* Clean all Message Buffer CS fields. */
    //for (i = 0; i < FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBERn(base); i++)
    //{
    //    base->MB[i].CS = 0x0;
    //}
  
}












void sendMSGtoIndividualMBCallback(const FLEXCAN_MAILBOX &mb_num, const CAN_message_t &msg) { /* this is global for ISR use */
  if ( msg.bus == 1 && FlexCAN_T4::_CAN1MBhandlers[mb_num] ) FlexCAN_T4::_CAN1MBhandlers[mb_num](msg);
  if ( msg.bus == 2 && FlexCAN_T4::_CAN2MBhandlers[mb_num] ) FlexCAN_T4::_CAN2MBhandlers[mb_num](msg);
}

void FlexCAN_T4::FlexCAN_T4_message_ISR(void) {
  CAN_message_t msg; // setup a temporary storage buffer
  uint64_t status = FLEXCANb_IFLAG1(_baseAddress) << 16 | FLEXCANb_IFLAG2(_baseAddress);
  uint64_t mb_ints_en = FLEXCANb_IMASK1(_baseAddress) << 16 | FLEXCANb_IMASK2(_baseAddress);

  if ( (FLEXCANb_IMASK1(_baseAddress) & FLEXCAN_IMASK1_BUF5M) && (status & FLEXCAN_IFLAG1_BUF5I) && (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN) ) { /* FIFO is enabled, capture frames if triggered */
    while ( FLEXCANb_IFLAG1(_baseAddress) & FLEXCAN_IFLAG1_BUF5I ) {
      if ( FLEXCANb_IMASK1(_baseAddress) & FLEXCAN_IMASK1_BUF5M ) {
        msg.len = FLEXCAN_get_length(FLEXCANb_MBn_CS(_baseAddress, 0));
        msg.flags.extended = (FLEXCANb_MBn_CS(_baseAddress, 0) & FLEXCAN_MB_CS_IDE) ? 1 : 0;
        msg.flags.remote = (FLEXCANb_MBn_CS(_baseAddress, 0) & FLEXCAN_MB_CS_RTR) ? 1 : 0;
        msg.timestamp = FLEXCAN_get_timestamp (FLEXCANb_MBn_CS(_baseAddress, 0));
        msg.id = (FLEXCANb_MBn_ID(_baseAddress, 0) & FLEXCAN_MB_ID_EXT_MASK);
        if (!msg.flags.extended) msg.id >>= FLEXCAN_MB_ID_STD_BIT_NO;
        uint32_t dataIn = FLEXCANb_MBn_WORD0(_baseAddress, 0);
        msg.buf[0] = dataIn >> 24; msg.buf[1] = dataIn >> 16; msg.buf[2] = dataIn >> 8; msg.buf[3] = dataIn;
        dataIn = FLEXCANb_MBn_WORD1(_baseAddress, 0);
        msg.buf[4] = dataIn >> 24; msg.buf[5] = dataIn >> 16; msg.buf[6] = dataIn >> 8; msg.buf[7] = dataIn;
        if ( _baseAddress == FLEXCAN1_BASE ) msg.bus = 1;
        else if ( _baseAddress == FLEXCAN2_BASE ) msg.bus = 2;
        FLEXCANb_IFLAG1(_baseAddress) = FLEXCAN_IFLAG1_BUF5I; /* clear FIFO bit only! */

        bool enhance_filtering_success = 0;

        for ( uint8_t i = 0; i < mailboxOffset(); i++ ) {
          if ( filter_enhancement[i][0] && filter_set[i] ) { /* if enhancement is active and set */
            if ( !filter_enhancement[i][1] ) { /* if it's multi ID */
              for ( uint8_t j = 0; j < 5; j++ ) {
                if ( msg.id == filter_enhancement_config[i][j] ) {
                  enhance_filtering_success = 1;
                  break;
                }
              }
            }
            else { /* if it's range based */

              if ( (msg.id >= filter_enhancement_config[i][0]) &&
                   (msg.id <= filter_enhancement_config[i][1]) ) enhance_filtering_success = 1;

              if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 1 ) { 
                if ( (msg.id >= filter_enhancement_config[i][2]) &&
                     (msg.id <= filter_enhancement_config[i][3]) ) enhance_filtering_success = 1;
              }

            }
          }
        }

        /* if enhancement is on and a match is found, OR, enhancement is disabled, run handler or queue as normal */

        if ( ( enhance_filtering_success && filter_enhancement[0][0] ) || !filter_enhancement[0][0] ) { /* if enhanced AND success, OR not enhanced */
          if ( !can_events ) {
            if ( msg.bus == 1 && FlexCAN_T4::_CAN1GLOBALhandler ) FlexCAN_T4::_CAN1GLOBALhandler(msg);
            if ( msg.bus == 2 && FlexCAN_T4::_CAN2GLOBALhandler ) FlexCAN_T4::_CAN2GLOBALhandler(msg);
            sendMSGtoIndividualMBCallback((FLEXCAN_MAILBOX)0, msg); /* send frames direct to callback (unbuffered) */
          }
          else {
            struct2queue(msg); /* store frame in queue ( buffered ) */
          }
          ext_output(msg);
        }

        /* callback, queue, or neither, we check other filters for cross-matches */
        if ( msg_distribution && can_events ) packet_distribution(msg);

      }
      FLEXCANb_IFLAG1(_baseAddress) = FLEXCAN_IFLAG1_BUF5I; /* clear FIFO bit only! */
    }
    if ( status & FLEXCAN_IFLAG1_BUF6I ) {
      FLEXCANb_IFLAG1(_baseAddress) = FLEXCAN_IFLAG1_BUF6I;
      status &= ~FLEXCAN_IFLAG1_BUF6I; /* remove bit from initial flag lookup so it's not set at end when another frame is captured */
    }
    if ( status & FLEXCAN_IFLAG1_BUF7I ) {
      FLEXCANb_IFLAG1(_baseAddress) = FLEXCAN_IFLAG1_BUF7I;
      status &= ~FLEXCAN_IFLAG1_BUF7I; /* remove bit from initial flag lookup so it's not set at end when another frame is captured */
    }
    status &= ~FLEXCAN_IFLAG1_BUF5I; /* remove bit from initial flag lookup so it's not set at end when another frame is captured */
  }



  /* mailbox handling routine */

  for (uint8_t i = mailboxOffset(); i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++) {

    if (!(mb_ints_en & ( 1ULL << i ))) continue; // skip mailboxes that don't have interrupts enabled
    if (!(status & (1ULL << i))) continue; // skip mailboxes that haven't triggered an interrupt
    uint32_t code = FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, i)); // Reading Control Status atomically locks mailbox.
    switch ( code ) {
      case FLEXCAN_MB_CODE_RX_FULL:           // rx full, Copy the frame to RX buffer
      case FLEXCAN_MB_CODE_RX_OVERRUN: {      // rx overrun. Incomming frame overwrote existing frame.
          if ( FLEXCAN_MB_CODE_RX_OVERRUN == code ) msg.flags.overrun = 1;
          msg.len = FLEXCAN_get_length(FLEXCANb_MBn_CS(_baseAddress, i));
          msg.flags.extended = (FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_IDE) ? 1 : 0;
          msg.flags.remote = (FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_RTR) ? 1 : 0;
          msg.timestamp = FLEXCAN_get_timestamp (FLEXCANb_MBn_CS(_baseAddress, i));
          msg.id = (FLEXCANb_MBn_ID(_baseAddress, i) & FLEXCAN_MB_ID_EXT_MASK);
          if (!msg.flags.extended) msg.id >>= FLEXCAN_MB_ID_STD_BIT_NO;
          uint32_t dataIn = FLEXCANb_MBn_WORD0(_baseAddress, i);
          msg.buf[0] = dataIn >> 24; msg.buf[1] = dataIn >> 16; msg.buf[2] = dataIn >> 8; msg.buf[3] = dataIn;
          dataIn = FLEXCANb_MBn_WORD1(_baseAddress, i);
          msg.buf[4] = dataIn >> 24; msg.buf[5] = dataIn >> 16; msg.buf[6] = dataIn >> 8; msg.buf[7] = dataIn;
          msg.mb = i; /* store the mailbox the message came from (for callback reference) */
          if ( _baseAddress == FLEXCAN1_BASE ) msg.bus = 1;

          bool enhance_filtering_success = 0;
          if ( filter_enhancement[i][0] ) { /* filter enhancement (if enabled) */
            if ( !filter_enhancement[i][1] ) { /* multi-ID based filter enhancement */
              for ( uint8_t i = 0; i < 5; i++ ) {
                if ( msg.id == filter_enhancement_config[msg.mb][i] ) {
                  enhance_filtering_success = 1;
                  break;
                }
              }
            }
            else { /* range based ID filtering enhancement */
              if ( (msg.id >= filter_enhancement_config[i][0]) &&
                   (msg.id <= filter_enhancement_config[i][1]) ) enhance_filtering_success = 1;
            }
          }

          /* if enhancement is on and a match is found, OR, enhancement is disabled, run handler or queue as normal */

          if ( ( enhance_filtering_success && filter_enhancement[i][0] ) || !filter_enhancement[i][0] ) { /* if enhanced AND success, OR not enhanced */
            if ( !can_events ) {
              if ( msg.bus == 1 && FlexCAN_T4::_CAN1GLOBALhandler ) FlexCAN_T4::_CAN1GLOBALhandler(msg);
              if ( msg.bus == 2 && FlexCAN_T4::_CAN2GLOBALhandler ) FlexCAN_T4::_CAN2GLOBALhandler(msg);
              sendMSGtoIndividualMBCallback((FLEXCAN_MAILBOX)i, msg); /* send frames direct to callback (unbuffered) */
            }
            else {
              struct2queue(msg); /* store frame in queue ( buffered ) */
            }
            ext_output(msg);
          }

          if (!msg.flags.extended) FLEXCANb_MBn_CS(_baseAddress, i) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY);
          else FLEXCANb_MBn_CS(_baseAddress, i) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY) | FLEXCAN_MB_CS_SRR | FLEXCAN_MB_CS_IDE;
          FLEXCANb_TIMER(_baseAddress); // reading timer unlocks individual mailbox
          if ( i < 32 ) FLEXCANb_IFLAG1(_baseAddress) = (1UL << i); /* immediately flush interrupt of current mailbox */
          else FLEXCANb_IFLAG2(_baseAddress) = (1UL << (i - 32)); /* immediately flush interrupt of current mailbox */
          status &= ~(1ULL << i); /* remove bit from initial flag lookup so it's not set at end when another frame is captured */

          /* callback, queue, or neither, we check other filters for cross-matches */
          if ( msg_distribution && can_events ) packet_distribution(msg);

          break;
        }


      case FLEXCAN_MB_CODE_TX_INACTIVE: {       // TX inactive. Dequeue if available
          status &= ~(1UL << i); /* remove bit from initial flag lookup so it's not set at end when another frame is captured */
          if ( (_baseAddress == FLEXCAN1_BASE && flexcanTx1Buffer.size()) || (_baseAddress == FLEXCAN2_BASE && flexcanTx2Buffer.size()) ) {
            CAN_message_t queueToSend;
            uint8_t buf[sizeof(CAN_message_t)];
            if ( this == &Can1 ) flexcanTx1Buffer.peek_front(buf, sizeof(CAN_message_t));
            else if ( this == &Can2 ) flexcanTx2Buffer.peek_front(buf, sizeof(CAN_message_t));
            memmove(&queueToSend, buf, sizeof(queueToSend));

            if ( queueToSend.seq ) {
              if ( queueToSend.mb == i ) {
                if ( this == &Can1 ) flexcanTx1Buffer.read();
                else if ( this == &Can2 ) flexcanTx2Buffer.read();
                writeTxMailbox(i,queueToSend);
              }
            }
            else {
              if ( this == &Can1 ) flexcanTx1Buffer.pop_front(buf, sizeof(CAN_message_t));
              else if ( this == &Can2 ) flexcanTx2Buffer.pop_front(buf, sizeof(CAN_message_t));
              writeTxMailbox(i,queueToSend);
            }
          }
          if ( i < 32 ) FLEXCANb_IFLAG1(_baseAddress) = (1UL << i); /* immediately flush interrupt of current mailbox */
          else FLEXCANb_IFLAG2(_baseAddress) = (1UL << (i - 32)); 
          status &= ~(1ULL << i); /* remove bit from initial flag lookup so it's not set at end when another frame is captured */
          break;
        }

      case FLEXCAN_MB_CODE_RX_BUSY:           // mailbox is busy, check it later.
      case FLEXCAN_MB_CODE_RX_INACTIVE:       // inactive Receive box. Must be a false alarm!?
      case FLEXCAN_MB_CODE_RX_EMPTY:          // rx empty already. Why did it interrupt then?
      case FLEXCAN_MB_CODE_TX_ABORT:          // TX being aborted.
      case FLEXCAN_MB_CODE_TX_RESPONSE:       // remote request response (deprecated)
      case FLEXCAN_MB_CODE_TX_ONCE:           // TX mailbox is full and will be sent as soon as possible
      case FLEXCAN_MB_CODE_TX_RESPONSE_TEMPO: // remote request junk again. Go away.
        break;
      default:
        break;
    }
  }
  FLEXCANb_IFLAG1(_baseAddress) = status >> 16; /* essentially, if all bits were cleared above, it's basically writing 0 to IFLAG, to prevent new data interruptions. */
  FLEXCANb_IFLAG2(_baseAddress) = status;
  FLEXCANb_ESR1(_baseAddress) |= FLEXCAN_ESR_ERR_INT; /* we clear the ERROR bit if we received a functional callback */
}

void __attribute__((weak)) ext_output(const CAN_message_t &msg) {
}
uint16_t __attribute__((weak)) ext_events() {
  return 0;
}

uint16_t FlexCAN_T4::events() {
    ext_events();

    if ( !can_events ) can_events = 1; /* handle callbacks from loop */

    if ( flexcanRxBuffer.size() ) { /* if a queue frame is available */
      CAN_message_t frame;
      queue2struct(frame);
      if ( !frame.bus && FlexCAN_T4::_CAN1GLOBALhandler ) FlexCAN_T4::_CAN1GLOBALhandler(frame);
      if ( frame.bus && FlexCAN_T4::_CAN2GLOBALhandler ) FlexCAN_T4::_CAN2GLOBALhandler(frame);
      sendMSGtoIndividualMBCallback((FLEXCAN_MAILBOX)frame.mb, frame); 
    }

  return 0;
}

void FlexCAN_T4::packet_distribution(CAN_message_t &frame) {
  uint8_t mailbox_offset = mailboxOffset();
  uint64_t imask = FLEXCANb_IMASK1(_baseAddress) << 16 | FLEXCANb_IMASK2(_baseAddress);

  for ( uint8_t i = 0; i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++) {

    /* if fifo enabled, skip the area it occupies except the MB0 slot when checking interrupt enable status */
    /* continue scanning from 0, then depending on RFFN, lets say 0, scan continues at slot 8 onwards */

    if ( !(imask & (1ULL << i)) ) {
      if ( (imask & FLEXCAN_IMASK1_BUF5M) && (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN) && ( i < mailbox_offset ) ); /* do not skip fifo filters */
      else continue; /* skip non-interrupt mailboxes */
    }

    /* here we prevent 2 or more FIFO filters from duplicating a message frame */
    if ( (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN) && !frame.mb && i < mailbox_offset ) continue;

    if ( filter_set[i] ) { /* if MB/FIFO filter is set */

      if ( filter_enhancement[i][0] ) {
        if ( !filter_enhancement[i][1] ) { /* multi-ID based filter enhancement */
          for ( uint8_t j = 0; j < 5; j++ ) {
            if ( i != frame.mb && frame.id == filter_enhancement_config[i][j] ) {
              uint8_t mb = frame.mb;
              frame.mb = i; /* set ID to distributed mailbox */
              if ( (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN) && i < mailbox_offset ) { /* make FIFO entries always 0 */
                frame.mb = 0;
                i = mailbox_offset - 1;
              }
              struct2queue(frame);
              frame.mb = mb; /* restore id for next cycle */
              break;
            }
          }
        } // MULTI-ID BASED FILTERING


        else { /* if it's range based */

          bool enhance_filtering_success = 0;

          if ( (frame.id >= filter_enhancement_config[i][0]) &&
               (frame.id <= filter_enhancement_config[i][1]) ) enhance_filtering_success = 1;
          if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 1 ) { 
            if ( (frame.id >= filter_enhancement_config[i][2]) &&
                 (frame.id <= filter_enhancement_config[i][3]) ) enhance_filtering_success = 1;
          }

          if ( i != frame.mb && enhance_filtering_success ) {
            uint8_t mb = frame.mb;
            frame.mb = i; /* set ID to distributed mailbox */
            if ( (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN) && i < mailbox_offset ) { /* make FIFO entries always 0 */
              frame.mb = 0;
              i = mailbox_offset - 1;
            }
            struct2queue(frame);
            frame.mb = mb; /* restore id for next cycle */
          }
        } // RANGE BASED

      } // ENHANCED FILTER


      else { /* if no enhancement */

        if ( i >= mailboxOffset() && i != frame.mb ) { /* mailbox area, id from different mailbox */

          /* if the ID doesn't match the EXT flag, skip filter check */
          if ( (( frame.id >  0x7FF ) && !( FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_IDE )) ||
               (( frame.id <= 0x7FF ) &&  ( FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_IDE )) ) continue;

          uint32_t mask = ( !(FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_IDE) ) ? (masks[i] >> 18) & 0x7FF : (masks[i] >> 0) & 0x1FFFFFFF;

          if ( ((frame.id)&mask) == ((filter_enhancement_config[i][0])&mask) ) {
            uint8_t mb = frame.mb;
            frame.mb = i; /* set ID to distributed mailbox */
            struct2queue(frame);
            frame.mb = mb; /* restore id for next cycle */
          }
        } // MB AREA

        else if ( FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN && i < mailboxOffset() && i != frame.mb ) {

          if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 0 ) { /* Table A */

            uint32_t mask = (( !(FLEXCANb_IDFLT_TAB(_baseAddress, i) & (1UL << 30)) ) ? (masks[i] >> 19) & 0x7FF : (masks[i] >> 16) & 0x3FFF);

            if ( ((frame.id)&mask) == ((filter_enhancement_config[i][0])&mask) ) {
              uint8_t mb = frame.mb;
              frame.mb = 0; /* set ID to FIFO buffer */
              i = mailbox_offset - 1;
              struct2queue(frame);
              frame.mb = mb; /* restore id for next cycle */
            }
          } // TABLE_A

          else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 1 ) { /* Table B */

            bool filter_match1 = 0, filter_match2 = 0;

            if ( FLEXCANb_IDFLT_TAB(_baseAddress, i) & (1UL << 30) ) { /* if extended bit */
              if ( ((frame.id >> (29 - 14))&(masks[i]&0x3FFF)) == ((filter_enhancement_config[i][0] >> (29 - 14))&(masks[i]&0x3FFF)) ) filter_match1 = 1;
            }
            else {
              if ( (frame.id&((masks[i] >> 19) & 0x7FF)) == (filter_enhancement_config[i][0]&((masks[i] >> 19) & 0x7FF)) ) filter_match1 = 1;
            }

            if ( FLEXCANb_IDFLT_TAB(_baseAddress, i) & (1UL << 14) ) { /* if extended bit */
              if ( ((frame.id >> (29 - 14))&(masks[i]&0x3FFF)) == ((filter_enhancement_config[i][2] >> (29 - 14))&(masks[i]&0x3FFF)) ) filter_match2 = 1;
            } 
            else {
              if ( (frame.id&((masks[i] >> 19) & 0x7FF)) == (filter_enhancement_config[i][0]&((masks[i] >> 19) & 0x7FF)) ) filter_match2 = 1;
            }

            if ( filter_match1 || filter_match2 ) {
              uint8_t mb = frame.mb;
              frame.mb = 0; /* set ID to FIFO buffer */
              i = mailbox_offset - 1;
              struct2queue(frame);
              frame.mb = mb; /* restore id for next cycle */
            }
          } // TABLE_B

          else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 2 ) { /* Table C */

              bool filter_match = 0;

              uint8_t bits = (( frame.id > 0x7FF ) ? 29 : 11);

              for ( uint8_t j = 0; j < 4; j++ ) {

                /* for partial masks, we double check ext frames dont mix with std frames */
                /* from local copy of original ID and frame.id itself */
                if ( (frame.flags.extended && filter_enhancement_config[i][j] <= 0x7FF) ||
                     (!frame.flags.extended && filter_enhancement_config[i][j] > 0x7FF) ) continue;

                /* look at the 4 partial masks for a match */
                if ( ((frame.id >> (bits - 8))&(masks[i]&0xFF)) == ((filter_enhancement_config[i][j] >> (bits - 8))&(masks[i]&0xFF)) ) {
                  filter_match = 1;
                  break;
                }
              }

              if ( filter_match ) {
                uint8_t mb = frame.mb;
                frame.mb = 0; /* set ID to FIFO buffer */
                i = mailbox_offset - 1;
                struct2queue(frame);
                frame.mb = mb; /* restore id for next cycle */
              }
          } // TABLE_C

        } // FIFO AREA
      } // NOT ENHANCED

    } // FILTER_SET
  } // FOR LOOP
}

void FlexCAN_T4::struct2queue(const CAN_message_t &msg) {
  uint8_t buf[sizeof(CAN_message_t)];
  memmove(buf, &msg, sizeof(msg));
  flexcanRxBuffer.push_back(buf, sizeof(CAN_message_t));
}
void FlexCAN_T4::queue2struct(CAN_message_t &msg) {
  uint8_t buf[sizeof(CAN_message_t)];
  flexcanRxBuffer.pop_front(buf, sizeof(CAN_message_t));
  memmove(&msg, buf, sizeof(msg));
}

int FlexCAN_T4::readFIFO(CAN_message_t &msg) {
  if ( FLEXCANb_IMASK1(_baseAddress) & FLEXCAN_IMASK1_BUF5M ) return 0; /* FIFO interrupt enabled, manual read blocked */
  if ( ( FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN ) && ( FLEXCANb_IFLAG1(_baseAddress) & FLEXCAN_IFLAG1_BUF5I ) ) {
    msg.len = FLEXCAN_get_length(FLEXCANb_MBn_CS(_baseAddress, 0));
    msg.flags.extended = (FLEXCANb_MBn_CS(_baseAddress, 0) & FLEXCAN_MB_CS_IDE) ? 1 : 0;
    msg.flags.remote = (FLEXCANb_MBn_CS(_baseAddress, 0) & FLEXCAN_MB_CS_RTR) ? 1 : 0;
    msg.timestamp = FLEXCAN_get_timestamp (FLEXCANb_MBn_CS(_baseAddress, 0));
    msg.id = (FLEXCANb_MBn_ID(_baseAddress, 0) & FLEXCAN_MB_ID_EXT_MASK);
    if (!msg.flags.extended) msg.id >>= FLEXCAN_MB_ID_STD_BIT_NO;
    uint32_t dataIn = FLEXCANb_MBn_WORD0(_baseAddress, 0);
    msg.buf[0] = dataIn >> 24; msg.buf[1] = dataIn >> 16; msg.buf[2] = dataIn >> 8; msg.buf[3] = dataIn;
    dataIn = FLEXCANb_MBn_WORD1(_baseAddress, 0);
    msg.buf[4] = dataIn >> 24; msg.buf[5] = dataIn >> 16; msg.buf[6] = dataIn >> 8; msg.buf[7] = dataIn;
    FLEXCANb_IFLAG1(_baseAddress) = FLEXCAN_IFLAG1_BUF5I; /* clear FIFO bit only! */
    if ( FLEXCANb_IFLAG1(_baseAddress) & FLEXCAN_IFLAG1_BUF6I ) FLEXCANb_IFLAG1(_baseAddress) = FLEXCAN_IFLAG1_BUF6I;
    if ( FLEXCANb_IFLAG1(_baseAddress) & FLEXCAN_IFLAG1_BUF7I ) FLEXCANb_IFLAG1(_baseAddress) = FLEXCAN_IFLAG1_BUF7I;
    if ( _baseAddress == FLEXCAN1_BASE ) msg.bus = 1;
    else if ( _baseAddress == FLEXCAN2_BASE ) msg.bus = 2;
    msg.mb = 0; /* store the mailbox the message came from (for callback reference) */

    bool enhance_filtering_success = 0;
    uint8_t mailboxes = mailboxOffset();

    for ( uint8_t i = 0; i < mailboxes; i++ ) {
      if ( filter_enhancement[i][0] && filter_set[i] ) { /* if enhancement is active and set */
        if ( !filter_enhancement[i][1] ) { /* if it's multi ID */
          for ( uint8_t j = 0; j < 5; j++ ) {
            if ( msg.id == filter_enhancement_config[i][j] ) {
              enhance_filtering_success = 1;
              break;
            }
          }
        }
        else { /* if it's range based */
          if ( (msg.id >= filter_enhancement_config[i][0]) &&
               (msg.id <= filter_enhancement_config[i][1]) ) enhance_filtering_success = 1;
        }
      }
    }
    if ( ( !enhance_filtering_success && filter_enhancement[0][0] ) ) return 0; /* did not pass enhancement filter */

    return 1;
  }
  return 0;
}

int FlexCAN_T4::read(CAN_message_t &msg) {
  bool _random = random(0, 2);
  if ( ( !_random ) && ( FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN ) &&
       !( FLEXCANb_IMASK1(_baseAddress) & FLEXCAN_IMASK1_BUF5M ) &&
       ( FLEXCANb_IFLAG1(_baseAddress) & FLEXCAN_IFLAG1_BUF5I ) ) return readFIFO(msg);
  return readMB(msg);
}

int FlexCAN_T4::readMB(CAN_message_t &msg) {
  uint8_t cycle_count = 0;

rescan_rx_mbs:

  if ( FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN ) {  /* FIFO is enabled, get only remaining RX (if any) */
    uint32_t remaining_mailboxes = FLEXCANb_MAXMB_SIZE(_baseAddress) - 6 /* MAXMB - FIFO */ - ((((FLEXCANb_CTRL2(_baseAddress) >> FLEXCAN_CTRL2_RFFN_BIT_NO) & 0xF) + 1) * 2);
    if ( FLEXCANb_MAXMB_SIZE(_baseAddress) < (6 + ((((FLEXCANb_CTRL2(_baseAddress) >> FLEXCAN_CTRL2_RFFN_BIT_NO) & 0xF) + 1) * 2))) remaining_mailboxes = 0;
    if ( mailbox_reader_increment < ( FLEXCANb_MAXMB_SIZE(_baseAddress) - remaining_mailboxes ) )
      mailbox_reader_increment = FLEXCANb_MAXMB_SIZE(_baseAddress) - remaining_mailboxes -1;
  }

  if ( ++mailbox_reader_increment >= FLEXCANb_MAXMB_SIZE(_baseAddress) ) {
    mailbox_reader_increment = 0;
    if ( ++cycle_count > FLEXCANb_MAXMB_SIZE(_baseAddress) ) return 0; /* if cycles are greater than number of mailboxes */
    if ( FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN ) goto rescan_rx_mbs; /* FIFO enabled? offset mailbox.. */
  }

  if ( mailbox_reader_increment < 32 ) {
    if (FLEXCANb_IMASK1(_baseAddress) & (1UL << mailbox_reader_increment)) { /* don't read interrupt enabled mailboxes */
      if ( ++cycle_count > FLEXCANb_MAXMB_SIZE(_baseAddress) ) return 0; /* if cycles are greater than number of mailboxes */
      goto rescan_rx_mbs;
    }
  }
  else {
    if (FLEXCANb_IMASK2(_baseAddress) & (1UL << (mailbox_reader_increment - 32))) { /* don't read interrupt enabled mailboxes */
      if ( ++cycle_count > FLEXCANb_MAXMB_SIZE(_baseAddress) ) return 0; /* if cycles are greater than number of mailboxes */
      goto rescan_rx_mbs;
    }
  }

  uint32_t code = FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, mailbox_reader_increment)); // Reading Control Status atomically locks mailbox.
  switch ( code ) {
    case FLEXCAN_MB_CODE_RX_FULL:           // rx full, Copy the frame to RX buffer
    case FLEXCAN_MB_CODE_RX_OVERRUN: {      // rx overrun. Incomming frame overwrote existing frame.
        FLEXCANb_MBn_CS(_baseAddress, mailbox_reader_increment) |= FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_INACTIVE); /* deactivate mailbox */
        if ( FLEXCAN_MB_CODE_RX_OVERRUN == code ) msg.flags.overrun = 1;
        msg.len = FLEXCAN_get_length(FLEXCANb_MBn_CS(_baseAddress, mailbox_reader_increment));
        msg.flags.extended = (FLEXCANb_MBn_CS(_baseAddress, mailbox_reader_increment) & FLEXCAN_MB_CS_IDE) ? 1 : 0;
        msg.flags.remote = (FLEXCANb_MBn_CS(_baseAddress, mailbox_reader_increment) & FLEXCAN_MB_CS_RTR) ? 1 : 0;
        msg.timestamp = FLEXCAN_get_timestamp (FLEXCANb_MBn_CS(_baseAddress, mailbox_reader_increment));
        msg.id = (FLEXCANb_MBn_ID(_baseAddress, mailbox_reader_increment) & FLEXCAN_MB_ID_EXT_MASK);
        if (!msg.flags.extended) msg.id >>= FLEXCAN_MB_ID_STD_BIT_NO;
        uint32_t dataIn = FLEXCANb_MBn_WORD0(_baseAddress, mailbox_reader_increment);
        msg.buf[0] = dataIn >> 24; msg.buf[1] = dataIn >> 16; msg.buf[2] = dataIn >> 8; msg.buf[3] = dataIn;
        dataIn = FLEXCANb_MBn_WORD1(_baseAddress, mailbox_reader_increment);
        msg.buf[4] = dataIn >> 24; msg.buf[5] = dataIn >> 16; msg.buf[6] = dataIn >> 8; msg.buf[7] = dataIn;
        msg.mb = mailbox_reader_increment; /* store the mailbox the message came from (for callback reference) */
        if ( _baseAddress == FLEXCAN1_BASE ) msg.bus = 1;
        else if ( _baseAddress == FLEXCAN2_BASE ) msg.bus = 2;
        if (!msg.flags.extended) FLEXCANb_MBn_CS(_baseAddress, mailbox_reader_increment) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY);
        else FLEXCANb_MBn_CS(_baseAddress, mailbox_reader_increment) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY) | FLEXCAN_MB_CS_SRR | FLEXCAN_MB_CS_IDE;
        FLEXCANb_TIMER(_baseAddress); // reading timer unlocks individual mailbox

        if ( mailbox_reader_increment < 32 ) FLEXCANb_IFLAG1(_baseAddress) = (1UL << mailbox_reader_increment); /* immediately flush interrupt of current mailbox */
        else FLEXCANb_IFLAG1(_baseAddress) = (1UL << (mailbox_reader_increment - 32));

        if ( filter_enhancement[mailbox_reader_increment][0] ) { /* filter enhancement (if enabled) */
          if ( !filter_enhancement[mailbox_reader_increment][1] ) { /* multi-ID based filter enhancement */
            for ( uint8_t i = 0; i < 5; i++ ) {
              if ( msg.id == filter_enhancement_config[mailbox_reader_increment][i] ) return 1;
            }
          }
          else { /* range based ID filtering enhancement */
            if ( (msg.id >= filter_enhancement_config[mailbox_reader_increment][0]) &&
                 (msg.id <= filter_enhancement_config[mailbox_reader_increment][1]) ) return 1;
          }
          goto rescan_rx_mbs; /* enhanced filtering match not found, check for another frame */
        }
        return 1; /* we got a frame, exit */
      }
    case FLEXCAN_MB_CODE_TX_INACTIVE: {       // TX inactive. Just chillin' waiting for a message to send.
        goto rescan_rx_mbs;
      }
    case FLEXCAN_MB_CODE_RX_BUSY:           // mailbox is busy, check it later.
    case FLEXCAN_MB_CODE_RX_INACTIVE:       // inactive Receive box. Must be a false alarm!?
    case FLEXCAN_MB_CODE_RX_EMPTY:          // rx empty already. Why did it interrupt then?
    case FLEXCAN_MB_CODE_TX_ABORT:          // TX being aborted.
    case FLEXCAN_MB_CODE_TX_RESPONSE:       // remote request response (deprecated)
    case FLEXCAN_MB_CODE_TX_ONCE:           // TX mailbox is full and will be sent as soon as possible
    case FLEXCAN_MB_CODE_TX_RESPONSE_TEMPO: // remote request junk again. Go away.
    default:
      break;
  }
  return 0; /* no messages available */
}

void FlexCAN_T4::writeTxMailbox(uint8_t mb_num, const CAN_message_t &msg) {
  if ( mb_num < 32 ) FLEXCANb_IFLAG1(_baseAddress) |= (1UL << mb_num); // 1st step clear flag in case it's set as per datasheet
  else FLEXCANb_IFLAG2(_baseAddress) |= (1UL << (mb_num - 32)); // 1st step clear flag in case it's set as per datasheet

  if (msg.flags.extended) FLEXCANb_MBn_ID(_baseAddress, mb_num) = (msg.id & FLEXCAN_MB_ID_EXT_MASK);
  else FLEXCANb_MBn_ID(_baseAddress, mb_num) = FLEXCAN_MB_ID_IDSTD(msg.id);
  FLEXCANb_MBn_WORD0(_baseAddress, mb_num) = (msg.buf[0] << 24) | (msg.buf[1] << 16) | (msg.buf[2] << 8) | msg.buf[3];
  FLEXCANb_MBn_WORD1(_baseAddress, mb_num) = (msg.buf[4] << 24) | (msg.buf[5] << 16) | (msg.buf[6] << 8) | msg.buf[7];

  FLEXCANb_MBn_CS(_baseAddress, mb_num) = (FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_ONCE) |
                                      FLEXCAN_MB_CS_LENGTH(msg.len) |
                                      ((msg.flags.remote) ? FLEXCAN_MB_CS_RTR : 0UL) |
                                      ((msg.flags.extended) ? FLEXCAN_MB_CS_IDE : 0UL) |
                                      ((msg.flags.extended) ? FLEXCAN_MB_CS_SRR : 0UL));
}

int FlexCAN_T4::write(FLEXCAN_MAILBOX mb_num, const CAN_message_t &msg) {
  if ( mb_num < mailboxOffset() ) return 0; /* FIFO doesn't transmit */
  if ( !((FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, mb_num))) >> 3) ) return 0; /* not a transmit mailbox */

  if ( mb_num < 32 ) FLEXCANb_IMASK1(_baseAddress) &= ~(1UL << mb_num); /* we disable the TX interrupt so we can use the mailbox directly (prevent ISR takeover for dequeue) */
  else FLEXCANb_IMASK2(_baseAddress) &= ~(1UL << (mb_num - 32));

  uint32_t timeout = millis();
  while ( FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, mb_num)) != FLEXCAN_MB_CODE_TX_INACTIVE ) {
    if ( millis() - timeout > 100 ) {
      if ( mb_num < 32 ) FLEXCANb_IMASK1(_baseAddress) |= (1UL << mb_num); /* we restore the TX interrupt before exiting on fail */
      else FLEXCANb_IMASK2(_baseAddress) |= (1UL << (mb_num - 32)); 
      return 0; /* we exit out on a timeout */
    }
  }
  writeTxMailbox(mb_num,msg);
  if ( mb_num < 32 ) FLEXCANb_IMASK1(_baseAddress) |= (1UL << mb_num); /* we restore the TX interrupt on success */
  else FLEXCANb_IMASK2(_baseAddress) |= (1UL << (mb_num - 32)); 
  return 1; // transmit entry accepted //
}

void FlexCAN_T4::currentMasks() {

  FLEXCAN_EnterFreezeMode(); /* let's get current list, must be in FRZ mode */
  for ( uint8_t i = 0; i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++ ) masks[i] = FLEXCANb_RXIMR(_baseAddress, i);
  FLEXCAN_ExitFreezeMode();

  Serial.println();
  if ( (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN) ) {
    Serial.print("FIFO enabled. Filters 0 to ");
    Serial.print(mailboxOffset() - 1);
    Serial.println(" used for FIFO.");
  }

  Serial.print("\nMasks:\n");
  char mask_padded[10];
  for ( uint8_t i = 0; i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++ ) {

    if ( (i >= mailboxOffset()) && (FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, i)) & 0x08) ) { // TX or RX mailbox?
      Serial.print("[ ");
      Serial.print(i);
      Serial.print(" ]:\tTransmit Mailbox\n\n");
      continue;
    }

    sprintf(mask_padded, "%08lX", masks[i]);
    Serial.print("[ ");
    Serial.print(i);
    Serial.print(" ]:\tFlexcan Mask: 0x");
    Serial.print(mask_padded);

    if ( (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN) && i < mailboxOffset() ) { /* detailed filter for FIFO only */

      Serial.print(" ( FIFO Filter )\n\t\t^-- ");

      if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 0 ) { /* Table A */
        if ( !filter_set[i] ) {
          Serial.print(" [Table A]\n\t\t\t * Filter was not set\n\n");
          continue;
        }

        Serial.print(" [Table A]\n\t\t\t * User Mask: ");
        if ( !(masks[i] & 0x1FFFE) ) {
          Serial.print("(Standard Mask) 0x");
          Serial.print((masks[i] >> 19)&0x7FF,HEX);
        }
        else {
          Serial.print("(Extended Mask) 0x");
          Serial.print((masks[i] >> 1)&0x1FFFFFFF,HEX);
        }
        Serial.println();
      }


      else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 1 ) { /* Table B */
        if ( !filter_set[i] ) {
          Serial.print(" [Table B]\n\t\t\t * Filter was not set\n\n");
          continue;
        }

        Serial.print(" [Table B]\n\t\t\t * User Mask 1: ");
        if ( (FLEXCANb_IDFLT_TAB(_baseAddress, i) & (1UL<<30)) ) {
          Serial.print("(Extended Mask) 0x");
          Serial.print((masks[i] >> 16)&0x3FFF,HEX);
        }
        else {
          Serial.print("(Standard Mask) 0x");
          Serial.print((masks[i] >> 19)&0x7FF,HEX);
        }

        Serial.print("\n\t\t\t * User Mask 2: ");

        if ( (FLEXCANb_IDFLT_TAB(_baseAddress, i) & (1UL<<14)) ) {
          Serial.print("(Extended Mask) 0x");
          Serial.print((masks[i] >> 0)&0x3FFF,HEX);
        }
        else {
          Serial.print("(Standard Mask) 0x");
          Serial.print((masks[i] >> 3)&0x7FF,HEX);
        }

        Serial.println();

      }
      else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 2 ) { /* Table C */
        if ( !filter_set[i] ) {
          Serial.print(" [Table C] * Filter was not set\n\n");
          continue;
        }
        Serial.print(" [Table C]\n\t\t\t * User Mask 1: 0x");
        Serial.print((masks[i])&0xFF000000,HEX);
        Serial.print("\n\t\t\t * User Mask 2: 0x");
        Serial.print((masks[i])&0x00FF0000,HEX);
        Serial.print("\n\t\t\t * User Mask 3: 0x");
        Serial.print((masks[i])&0x0000FF00,HEX);
        Serial.print("\n\t\t\t * User Mask 4: 0x");
        Serial.println((masks[i])&0x000000FF,HEX);
      }
    }

    if ( i >= mailboxOffset() ) { /* detailed filter for mailboxes only */
      Serial.print(" ( Mailbox Filter )\n\t\t^-- ");

      if ( !filter_set[i] ) {
        Serial.print("* Filter was not set\n\n");
        continue;
      }

      if ( (FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_IDE) ? 0 : 1 ) {
        Serial.print("(Standard Mask) * User Mask: 0x");
        Serial.print((masks[i] >> 18)&0x7FF,HEX);
      }
      else {
        Serial.print("(Extended Mask) * User Mask: 0x");
        Serial.print((masks[i] >> 0)&0x1FFFFFFF,HEX);
      }
      Serial.println();
    }
    Serial.println();
  }
  Serial.println();
}

int FlexCAN_T4::getFirstTxBox() {
  uint8_t count = 0;
  for (uint8_t i = mailboxOffset(); i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++) {
    if ( (FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, i)) >> 3) ) count++; // if TX
  }
  if ( !count ) return -1;
  return (getNumMailBoxes() - count);
}

int FlexCAN_T4::write(const CAN_message_t &msg) {
  CAN_message_t sequence = msg;

  if ( this == &Can1 ) sequence.bus = 1;
  else if ( this == &Can2 ) sequence.bus = 2;

  if ( sequence.seq ) {
    sequence.mb = getFirstTxBox();
    if ( (sequence.bus == 1 && flexcanTx1Buffer.size() && flexcanTx1Buffer.size() != flexcanTx1Buffer.capacity()) ||
         (sequence.bus == 2 && flexcanTx2Buffer.size() && flexcanTx2Buffer.size() != flexcanTx2Buffer.capacity()) ) { /* queue if buffer exists and not full */
      NVIC_DISABLE_IRQ(NVIC_IRQ);
      struct2queueTx(sequence);
      NVIC_ENABLE_IRQ(NVIC_IRQ);
      return 1;
    }
    if ( FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, sequence.mb)) == FLEXCAN_MB_CODE_TX_INACTIVE ) {
      writeTxMailbox(sequence.mb, sequence);
      return 1;
    }
  }
  else {
    for (uint8_t i = mailboxOffset(); i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++) {
      if ( FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, i)) == FLEXCAN_MB_CODE_TX_INACTIVE ) {
        writeTxMailbox(i, msg);
        return 1; /* transmit entry accepted */
      }
    }
  }
  if ( (sequence.bus == 1 && flexcanTx1Buffer.size() < flexcanTx1Buffer.capacity()) ||
       (sequence.bus == 2 && flexcanTx2Buffer.size() < flexcanTx2Buffer.capacity()) ) {
    NVIC_DISABLE_IRQ(NVIC_IRQ);
    struct2queueTx(sequence);
    NVIC_ENABLE_IRQ(NVIC_IRQ);
    return 1; /* transmit entry accepted */
  }
  return 0; /* transmit entry failed, no mailboxes available */
}

void FlexCAN_T4::struct2queueTx(const CAN_message_t &msg) {
  uint8_t buf[sizeof(CAN_message_t)];
  memmove(buf, &msg, sizeof(msg));
  if ( msg.bus == 1 ) flexcanTx1Buffer.push_back(buf, sizeof(CAN_message_t));
  else if ( msg.bus == 2 ) flexcanTx2Buffer.push_back(buf, sizeof(CAN_message_t));
}

void FlexCAN_T4::onReceive(const FLEXCAN_MAILBOX &mb_num, _MB_ptr handler) {
  if ( _baseAddress == FLEXCAN1_BASE ) FlexCAN_T4::_CAN1MBhandlers[mb_num] = handler;
  else if ( _baseAddress == FLEXCAN2_BASE ) FlexCAN_T4::_CAN2MBhandlers[mb_num] = handler;
}

void FlexCAN_T4::onReceive(_MB_ptr handler) {
  if ( _baseAddress == FLEXCAN1_BASE ) FlexCAN_T4::_CAN1GLOBALhandler = handler;
  else if ( _baseAddress == FLEXCAN2_BASE ) FlexCAN_T4::_CAN2GLOBALhandler = handler;
}

void FlexCAN_T4::setBaudRate(uint32_t baud) {
  currentBitrate = baud;
  uint32_t divisor = 0, bestDivisor = 0, result = 20000000 / baud / (divisor + 1);
  int error = baud - (20000000 / (result * (divisor + 1))), bestError = error;
  bool frz_flag_negate = 0;

  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FRZ_ACK) ) { // currently not in freeze mode
    frz_flag_negate = 1; FLEXCAN_EnterFreezeMode();
  }

  while (result > 5) {
    divisor++;
    result = 20000000 / baud / (divisor + 1);
    if (result <= 25) {
      error = baud - (20000000 / (result * (divisor + 1)));
      if (error < 0) error *= -1;
      if (error < bestError) {
        bestError = error;
        bestDivisor = divisor;
      }
      if ((error == bestError) && (result > 11) && (result < 19)) {
        bestError = error;
        bestDivisor = divisor;
      }
    }
  }

  divisor = bestDivisor;
  result = 20000000 / baud / (divisor + 1);

  if ((result < 5) || (result > 25) || (bestError > 300)) {
    if ( frz_flag_negate ) FLEXCAN_ExitFreezeMode();
    return;
  }

  result -= 5; // the bitTimingTable is offset by 5 since there was no reason to store bit timings for invalid numbers
  uint8_t bitTimingTable[21][3] = {
    {0, 0, 1}, //5
    {1, 0, 1}, //6
    {1, 1, 1}, //7
    {2, 1, 1}, //8
    {2, 2, 1}, //9
    {2, 3, 1}, //10
    {2, 3, 2}, //11
    {2, 4, 2}, //12
    {2, 5, 2}, //13
    {2, 5, 3}, //14
    {2, 6, 3}, //15
    {2, 7, 3}, //16
    {2, 7, 4}, //17
    {3, 7, 4}, //18
    {3, 7, 5}, //19
    {4, 7, 5}, //20
    {4, 7, 6}, //21
    {5, 7, 6}, //22
    {6, 7, 6}, //23
    {6, 7, 7}, //24
    {7, 7, 7}, //25
  }, propSeg = bitTimingTable[result][0], pSeg1 = bitTimingTable[result][1], pSeg2 = bitTimingTable[result][2];
  FLEXCANb_CTRL1(_baseAddress) = (FLEXCAN_CTRL_PROPSEG(propSeg) | FLEXCAN_CTRL_RJW(1) | FLEXCAN_CTRL_PSEG1(pSeg1) |
                    FLEXCAN_CTRL_PSEG2(pSeg2) | FLEXCAN_CTRL_ERR_MSK | FLEXCAN_CTRL_PRESDIV(divisor));
  FLEXCANb_CTRL1(_baseAddress) &= ~FLEXCAN_CTRL_LOM; /* disable listen-only mode */
  if ( frz_flag_negate ) FLEXCAN_ExitFreezeMode();
}

bool FlexCAN_T4::setMB(const FLEXCAN_MAILBOX &mb_num, const FLEXCAN_RXTX &mb_rx_tx, const FLEXCAN_IDE &ide) {
  if ( mb_num < mailboxOffset() ) return 0; /* mailbox not available */

   /* immediately disable mailbox interrupt */
  if ( mb_num < 32 ) FLEXCANb_IMASK1(_baseAddress) &= ~(1UL << mb_num);
  else FLEXCANb_IMASK2(_baseAddress) &= ~(1UL << (mb_num - 32));

  FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, mb_num)); // Reading Control Status atomically locks mailbox (if it is RX mode).
  FLEXCANb_MBn_ID(_baseAddress, mb_num) = 0x00000000;
  FLEXCANb_MBn_WORD0(_baseAddress, mb_num) = 0x00000000;
  FLEXCANb_MBn_WORD1(_baseAddress, mb_num) = 0x00000000;

  if ( mb_rx_tx == RX ) {
    if ( ide != EXT ) FLEXCANb_MBn_CS(_baseAddress, mb_num) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY);
    else FLEXCANb_MBn_CS(_baseAddress, mb_num) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY) | FLEXCAN_MB_CS_SRR | FLEXCAN_MB_CS_IDE;
  }

  if ( mb_rx_tx == TX ) {
    FLEXCANb_MBn_CS(_baseAddress, mb_num) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE);
    if ( mb_num < 32 ) FLEXCANb_IMASK1(_baseAddress) |= (1UL << mb_num); /* enable TX mailbox interrupt */
    else FLEXCANb_IMASK2(_baseAddress) |= (1UL << (mb_num - 32));
  }

  FLEXCANb_TIMER(_baseAddress); /* reading timer unlocks individual mailbox */
  
  if ( mb_num < 32 ) FLEXCANb_IFLAG1(_baseAddress) = ( 1UL << mb_num ); /* clear mailbox reception flag */
  else FLEXCANb_IFLAG2(_baseAddress) = ( 1UL << (mb_num - 32));
  return 1;
}

void FlexCAN_T4::enableFIFOInterrupt(bool status) {
  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN) ) return; /* FIFO must be enabled first */
  if ( FLEXCANb_IMASK1(_baseAddress) & FLEXCAN_IMASK1_BUF5M ) return; /* FIFO interrupts already enabled */
  FLEXCANb_IMASK1(_baseAddress) &= ~0xFF; /* disable FIFO interrupt flags */
  if ( status ) FLEXCANb_IMASK1(_baseAddress) |= FLEXCAN_IMASK1_BUF5M; /* enable FIFO interrupt */
}

void FlexCAN_T4::enableMBInterrupt(const FLEXCAN_MAILBOX &mb_num, bool status) {
  if ( (FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, mb_num)) >> 3) ) return; // if TX, don't disable
  if ( mb_num < mailboxOffset() ) return; /* mailbox not available */
  if ( status ) { /* enable mailbox interrupt */
    if ( mb_num < 32 ) FLEXCANb_IMASK1(_baseAddress) |= (1UL << mb_num);
    else FLEXCANb_IMASK2(_baseAddress) |= (1UL << (mb_num - 32));
  }
  else { /* disable mailbox interrupt */
  if ( mb_num < 32 ) FLEXCANb_IMASK1(_baseAddress) &= ~(1UL << mb_num);
    else FLEXCANb_IMASK2(_baseAddress) &= ~(1UL << mb_num);
  }
}

void FlexCAN_T4::enableFIFO(bool status) {
  bool frz_flag_negate = 0;
  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FRZ_ACK) ) { // currently not in freeze mode
    frz_flag_negate = 1; FLEXCAN_EnterFreezeMode();
  }
  FLEXCANb_MCR(_baseAddress) &= ~FLEXCAN_MCR_FEN; // Disable FIFO if already enabled for cleanup.
  FLEXCANb_IMASK1(_baseAddress) = 0UL; // disable all FIFO/MB Interrupts
  FLEXCANb_IMASK2(_baseAddress) = 0UL; // disable all FIFO/MB Interrupts
  for (uint8_t i = 0; i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++ ) { // clear all mailboxes
    FLEXCANb_MBn_ID(_baseAddress, i) = 0x00000000;
    FLEXCANb_MBn_WORD0(_baseAddress, i) = 0x00000000;
    FLEXCANb_MBn_WORD1(_baseAddress, i) = 0x00000000;
    FLEXCANb_MBn_CS(_baseAddress, i) = 0x00000000;
    FLEXCANb_RXIMR(_baseAddress, i) = 0UL; // CLEAR MAILBOX MASKS (RXIMR)
  }
  /*
    RXMGMASK is provided for legacy application support.
    •  When the MCR[IRMQ] bit is negated, RXMGMASK is always in effect.
    •  When the MCR[IRMQ] bit is asserted, RXMGMASK has no effect.
    RXMGMASK is used to mask the filter fields of all Rx MBs, excluding MBs 14-15,
    which have individual mask registers
    RX14MASK/RX15MASK is provided for legacy application support. When the MCR[IRMQ] bit is
    asserted, RX14MASK/RX15MASK has no effect
  */
  FLEXCANb_RXMGMASK(_baseAddress) = 0;
  FLEXCANb_RXFGMASK(_baseAddress) = 0;
  /*
      Enable RX FIFO
      Before enabling the RFEN, the CPU must service the IFLAG bits asserted in the Rx
      FIFO region; Otherwise, these IFLAG bits will mistakenly show
      the related MBs now belonging to FIFO as having contents to be serviced.
  */
  FLEXCANb_IFLAG1(_baseAddress) = FLEXCANb_IFLAG1(_baseAddress); // (all bits reset when written back)
  FLEXCANb_IFLAG2(_baseAddress) = FLEXCANb_IFLAG2(_baseAddress); // (all bits reset when written back)
  if ( status ) {
    FLEXCANb_MCR(_baseAddress) |= FLEXCAN_MCR_FEN;
    /*
      Each group of eight filters occupies a memory space equivalent to two Message Buffers which
      means that the more filters are implemented the less Mailboxes will be available.
    */
    FLEXCAN_set_rffn(FLEXCANb_CTRL2(_baseAddress), 0); // setup 8 Filters for FIFO, 0-5 = FIFO, 6-7 FILTER, 8-64 MBs, max value 0xF which leaves MB14/15 free to use.
    // Setup TX mailboxes from 8 -> max, FIFO uses the first 8 (6MB for FIFO, 2MB for 8 filters for FIFO).

    for (uint8_t i = mailboxOffset(); i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++) FLEXCANb_MBn_CS(_baseAddress,i) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE);
  } 
  else { // FIFO disabled default setup of mailboxes, 0-7 RX, 8-15 TX
    for (uint8_t i = 0; i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++ ) { // clear all mailboxes
      if ( i < 8 ) {
        FLEXCANb_MBn_ID(_baseAddress, i) = 0x00000000;
        FLEXCANb_MBn_WORD0(_baseAddress, i) = 0x00000000;
        FLEXCANb_MBn_WORD1(_baseAddress, i) = 0x00000000;
        if ( i < 4 ) FLEXCANb_MBn_CS(_baseAddress, i) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY);
        else FLEXCANb_MBn_CS(_baseAddress, i) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_RX_EMPTY) | FLEXCAN_MB_CS_IDE | FLEXCAN_MB_CS_SRR;
      }
      else {
        FLEXCANb_MBn_ID(_baseAddress, i) = 0x00000000;
        FLEXCANb_MBn_WORD0(_baseAddress, i) = 0x00000000;
        FLEXCANb_MBn_WORD1(_baseAddress, i) = 0x00000000;
        FLEXCANb_MBn_CS(_baseAddress, i) = FLEXCAN_MB_CS_CODE(FLEXCAN_MB_CODE_TX_INACTIVE);
      }
    }
  }
  if ( frz_flag_negate ) FLEXCAN_ExitFreezeMode();
}

void FlexCAN_T4::mailboxStatus() {
  if ( FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN ) {
    Serial.print("FIFO Enabled --> "); ( FLEXCANb_IMASK1(_baseAddress) & FLEXCAN_IFLAG1_BUF5I ) ? Serial.println("Interrupt Enabled") : Serial.println("Interrupt Disabled");
    Serial.print("\tFIFO Filters in use: ");
    Serial.println((((FLEXCANb_CTRL2(_baseAddress) >> FLEXCAN_CTRL2_RFFN_BIT_NO) & 0xF) + 1) * 8); // 8 filters per 2 mailboxes
    Serial.print("\tRemaining Mailboxes: ");
    uint32_t remaining_mailboxes = FLEXCANb_MAXMB_SIZE(_baseAddress) - 6 /* MAXMB - FIFO */ - ((((FLEXCANb_CTRL2(_baseAddress) >> FLEXCAN_CTRL2_RFFN_BIT_NO) & 0xF) + 1) * 2);
    if ( FLEXCANb_MAXMB_SIZE(_baseAddress) < (6 + ((((FLEXCANb_CTRL2(_baseAddress) >> FLEXCAN_CTRL2_RFFN_BIT_NO) & 0xF) + 1) * 2))) remaining_mailboxes = 0;
    Serial.println(remaining_mailboxes); // 8 filters per 2 mailboxes
    for ( uint8_t i = FLEXCANb_MAXMB_SIZE(_baseAddress) - remaining_mailboxes; i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++ ) {
      switch ( FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, i)) ) {
        case 0b0000: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_INACTIVE"); break;
          }
        case 0b0100: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.print(" code: RX_EMPTY");
            (FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_IDE) ? Serial.println("\t(Extended Frame)") : Serial.println("\t(Standard Frame)");
            break;
          }
        case 0b0010: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_FULL"); break;
          }
        case 0b0110: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_OVERRUN"); break;
          }
        case 0b1010: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_RANSWER"); break;
          }
        case 0b0001: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_BUSY"); break;
          }
        case 0b1000: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: TX_INACTIVE"); break;
          }
        case 0b1001: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: TX_ABORT"); break;
          }
        case 0b1100: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.print(" code: TX_DATA (Transmitting)");
            uint32_t extid = (FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_IDE);
            (extid) ? Serial.print("(Extended Frame)") : Serial.print("(Standard Frame)");
            uint32_t dataIn = FLEXCANb_MBn_WORD0(_baseAddress, i);
            uint32_t id = (FLEXCANb_MBn_ID(_baseAddress, i) & FLEXCAN_MB_ID_EXT_MASK);
            if (!extid) id >>= FLEXCAN_MB_ID_STD_BIT_NO;
            Serial.print("(ID: 0x"); Serial.print(id, HEX); Serial.print(")");
            Serial.print("(Payload: "); Serial.print((uint8_t)(dataIn >> 24), HEX);
            Serial.print(" "); Serial.print((uint8_t)(dataIn >> 16), HEX);
            Serial.print(" "); Serial.print((uint8_t)(dataIn >> 8), HEX);
            Serial.print(" "); Serial.print((uint8_t)dataIn, HEX);
            dataIn = FLEXCANb_MBn_WORD1(_baseAddress, i);
            Serial.print(" "); Serial.print((uint8_t)(dataIn >> 24), HEX);
            Serial.print(" "); Serial.print((uint8_t)(dataIn >> 16), HEX);
            Serial.print(" "); Serial.print((uint8_t)(dataIn >> 8), HEX);
            Serial.print(" "); Serial.print((uint8_t)dataIn, HEX);
            Serial.println(")");
            break;
          }
        case 0b1110: {
            Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: TX_TANSWER"); break;
          }
      }
    } // for loop
    return;
  } // fifo detected ends here
  Serial.print("FIFO Disabled\n\tMailboxes:\n");
  for ( uint8_t i = 0; i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++ ) {
    switch ( FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, i)) ) {
      case 0b0000: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_INACTIVE"); break;
        }
      case 0b0100: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.print(" code: RX_EMPTY");
          (FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_IDE) ? Serial.println("\t(Extended Frame)") : Serial.println("\t(Standard Frame)");
          break;
        }
      case 0b0010: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_FULL"); break;
        }
      case 0b0110: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_OVERRUN"); break;
        }
      case 0b1010: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_RANSWER"); break;
        }
      case 0b0001: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: RX_BUSY"); break;
        }
      case 0b1000: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: TX_INACTIVE"); break;
        }
      case 0b1001: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: TX_ABORT"); break;
        }
      case 0b1100: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.print(" code: TX_DATA (Transmitting)");
          uint32_t extid = (FLEXCANb_MBn_CS(_baseAddress, i) & FLEXCAN_MB_CS_IDE);
          (extid) ? Serial.print("(Extended Frame)") : Serial.print("(Standard Frame)");
          uint32_t dataIn = FLEXCANb_MBn_WORD0(_baseAddress, i);
          uint32_t id = (FLEXCANb_MBn_ID(_baseAddress, i) & FLEXCAN_MB_ID_EXT_MASK);
          if (!extid) id >>= FLEXCAN_MB_ID_STD_BIT_NO;
          Serial.print("(ID: 0x"); Serial.print(id, HEX); Serial.print(")");
          Serial.print("(Payload: "); Serial.print((uint8_t)(dataIn >> 24), HEX);
          Serial.print(" "); Serial.print((uint8_t)(dataIn >> 16), HEX);
          Serial.print(" "); Serial.print((uint8_t)(dataIn >> 8), HEX);
          Serial.print(" "); Serial.print((uint8_t)dataIn, HEX);
          dataIn = FLEXCANb_MBn_WORD1(_baseAddress, i);
          Serial.print(" "); Serial.print((uint8_t)(dataIn >> 24), HEX);
          Serial.print(" "); Serial.print((uint8_t)(dataIn >> 16), HEX);
          Serial.print(" "); Serial.print((uint8_t)(dataIn >> 8), HEX);
          Serial.print(" "); Serial.print((uint8_t)dataIn, HEX);
          Serial.println(")");
          break;
        }
      case 0b1110: {
          Serial.print("\t\tMB"); Serial.print(i); Serial.println(" code: TX_TANSWER"); break;
        }
    }
  } // for loop
}

void FlexCAN_T4::setMRP(bool mrp) { /* mailbox priority (1) or FIFO priority (0) */
  FLEXCAN_EnterFreezeMode();
  if ( mrp ) FLEXCANb_CTRL2(_baseAddress) |= FLEXCAN_CTRL2_MRP;
  else FLEXCANb_CTRL2(_baseAddress) &= ~FLEXCAN_CTRL2_MRP;
  FLEXCAN_ExitFreezeMode();
}

void FlexCAN_T4::setRRS(bool rrs) { /* store remote frames */
  FLEXCAN_EnterFreezeMode();
  if ( rrs ) FLEXCANb_CTRL2(_baseAddress) |= FLEXCAN_CTRL2_RRS;
  else FLEXCANb_CTRL2(_baseAddress) &= ~FLEXCAN_CTRL2_RRS;
  FLEXCAN_ExitFreezeMode();
}

uint8_t FlexCAN_T4::mailboxOffset() {
  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN ) ) return 0; /* return offset 0 since FIFO is disabled */
  uint32_t remaining_mailboxes = FLEXCANb_MAXMB_SIZE(_baseAddress) - 6 /* MAXMB - FIFO */ - ((((FLEXCANb_CTRL2(_baseAddress) >> FLEXCAN_CTRL2_RFFN_BIT_NO) & 0xF) + 1) * 2);
  if ( FLEXCANb_MAXMB_SIZE(_baseAddress) < (6 + ((((FLEXCANb_CTRL2(_baseAddress) >> FLEXCAN_CTRL2_RFFN_BIT_NO) & 0xF) + 1) * 2))) remaining_mailboxes = 0;
  return (FLEXCANb_MAXMB_SIZE(_baseAddress) - remaining_mailboxes); /* otherwise return offset MB position after FIFO area */
}

void FlexCAN_T4::softReset() {
  /*
    Soft Reset
    When this bit is asserted, FlexCAN resets its internal state machines and some of the memory mapped
    registers. The following registers are reset: MCR (except the MDIS bit), TIMER, ECR, ESR1, ESR2,
    IMASK1, IMASK2, IFLAG1, IFLAG2 and CRCR. Configuration registers that control the interface to the
    CAN bus are not affected by soft reset. The following registers are unaffected:
    CTRL1, CTRL2, RXIMR0_RXIMR63, RXGMASK, RX14MASK, RX15MASK, RXFGMASK, RXFIR and all
    Message Buffers
  */
  FLEXCANb_MCR(_baseAddress) ^= FLEXCAN_MCR_SOFT_RST;
  while (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_SOFT_RST);
}

void FlexCAN_T4::softResetRestore() {
  /*
    Soft Reset
    When this bit is asserted, FlexCAN resets its internal state machines and some of the memory mapped
    registers. The following registers are reset: MCR (except the MDIS bit), TIMER, ECR, ESR1, ESR2,
    IMASK1, IMASK2, IFLAG1, IFLAG2 and CRCR. Configuration registers that control the interface to the
    CAN bus are not affected by soft reset. The following registers are unaffected:
    CTRL1, CTRL2, RXIMR0_RXIMR63, RXGMASK, RX14MASK, RX15MASK, RXFGMASK, RXFIR and all
    Message Buffers
  */

  bool frz_flag_negate = 0;
  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FRZ_ACK) ) { // currently not in freeze mode
    frz_flag_negate = 1; FLEXCAN_EnterFreezeMode();
  }

  uint32_t mcr = FLEXCANb_MCR(_baseAddress);
  uint32_t imask1 = FLEXCANb_IMASK1(_baseAddress);
  uint32_t imask2 = FLEXCANb_IMASK2(_baseAddress);

  FLEXCANb_MCR(_baseAddress) ^= FLEXCAN_MCR_SOFT_RST;
  while (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_SOFT_RST);

  FLEXCANb_MCR(_baseAddress) = mcr;
  FLEXCANb_IMASK1(_baseAddress) = imask1;
  FLEXCANb_IMASK2(_baseAddress) = imask2;
  FLEXCANb_IFLAG1(_baseAddress) = FLEXCANb_IFLAG1(_baseAddress);
  FLEXCANb_IFLAG2(_baseAddress) = FLEXCANb_IFLAG2(_baseAddress);
  if ( frz_flag_negate ) FLEXCAN_ExitFreezeMode();
}

void FlexCAN_T4::FLEXCAN_ExitFreezeMode() {
  FLEXCANb_MCR(_baseAddress) &= ~FLEXCAN_MCR_HALT;
  while (FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FRZ_ACK);
}

void FlexCAN_T4::FLEXCAN_EnterFreezeMode() {
  FLEXCANb_MCR(_baseAddress) |= FLEXCAN_MCR_HALT;
  while (!(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FRZ_ACK));
}

void FlexCAN_T4::setTX(FLEXCAN_PINS select) {
  if ( _baseAddress == FLEXCAN2_BASE ) {
    CORE_PIN1_CONFIG = 0x10 | 0;
    CORE_PIN1_PADCONFIG = 0x1A000;
  }
}

void FlexCAN_T4::setRX(FLEXCAN_PINS select) {
  if ( _baseAddress == FLEXCAN2_BASE ) {
    CORE_PIN0_CONFIG = 0x10 | 0;
    CORE_PIN0_PADCONFIG = 0x1A000;
    IOMUXC_FLEXCAN2_RX_SELECT_INPUT = 0x1;
  }
}

void FlexCAN_T4::setMaxMB(uint8_t last) {
  last = constrain(last,1,64);
  last--;

  FLEXCAN_EnterFreezeMode();
  /*
    Before updating
    MCR[MAXMB] field, Arm must treat the IFLAG1 and IFLAG2 bits which MB value is greater than
    the MCR[MAXMB] to be updated, otherwise they will keep set and be inconsistent with
    the amount of MBs available.
  */

  bool fifo_was_cleared = 0;
  if ( FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN ) fifo_was_cleared = 1; /* let fifo clear current mailboxes */
  disableFIFO();
  
  FLEXCANb_IFLAG1(_baseAddress) = FLEXCANb_IFLAG1(_baseAddress);
  FLEXCANb_IFLAG2(_baseAddress) = FLEXCANb_IFLAG2(_baseAddress);
  FLEXCANb_MCR(_baseAddress) = (FLEXCANb_MCR(_baseAddress) & ~0x7F) | last;
  if ( fifo_was_cleared ) enableFIFO();
}

void FlexCAN_T4::acceptedIDs(const FLEXCAN_MAILBOX &mb_num, bool list) {

  if ( mb_num < mailboxOffset() ) return; /* mailbox not available */

  if ( mb_num == FIFO ) {

    if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 0 ) { /* Table A */
      Serial.print("\nFIFO Table A: \n");
      Serial.print("\tSpecifies an ID to be used as acceptance criteria for the FIFO. In the standard frame\n");
      Serial.print("\tformat, only the 11 most significant bits (29 to 19) are used for frame identification. In\n");
      Serial.print("\tthe extended frame format, all bits are used.\n");
    } // TABLE_A

    else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 1 ) { /* Table B */
      Serial.print("\nFIFO Table B: \n");
      Serial.print("\tSpecifies an ID to be used as acceptance criteria for the FIFO. In the standard frame\n");
      Serial.print("\tformat, the 11 most significant bits (a full standard ID) (29 to 19 and 13 to 3) are used for\n");
      Serial.print("\tframe identification. In the extended frame format, all 14 bits of the field are compared to\n");
      Serial.print("\tthe 14 most significant bits of the received ID.\n");
    } // TABLE_B

    else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 2 ) { /* Table C */
      Serial.print("\nFIFO Table C: \n");
      Serial.print("\tSpecifies an ID to be used as acceptance criteria for the FIFO. In both standard and\n");
      Serial.print("\textended frame formats, all 8 bits of the field are compared to the 8 most significant bits\n");
      Serial.print("\tof the received ID.\n");
    }  // TABLE_C



    for ( uint8_t filter = 0; filter < mailboxOffset(); filter++ ) {

      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////           TABLE A          /////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 0 ) { /* Table A */

        Serial.print("\n  *** Filter[");
        Serial.print(filter);
        Serial.print("] accepted IDs: \n\t\t");

        if ( !filter_set[filter] ) {
          Serial.print("FILTER NOT SET! Currently set to ");
          if ( !masks[filter] ) Serial.print("accept all traffic\n\n");
          else if ( masks[filter] == 0xFFFFFFFF ) Serial.print("reject all traffic\n\n");
          continue;
        }

        uint32_t shifted_mask = (FLEXCANb_IDFLT_TAB(_baseAddress, filter) & (1UL << 30)) ? (masks[filter] >> 1) & 0x1FFFFFFF : (masks[filter] >> 19) & 0x7FF;
        uint32_t canid = filter_enhancement_config[filter][0];
        uint32_t min = ( canid > 0x7FF ) ? canid >> __builtin_clz(shifted_mask) << __builtin_clz(shifted_mask) : canid & (~(shifted_mask) & 0x7FF);
        uint32_t max = ( canid > 0x7FF ) ? canid | (~(shifted_mask) & 0x1FFFFFFF) : canid | (~(shifted_mask) & 0x7FF);

        Serial.print("\tmin search: 0x"); Serial.print(min, HEX);
        Serial.print("\t\t"); Serial.print("max search: 0x");
        Serial.print(max, HEX); Serial.print("\n\n\t\t\t");

        for ( uint32_t i = min, count = 0, list_count = 0; i <= max; i++ ) {
          if ( ( canid & shifted_mask ) == ( i & shifted_mask ) ) {
            if ( !list && list_count++ > 50 ) {
              Serial.print("...");
              break;
            }
            Serial.print("0x"); Serial.print(i, HEX);
            if ( count++ >= 5 ) {
              Serial.print("\n\t\t\t");
              count = 0;
              continue;
            }
            Serial.print("\t");
          }
        }
        Serial.println("\n");

        if ( !filter_enhancement[filter][0] ) Serial.println("\tEnhancement Disabled");
        else Serial.println("\tEnhancement Enabled");

        if ( filter_enhancement[filter][1] ) {
          Serial.print("\t\t* Enhanced ID-range mode filtering:  0x"); /* ID range based */
          Serial.print(filter_enhancement_config[filter][0], HEX);
          Serial.print(" <--> 0x");
          Serial.print(filter_enhancement_config[filter][1], HEX);
        }
        else {
          Serial.print("\t\t* Enhanced Multi-ID mode filtering:  "); /* multi-id based */
          std::sort(&filter_enhancement_config[filter][0], &filter_enhancement_config[filter][5]);
          for ( uint8_t i = 0; i < 5; ) {
            Serial.print("0x");
            Serial.print(filter_enhancement_config[filter][i], HEX);
            Serial.print(" ");
            while ( (i < 4) && (filter_enhancement_config[filter][i] == filter_enhancement_config[filter][i + 1]) ) {
              i++;
              continue;
            }
            i++;
          }
        } Serial.println("\n");


      } // TABLE_A
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////           TABLE B          /////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 1 ) { /* Table B */

        Serial.print("\n  *** Filter[");
        Serial.print(filter);
        Serial.print("] accepted IDs: \n\t\t");

        if ( !filter_set[filter] ) {
          Serial.print("FILTER NOT SET! Currently set to ");
          if ( !masks[filter] ) Serial.print("accept all traffic\n\n");
          else if ( masks[filter] == 0x3FFF3FFF ) Serial.print("reject all traffic\n\n");
          continue;
        }

        uint32_t shifted_mask = (FLEXCANb_IDFLT_TAB(_baseAddress, filter) & (1UL << 30)) ? ((masks[filter] >> 16) & 0x3FFF) << (29 - 14) : (masks[filter] >> 19) & 0x7FF;
        uint32_t canid = filter_enhancement_config[filter][0];
        uint32_t min = ( canid > 0x7FF ) ? canid >> __builtin_clz(shifted_mask) << __builtin_clz(shifted_mask) : canid & (~(shifted_mask) & 0x7FF);
        uint32_t max = ( canid > 0x7FF ) ? canid | (~(shifted_mask) & 0x3FFF) : canid | (~(shifted_mask) & 0x7FF);

        Serial.print("\tmin search: 0x"); Serial.print(min, HEX);
        Serial.print("\t\t"); Serial.print("max search: 0x");
        Serial.print(max, HEX); Serial.print("\n\n\t\t\t");

        for ( uint32_t i = min, count = 0, list_count = 0; i <= max; i++ ) {
          if ( ( canid & shifted_mask ) == ( i & shifted_mask ) ) {
            if ( !list && list_count++ > 50 ) {
              Serial.print("...");
              break;
            }
            Serial.print("0x"); Serial.print(i, HEX);
            if ( count++ >= 5 ) {
              Serial.print("\n\t\t\t");
              count = 0;
              continue;
            }
            Serial.print("\t");
          }
        }
        Serial.println("\n");

        if ( !filter_enhancement[filter][0] ) Serial.println("\tEnhancement Disabled");
        else Serial.println("\tEnhancement Enabled");

        if ( filter_enhancement[filter][1] ) {
          Serial.print("\t\t* Enhanced ID-range mode filtering:  0x"); /* ID range based */
          Serial.print(filter_enhancement_config[filter][0], HEX);
          Serial.print(" <--> 0x");
          Serial.print(filter_enhancement_config[filter][1], HEX);
        }
        else {
          Serial.print("\t\t* Enhanced Multi-ID mode filtering:  "); /* multi-id based */
          Serial.print("0x");
          Serial.print(filter_enhancement_config[filter][0], HEX);
          if ( filter_enhancement_config[filter][0] != filter_enhancement_config[filter][1] ) {
            Serial.print(" 0x");
            Serial.print(filter_enhancement_config[filter][1], HEX);
          }
        } Serial.println("\n");



        shifted_mask = (FLEXCANb_IDFLT_TAB(_baseAddress, filter) & (1UL << 14)) ? ((masks[filter] >> 0) & 0x3FFF) << (29 - 14) : (masks[filter] >> 3) & 0x7FF;
        canid = filter_enhancement_config[filter][2];
        min = ( canid > 0x7FF ) ? canid >> __builtin_clz(shifted_mask) << __builtin_clz(shifted_mask) : canid & (~(shifted_mask) & 0x7FF);
        max = ( canid > 0x7FF ) ? canid | (~(shifted_mask) & 0x3FFF) : canid | (~(shifted_mask) & 0x7FF);

        Serial.print("\t\t\tmin search: 0x"); Serial.print(min, HEX);
        Serial.print("\t\t"); Serial.print("max search: 0x");
        Serial.print(max, HEX); Serial.print("\n\n\t\t\t");

        for ( uint32_t i = min, count = 0, list_count = 0; i <= max; i++ ) {
          if ( ( canid & shifted_mask ) == ( i & shifted_mask ) ) {
            if ( !list && list_count++ > 50 ) {
              Serial.print("...");
              break;
            }
            Serial.print("0x"); Serial.print(i, HEX);
            if ( count++ >= 5 ) {
              Serial.print("\n\t\t\t");
              count = 0;
              continue;
            }
            Serial.print("\t");
          }
        }
        Serial.println("\n");

        if ( !filter_enhancement[filter][0] ) Serial.println("\tEnhancement Disabled");
        else Serial.println("\tEnhancement Enabled");

        if ( filter_enhancement[filter][1] ) {
          Serial.print("\t\t* Enhanced ID-range mode filtering:  0x"); /* ID range based */
          Serial.print(filter_enhancement_config[filter][2], HEX);
          Serial.print(" <--> 0x");
          Serial.print(filter_enhancement_config[filter][3], HEX);
        }
        else {
          Serial.print("\t\t* Enhanced Multi-ID mode filtering:  "); /* multi-id based */
          Serial.print("0x");
          Serial.print(filter_enhancement_config[filter][2], HEX);
          if ( filter_enhancement_config[filter][2] != filter_enhancement_config[filter][3] ) {
            Serial.print(" 0x");
            Serial.print(filter_enhancement_config[filter][3], HEX);
          }
        } Serial.println("\n");
      } // TABLE_B
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////           TABLE C          /////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 2 ) { /* Table C */

        Serial.print("\n  *** Filter[");
        Serial.print(filter);
        Serial.print("] accepted IDs: \n\t\t");

        if ( !filter_set[filter] ) {
          Serial.print("FILTER NOT SET! Currently set to ");
          if ( !masks[filter] ) Serial.print("accept all traffic\n\n");
          else if ( masks[filter] == 0xFFFFFFFF ) Serial.print("reject all traffic\n\n");
          continue;
        }

        uint32_t canid[4] = { filter_enhancement_config[filter][0], filter_enhancement_config[filter][1], filter_enhancement_config[filter][2], filter_enhancement_config[filter][3] };
        for ( uint8_t i = 0; i < 4; i++ ) {
          if ( __builtin_clz(canid[i]) <= 24 ) {
            uint8_t len = (24-__builtin_clz(canid[i]));
            canid[i] >>= len;
            //canid[i] <<= len;
          }
        }

        Serial.print("Due to the broad range of IDs, a table will not be provided.\n\t\tFilter contains these IDs:\n\t\t");
        for ( uint8_t i = 0; i < 4; i++ ) {
           Serial.print("0x"); Serial.print(filter_enhancement_config[filter][i],HEX); Serial.print(" ");
        } Serial.print("\n\t\tThe 8 most significant bits of each:\n\t\t");
        for ( uint8_t i = 0; i < 4; i++ ) {
           Serial.print("0x"); Serial.print(canid[i],HEX); Serial.print(" ");
        } Serial.println();
      } // TABLE_C
    } // FOR_LOOP
    return; // EXIT FIFO CALL
  }
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////           MAILBOX AREA          ////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  Serial.print("\nMailbox: "); Serial.println(mb_num);
  Serial.print("\tIn standard frame format, only the 11 most significant bits (28 to 18) are used for frame\n");
  Serial.print("\tidentification in both receive and transmit cases. The 18 least significant bits are ignored.\n");
  Serial.print("\tIn extended frame format, all bits are used for frame identification in both receive and\n");
  Serial.print("\ttransmit cases.\n");

  Serial.print("\n  *** MB");
  Serial.print(mb_num);
  if ( (FLEXCAN_get_code(FLEXCANb_MBn_CS(_baseAddress, mb_num)) >> 3) ) {
    Serial.println(" is a transmission mailbox.\n");
    return;
  }
  Serial.print(" accepted IDs: \n\t\t");


  if ( !filter_set[mb_num] ) {
    Serial.print("FILTER NOT SET! Currently set to ");
    if ( !masks[mb_num] ) Serial.print("accept all traffic\n\n");
    else if ( masks[mb_num] == 0xFFFFFFFF ) Serial.print("reject all traffic\n\n");
    return;
  }

  uint32_t shifted_mask = (FLEXCANb_MBn_CS(_baseAddress, mb_num) & FLEXCAN_MB_CS_IDE) ? ((masks[mb_num] >> 0) << 11) & 0x1FFFFFFF : (masks[mb_num] >> 18) & 0x7FF;
  uint32_t canid = filter_enhancement_config[mb_num][0];
  uint32_t min = ( canid > 0x7FF ) ? canid >> __builtin_clz(shifted_mask) << __builtin_clz(shifted_mask) : canid & (~(shifted_mask) & 0x7FF);
  uint32_t max = ( canid > 0x7FF ) ? canid | (~(shifted_mask) & 0x3FFF) : canid | (~(shifted_mask) & 0x7FF);

  Serial.print("\tmin search: 0x"); Serial.print(min, HEX);
  Serial.print("\t\t"); Serial.print("max search: 0x");
  Serial.print(max, HEX); Serial.print("\n\n\t\t\t");

  for ( uint32_t i = min, count = 0, list_count = 0; i <= max; i++ ) {
    if ( ( canid & shifted_mask ) == ( i & shifted_mask ) ) {
      if ( !list && list_count++ > 50 ) {
        Serial.print("...");
        break;
      }
      Serial.print("0x"); Serial.print(i, HEX);
      if ( count++ >= 5 ) {
        Serial.print("\n\t\t\t");
        count = 0;
        continue;
      }
      Serial.print("\t");
    }
  }
  Serial.println("\n");

  if ( !filter_enhancement[mb_num][0] ) Serial.println("\tEnhancement Disabled");
  else Serial.println("\tEnhancement Enabled");

  if ( filter_enhancement[mb_num][1] ) {
    Serial.print("\t\t* Enhanced ID-range mode filtering:  0x"); /* ID range based */
    Serial.print(filter_enhancement_config[mb_num][0], HEX);
    Serial.print(" <--> 0x");
    Serial.print(filter_enhancement_config[mb_num][1], HEX);
  }
  else {
    Serial.print("\t\t* Enhanced Multi-ID mode filtering:  "); /* multi-id based */
    std::sort(&filter_enhancement_config[mb_num][0], &filter_enhancement_config[mb_num][5]);
    for ( uint8_t i = 0; i < 5; ) {
      Serial.print("0x");
      Serial.print(filter_enhancement_config[mb_num][i], HEX);
      Serial.print(" ");
      while ( (i < 4) && (filter_enhancement_config[mb_num][i] == filter_enhancement_config[mb_num][i + 1]) ) {
        i++;
        continue;
      }
      i++;
    }
  } Serial.println("\n");
}

































void FlexCAN_T4::setMBFilter(FLEXCAN_FLTEN input) {

  FLEXCAN_EnterFreezeMode();
  for (uint8_t i = 0; i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++ ) {
    filter_enhancement[i][0] = 0;
    filter_enhancement[i][1] = 0;
  }
  for (uint8_t i = mailboxOffset(); i < FLEXCANb_MAXMB_SIZE(_baseAddress); i++) {
    filter_set[i] = 0; /* reset enhancement filters bit set */
    if ( input == ACCEPT_ALL ) FLEXCANb_RXIMR(_baseAddress, i) = 0x00000000; // (RXIMR)
    if ( input == REJECT_ALL ) FLEXCANb_RXIMR(_baseAddress, i) = 0xFFFFFFFF; // (RXIMR)
    masks[i] = FLEXCANb_RXIMR(_baseAddress, i);
    FLEXCANb_MBn_ID(_baseAddress, i) = 0x00000000;
  }
  FLEXCAN_ExitFreezeMode();
}



void FlexCAN_T4::setMBFilter(FLEXCAN_MAILBOX mb_num, FLEXCAN_FLTEN input) {

  if ( mb_num < mailboxOffset() || mb_num >= FLEXCANb_MAXMB_SIZE(_baseAddress) ) return; /* mailbox not available */

  filter_enhancement[mb_num][0] = 0;
  filter_enhancement[mb_num][1] = 0;
  filter_set[mb_num] = 1; /* set filters flag */
  FLEXCAN_EnterFreezeMode();
  if ( input == ACCEPT_ALL ) FLEXCANb_RXIMR(_baseAddress, mb_num) = 0x00000000; // (RXIMR)
  if ( input == REJECT_ALL ) FLEXCANb_RXIMR(_baseAddress, mb_num) = 0xFFFFFFFF; // (RXIMR)
  FLEXCAN_ExitFreezeMode();
  FLEXCANb_MBn_ID(_baseAddress, mb_num) = 0x00000000;
}



bool FlexCAN_T4::setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1) {

  if ( mb_num < mailboxOffset() || mb_num >= FLEXCANb_MAXMB_SIZE(_baseAddress) ) return 0; /* mailbox not available */
  bool extbit = FLEXCANb_MBn_CS(_baseAddress, mb_num) & FLEXCAN_MB_CS_IDE;

  if ( (id1 > 0x7FF) != extbit ) return 0;

  filter_enhancement[mb_num][0] = 0;
  filter_enhancement[mb_num][1] = 0;
  filter_enhancement_config[mb_num][0] = id1;
  filter_enhancement_config[mb_num][1] = id1;
  filter_enhancement_config[mb_num][2] = id1;
  filter_enhancement_config[mb_num][3] = id1;
  filter_enhancement_config[mb_num][4] = id1;
  filter_set[mb_num] = 1; /* set filters flag */

  uint32_t mask = ( !extbit ) ? FLEXCAN_MB_ID_IDSTD(((id1) ^ (id1)) ^ 0x7FF) : FLEXCAN_MB_ID_IDEXT(((id1) ^ (id1)) ^ 0x1FFFFFFF);

  setMBFilterProcessing(mb_num,id1,mask);
  return 1;
}

bool FlexCAN_T4::setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2) {
  if ( mb_num < mailboxOffset() || mb_num >= FLEXCANb_MAXMB_SIZE(_baseAddress) ) return 0; /* mailbox not available */
  bool extbit = FLEXCANb_MBn_CS(_baseAddress, mb_num) & FLEXCAN_MB_CS_IDE;

  if ( (id1 > 0x7FF) != extbit || (id2 > 0x7FF) != extbit ) return 0;

  filter_enhancement[mb_num][0] = 0;
  filter_enhancement[mb_num][1] = 0;
  filter_enhancement_config[mb_num][0] = id1;
  filter_enhancement_config[mb_num][1] = id2;
  filter_enhancement_config[mb_num][2] = id2;
  filter_enhancement_config[mb_num][3] = id2;
  filter_enhancement_config[mb_num][4] = id2;
  filter_set[mb_num] = 1; /* set filters flag */

  uint32_t mask = ( !extbit ) ? FLEXCAN_MB_ID_IDSTD(((id1 | id2) ^ (id1 & id2)) ^ 0x7FF) : FLEXCAN_MB_ID_IDEXT(((id1 | id2) ^ (id1 & id2)) ^ 0x1FFFFFFF);

  setMBFilterProcessing(mb_num,id1,mask);
  return 1;
}

bool FlexCAN_T4::setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2, uint32_t id3) {

  if ( mb_num < mailboxOffset() || mb_num >= FLEXCANb_MAXMB_SIZE(_baseAddress) ) return 0; /* mailbox not available */
  bool extbit = FLEXCANb_MBn_CS(_baseAddress, mb_num) & FLEXCAN_MB_CS_IDE;

  if ( (id1 > 0x7FF) != extbit || (id2 > 0x7FF) != extbit || (id3 > 0x7FF) != extbit ) return 0;

  filter_enhancement[mb_num][0] = 0;
  filter_enhancement[mb_num][1] = 0;
  filter_enhancement_config[mb_num][0] = id1;
  filter_enhancement_config[mb_num][1] = id2;
  filter_enhancement_config[mb_num][2] = id3;
  filter_enhancement_config[mb_num][3] = id3;
  filter_enhancement_config[mb_num][4] = id3;
  filter_set[mb_num] = 1; /* set filters flag */

  uint32_t mask = ( !extbit ) ? FLEXCAN_MB_ID_IDSTD(((id1 | id2 | id3) ^ (id1 & id2 & id3)) ^ 0x7FF) : FLEXCAN_MB_ID_IDEXT(((id1 | id2 | id3) ^ (id1 & id2 & id3)) ^ 0x1FFFFFFF);

  setMBFilterProcessing(mb_num,id1,mask);
  return 1;
}


void FlexCAN_T4::enhanceFilter(FLEXCAN_MAILBOX mb_num) {

  if ( mb_num < mailboxOffset() ) return; /* use "FIFO" instead of "MB(x)" for FIFO region */
  if ( mb_num == FIFO ) {
    for ( uint8_t i = 0; i < mailboxOffset(); i++ ) filter_enhancement[i][0] = 1; /* enhance FIFO filters based on consumed mailboxes (RFFN) */
    return; /* finished FIFO filter enhancements */
  }
  filter_enhancement[mb_num][0] = 1;
}

bool FlexCAN_T4::setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2, uint32_t id3, uint32_t id4) {

  if ( mb_num < mailboxOffset() || mb_num >= FLEXCANb_MAXMB_SIZE(_baseAddress) ) return 0; /* mailbox not available */
  bool extbit = FLEXCANb_MBn_CS(_baseAddress, mb_num) & FLEXCAN_MB_CS_IDE;

  if ( (id1 > 0x7FF) != extbit || (id2 > 0x7FF) != extbit || (id3 > 0x7FF) != extbit || (id4 > 0x7FF) != extbit ) return 0;

  filter_enhancement[mb_num][0] = 0;
  filter_enhancement[mb_num][1] = 0;
  filter_enhancement_config[mb_num][0] = id1;
  filter_enhancement_config[mb_num][1] = id2;
  filter_enhancement_config[mb_num][2] = id3;
  filter_enhancement_config[mb_num][3] = id4;
  filter_enhancement_config[mb_num][4] = id4;
  filter_set[mb_num] = 1; /* set filters flag */

  uint32_t mask = ( !extbit ) ? FLEXCAN_MB_ID_IDSTD(((id1 | id2 | id3 | id4) ^ (id1 & id2 & id3 & id4)) ^ 0x7FF) : FLEXCAN_MB_ID_IDEXT(((id1 | id2 | id3 | id4) ^ (id1 & id2 & id3 & id4)) ^ 0x1FFFFFFF);

  setMBFilterProcessing(mb_num,id1,mask);
  return 1;
}

bool FlexCAN_T4::setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2, uint32_t id3, uint32_t id4, uint32_t id5) {

  if ( mb_num < mailboxOffset() || mb_num >= FLEXCANb_MAXMB_SIZE(_baseAddress) ) return 0; /* mailbox not available */
  bool extbit = FLEXCANb_MBn_CS(_baseAddress, mb_num) & FLEXCAN_MB_CS_IDE;

  if ( (id1 > 0x7FF) != extbit || (id2 > 0x7FF) != extbit || (id3 > 0x7FF) != extbit || (id4 > 0x7FF) != extbit || (id5 > 0x7FF) != extbit ) return 0;

  filter_enhancement[mb_num][0] = 0;
  filter_enhancement[mb_num][1] = 0;
  filter_enhancement_config[mb_num][0] = id1;
  filter_enhancement_config[mb_num][1] = id2;
  filter_enhancement_config[mb_num][2] = id3;
  filter_enhancement_config[mb_num][3] = id4;
  filter_enhancement_config[mb_num][4] = id5;
  filter_set[mb_num] = 1; /* set filters flag */

  uint32_t mask = ( !extbit ) ? FLEXCAN_MB_ID_IDSTD(((id1 | id2 | id3 | id4 | id5) ^ (id1 & id2 & id3 & id4 & id5)) ^ 0x7FF) : FLEXCAN_MB_ID_IDEXT(((id1 | id2 | id3 | id4 | id5) ^ (id1 & id2 & id3 & id4 & id5)) ^ 0x1FFFFFFF);

  setMBFilterProcessing(mb_num,id1,mask);
  return 1;
}

bool FlexCAN_T4::setMBFilterRange(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2) {

  if ( mb_num < mailboxOffset() || mb_num >= FLEXCANb_MAXMB_SIZE(_baseAddress) ) return 0; /* mailbox not available */
  bool extbit = FLEXCANb_MBn_CS(_baseAddress, mb_num) & FLEXCAN_MB_CS_IDE;

  if ( (id1 > 0x7FF) != extbit || (id2 > 0x7FF) != extbit ) return 0;
  if ( id1 > id2 || ((id2 > id1) && (id2-id1>1000)) || !id1 || !id2 ) return 0; /* don't play around... */

  filter_enhancement[mb_num][0] = 0;
  filter_enhancement[mb_num][1] = 1;
  filter_enhancement_config[mb_num][0] = id1;
  filter_enhancement_config[mb_num][1] = id2;
  filter_enhancement_config[mb_num][2] = id2;
  filter_enhancement_config[mb_num][3] = id2;
  filter_enhancement_config[mb_num][4] = id2;
  filter_set[mb_num] = 1; /* set filters flag */


  uint32_t stage1 = id1, stage2 = id1;
  for ( uint32_t i = id1 + 1; i <= id2; i++ ) {
    stage1 |= i; stage2 &= i;
  }

  uint32_t mask = ( !extbit ) ? FLEXCAN_MB_ID_IDSTD( (stage1 ^ stage2) ^ 0x1FFFFFFF ) : FLEXCAN_MB_ID_IDEXT( (stage1 ^ stage2) ^ 0x1FFFFFFF );

  setMBFilterProcessing(mb_num,id1,mask);
  return 1;
}

void FlexCAN_T4::setMBFilterProcessing(FLEXCAN_MAILBOX mb_num, uint32_t filter_id, uint32_t calculated_mask) {
  FLEXCAN_EnterFreezeMode();
  FLEXCANb_RXIMR(_baseAddress, mb_num) = calculated_mask;
  masks[mb_num] = calculated_mask; /* we store updated masks to array, since we can't access it out of FRZ mode */
  FLEXCAN_ExitFreezeMode();
  if (!(FLEXCANb_MBn_CS(_baseAddress, mb_num) & FLEXCAN_MB_CS_IDE)) FLEXCANb_MBn_ID(_baseAddress, mb_num) = FLEXCAN_MB_ID_IDSTD(filter_id);
  else FLEXCANb_MBn_ID(_baseAddress, mb_num) = FLEXCAN_MB_ID_IDEXT(filter_id);
}

void FlexCAN_T4::setFIFOFilter(const FLEXCAN_FLTEN &input) {
  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN )) return; /* FIFO not enabled. */

  uint8_t mailboxes = mailboxOffset();

  FLEXCAN_EnterFreezeMode();
  for (uint8_t i = 0; i < mailboxes; i++) { /* block all ID's so filtering could be applied. */
    filter_set[i] = 0; /* reset enhancement filters bit set all off */
    if ( input == REJECT_ALL ) {

      if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 0 ) { /* If Table A is chosen for FIFO */
        FLEXCANb_IDFLT_TAB(_baseAddress, i) = 0xFFFFFFFF; /* reset id */
        FLEXCANb_RXIMR(_baseAddress, i) = 0xFFFFFFFF; // (RXIMR) /* block all id's */
        masks[i] = 0xFFFFFFFF;
      }

      else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 1 ) { /* If Table B is chosen for FIFO */
        FLEXCANb_IDFLT_TAB(_baseAddress, i) = 0xFFFFFFFF; /* reset id */
        FLEXCANb_RXIMR(_baseAddress, i) = 0x3FFF3FFF; // (RXIMR) /* block all id's */
        masks[i] = 0x3FFF3FFF;
      }

      else if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) == 2 ) { /* If Table C is chosen for FIFO */
        FLEXCANb_IDFLT_TAB(_baseAddress, i) = 0x6E6E6E6E; /* reset id */
        FLEXCANb_RXIMR(_baseAddress, i) = 0xFFFFFFFF; // (RXIMR) /* block all id's */
        masks[i] = 0xFFFFFFFF;
      }

    }
    else if ( input == ACCEPT_ALL ) {
      FLEXCANb_IDFLT_TAB(_baseAddress, i) = 0; /* reset id */
      FLEXCANb_RXIMR(_baseAddress, i) = 0; // (RXIMR) /* allow all id's */
      masks[i] = 0;
    }
  }
  FLEXCAN_ExitFreezeMode();
}



// FIFO TABLE_A SINGLE ID FILTER STD/EXT
bool FlexCAN_T4::setFIFOFilter(uint8_t filter, uint32_t id1, const FLEXCAN_IDE &ide, const FLEXCAN_IDE &remote) {

  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN )) return 0; /* FIFO not enabled. */
  if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) != 0 ) return 0; /* must be TableA to process */
  if ( filter >= mailboxOffset() ) return 0; /* not in FIFO region */

  filter_set[filter] = 1; /* show that it's set */
  for ( uint8_t i = 0; i < mailboxOffset(); i++ ) filter_enhancement[i][0] = 0; /* disable FIFO enhancement */
  filter_enhancement[filter][1] = 0; /* set it as multiid based */
  filter_enhancement_config[filter][0] = id1;
  filter_enhancement_config[filter][1] = id1;
  filter_enhancement_config[filter][2] = id1;
  filter_enhancement_config[filter][3] = id1;
  filter_enhancement_config[filter][4] = id1;

  FLEXCAN_EnterFreezeMode();

  uint32_t mask = ( ide != EXT ) ? ((((id1) ^ (id1)) ^ 0x7FF) << 19 ) | 0xC0000001 : ((((id1) ^ (id1)) ^ 0x3FFFFFFF) << 1 ) | 0xC0000001;

  FLEXCANb_RXIMR(_baseAddress, filter) = mask; // (RXIMR)
  masks[filter] = mask;
  FLEXCANb_IDFLT_TAB(_baseAddress, filter) = ((ide == EXT ? 1 : 0) << 30) | ((remote == RTR ? 1 : 0) << 31) |
      ((ide == EXT ? ((id1 & FLEXCAN_MB_ID_EXT_MASK) << 1) : (FLEXCAN_MB_ID_IDSTD(id1) << 1)));
  FLEXCAN_ExitFreezeMode();
  return 1;
}


// FIFO TABLE_A 2x FILTER STD or EXT
bool FlexCAN_T4::setFIFOFilter(uint8_t filter, uint32_t id1, uint32_t id2, const FLEXCAN_IDE &ide, const FLEXCAN_IDE &remote) {

  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN )) return 0; /* FIFO not enabled. */
  if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) != 0 ) return 0; /* must be TableA to process */
  if ( filter >= mailboxOffset() ) return 0; /* not in FIFO region */

  filter_set[filter] = 1; /* show that it's set */
  for ( uint8_t i = 0; i < mailboxOffset(); i++ ) filter_enhancement[i][0] = 0; /* disable FIFO enhancement */
  filter_enhancement[filter][1] = 0; /* set it as multiid based */
  filter_enhancement_config[filter][0] = id1;
  filter_enhancement_config[filter][1] = id2;
  filter_enhancement_config[filter][2] = id2;
  filter_enhancement_config[filter][3] = id2;
  filter_enhancement_config[filter][4] = id2;

  FLEXCAN_EnterFreezeMode();

  uint32_t mask = ( ide != EXT ) ? ((((id1 | id2) ^ (id1 & id2)) ^ 0x7FF) << 19 ) | 0xC0000001 : ((((id1 | id2) ^ (id1 & id2)) ^ 0x3FFFFFFF) << 1 ) | 0xC0000001;

  FLEXCANb_RXIMR(_baseAddress, filter) = mask; // (RXIMR)
  masks[filter] = mask;
  FLEXCANb_IDFLT_TAB(_baseAddress, filter) = ((ide == EXT ? 1 : 0) << 30) | ((remote == RTR ? 1 : 0) << 31) |
      ((ide == EXT ? ((id1 & FLEXCAN_MB_ID_EXT_MASK) << 1) : (FLEXCAN_MB_ID_IDSTD(id1) << 1)));
  FLEXCAN_ExitFreezeMode();
  return 1;
}


// FIFO TABLE_A FILTER RANGE min -> max, STD or EXT
bool FlexCAN_T4::setFIFOFilterRange(uint8_t filter, uint32_t id1, uint32_t id2, const FLEXCAN_IDE &ide, const FLEXCAN_IDE &remote) {

  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN )) return 0; /* FIFO not enabled. */
  if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) != 0 ) return 0; /* must be TableA to process */
  if ( id1 > id2 || ((id2 > id1) && (id2 - id1 > 1000)) || !id1 || !id2 ) return 0; /* don't play around... */
  if ( filter >= mailboxOffset() ) return 0; /* not in FIFO region */

  filter_set[filter] = 1; /* show that it's set */
  for ( uint8_t i = 0; i < mailboxOffset(); i++ ) filter_enhancement[i][0] = 0; /* disable FIFO enhancement */
  filter_enhancement[filter][1] = 1; /* set it as range based */
  filter_enhancement_config[filter][0] = id1;
  filter_enhancement_config[filter][1] = id2;
  filter_enhancement_config[filter][2] = id2;
  filter_enhancement_config[filter][3] = id2;
  filter_enhancement_config[filter][4] = id2;

  FLEXCAN_EnterFreezeMode();

  uint32_t stage1 = id1, stage2 = id1;
  for ( uint32_t i = id1 + 1; i <= id2; i++ ) {
    stage1 |= i; stage2 &= i;
  }

  uint32_t mask = ( ide != EXT ) ? (((stage1 ^ stage2) ^ 0x7FF) << 19) | 0xC0000001 : (((stage1 ^ stage2) ^ 0x3FFFFFFF) << 1) | 0xC0000001;

  FLEXCANb_RXIMR(_baseAddress, filter) = mask; // (RXIMR)
  masks[filter] = mask;
  FLEXCANb_IDFLT_TAB(_baseAddress, filter) = ((ide == EXT ? 1 : 0) << 30) | ((remote == RTR ? 1 : 0) << 31) |
      ((ide == EXT ? ((id1 & FLEXCAN_MB_ID_EXT_MASK) << 1) : (FLEXCAN_MB_ID_IDSTD(id1) << 1)));
  FLEXCAN_ExitFreezeMode();
  return 1;
}


void FlexCAN_T4::setFIFOFilterTable(FLEXCAN_FIFOTABLE letter) {
  FLEXCAN_EnterFreezeMode();
  FLEXCANb_MCR(_baseAddress) |= FLEXCAN_MCR_IDAM(letter);
  FLEXCAN_ExitFreezeMode();
}


// FIFO TABLE_B DUAL STD/EXT/MIXED ID FILTER
bool FlexCAN_T4::setFIFOFilter(uint8_t filter, uint32_t id1, const FLEXCAN_IDE &ide1, const FLEXCAN_IDE &remote1, uint32_t id2, const FLEXCAN_IDE &ide2, const FLEXCAN_IDE &remote2) {

  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN )) return 0; /* FIFO not enabled. */
  if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) != 1 ) return 0; /* must be TableB to process */
  if ( filter >= mailboxOffset() ) return 0; /* not in FIFO region */

  filter_set[filter] = 1; /* show that it's set */
  for ( uint8_t i = 0; i < mailboxOffset(); i++ ) filter_enhancement[i][0] = 0; /* disable FIFO enhancement */
  filter_enhancement[filter][1] = 0; /* set it as multi id based */
  filter_enhancement_config[filter][0] = id1;
  filter_enhancement_config[filter][1] = id1;
  filter_enhancement_config[filter][2] = id2;
  filter_enhancement_config[filter][3] = id2;
  filter_enhancement_config[filter][4] = id2;

  FLEXCAN_EnterFreezeMode();
  FLEXCANb_IDFLT_TAB(_baseAddress, filter) = ((ide1 == EXT ? 1 : 0) << 30) | ((ide2 == EXT ? 1 : 0) << 14) | /* STD IDs / EXT IDs */
      ((remote1 == RTR ? 1 : 0) << 31) | ((remote2 == RTR ? 1 : 0) << 15) | /* remote frames */
      (ide1 == EXT ? ((id1 >> (29 - 14)) << 16) : ((id1 & 0x7FF) << 19)) | /* first ID is EXT or STD? */
      (ide2 == EXT ? ((id2 >> (29 - 14)) <<  0) : ((id2 & 0x7FF) <<  3)) ; /* second ID is EXT or STD? */

  uint32_t mask = ( ide1 != EXT ) ? ((((id1) ^ (id1)) ^ 0x7FF) << 19 ) | 0xC0070000 : ((((id1) ^ (id1)) ^ 0x3FFF) << 16 | 0xC0000000 );
  ( ide2 != EXT ) ? mask |= ((((id2) ^ (id2)) ^ 0x7FF) << 3 ) | 0xC007 : mask |= ((((id2) ^ (id2)) ^ 0x3FFF) << 0 | 0xC000);

  FLEXCANb_RXIMR(_baseAddress, filter) = mask; // (RXIMR)
  masks[filter] = mask;
  FLEXCAN_ExitFreezeMode();
  return 1;
}


// FIFO TABLE_B QUAD STD/14MSB EXT/MIXED ID FILTER
bool FlexCAN_T4::setFIFOFilter(uint8_t filter, uint32_t id1, uint32_t id2, const FLEXCAN_IDE &ide1, const FLEXCAN_IDE &remote1, uint32_t id3, uint32_t id4, const FLEXCAN_IDE &ide2, const FLEXCAN_IDE &remote2) {

  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN )) return 0; /* FIFO not enabled. */
  if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) != 1 ) return 0; /* must be TableB to process */
  if ( filter >= mailboxOffset() ) return 0; /* not in FIFO region */

  filter_set[filter] = 1; /* show that it's set */
  for ( uint8_t i = 0; i < mailboxOffset(); i++ ) filter_enhancement[i][0] = 0; /* disable FIFO enhancement */
  filter_enhancement[filter][1] = 0; /* set it as multi id based */
  filter_enhancement_config[filter][0] = id1;
  filter_enhancement_config[filter][1] = id2;
  filter_enhancement_config[filter][2] = id3;
  filter_enhancement_config[filter][3] = id4;
  filter_enhancement_config[filter][4] = id4;

  FLEXCAN_EnterFreezeMode();

  uint32_t mask = ( ide1 != EXT ) ? ((((id1 | id2) ^ (id1 & id2)) ^ 0x7FF) << 19 ) | 0xC0070000 : ((((id1 | id2) ^ (id1 & id2)) ^ 0x3FFF) << 16 ) | 0xC0000000;
  mask |= ( ide2 != EXT ) ? ((((id3 | id4) ^ (id3 & id4)) ^ 0x7FF) << 3 ) | 0xC007 : ((((id3 | id4) ^ (id3 & id4)) ^ 0x3FFF) << 0 ) | 0xC000;

  FLEXCANb_IDFLT_TAB(_baseAddress, filter) = ((ide1 == EXT ? 1 : 0) << 30) | ((ide2 == EXT ? 1 : 0) << 14) | /* STD IDs / EXT IDs */
      ((remote1 == RTR ? 1 : 0) << 31) | ((remote2 == RTR ? 1 : 0) << 15) | /* remote frames */

      (ide1 == EXT ? ((id1 >> (29 - 14)) << 16) : ((id1 & 0x7FF) << 19)) | /* first ID is EXT or STD? */
      (ide2 == EXT ? ((id3 >> (29 - 14)) << 0 ) : ((id3 & 0x7FF) << 3 ))  ; /* second ID is EXT or STD? */

  FLEXCANb_RXIMR(_baseAddress, filter) = mask; // (RXIMR)
  masks[filter] = mask;
  FLEXCAN_ExitFreezeMode();
  return 1;
}


// FIFO TABLE_B DUAL RANGE STD/EXT/MIXED ID FILTER 
bool FlexCAN_T4::setFIFOFilterRange(uint8_t filter, uint32_t id1, uint32_t id2, const FLEXCAN_IDE &ide1, const FLEXCAN_IDE &remote1, uint32_t id3, uint32_t id4, const FLEXCAN_IDE &ide2, const FLEXCAN_IDE &remote2) {

  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN )) return 0; /* FIFO not enabled. */
  if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) != 1 ) return 0; /* must be TableB to process */
  if ( filter >= mailboxOffset() ) return 0; /* not in FIFO region */
  if ( id1 > id2 || ((id2 > id1) && (id2 - id1 > 1000)) || !id1 || !id2 ) return 0; /* don't play around... */
  if ( id3 > id4 || ((id4 > id3) && (id4 - id3 > 1000)) || !id3 || !id4 ) return 0; /* don't play around... */

  filter_set[filter] = 1; /* show that it's set */
  for ( uint8_t i = 0; i < mailboxOffset(); i++ ) filter_enhancement[i][0] = 0; /* disable FIFO enhancement */
  filter_enhancement[filter][1] = 1; /* set it as range based */
  filter_enhancement_config[filter][0] = id1;
  filter_enhancement_config[filter][1] = id2;
  filter_enhancement_config[filter][2] = id3;
  filter_enhancement_config[filter][3] = id4;
  filter_enhancement_config[filter][4] = id4;

  FLEXCAN_EnterFreezeMode();

  uint32_t stage1 = id1, stage2 = id1;

  for ( uint32_t i = id1 + 1; i <= id2; i++ ) {
    stage1 |= i; stage2 &= i;
  }

  uint32_t mask = ( ide1 != EXT ) ? (((stage1 ^ stage2) ^ 0x7FF) << 19) | 0xC0070000 : (((stage1 ^ stage2) ^ 0x3FFF) << 16) | 0xC0000000;

  stage1 = stage2 = id3;

  for ( uint32_t i = id3 + 1; i <= id4; i++ ) {
    stage1 |= i; stage2 &= i;
  }

  ( ide2 != EXT ) ? mask |= (((stage1 ^ stage2) ^ 0x7FF) << 3) | 0xC007 : mask |= (((stage1 ^ stage2) ^ 0x3FFF) << 0) | 0xC000;

  FLEXCANb_IDFLT_TAB(_baseAddress, filter) = ((ide1 == EXT ? 1 : 0) << 30) | ((ide2 == EXT ? 1 : 0) << 14) | /* STD IDs / EXT IDs */
      ((remote1 == RTR ? 1 : 0) << 31) | ((remote2 == RTR ? 1 : 0) << 15) | /* remote frames */
      (ide1 == EXT ? ((id1 >> (29 - 14)) << 16) : ((id1 & 0x7FF) << 19)) | /* first ID is EXT or STD? */
      (ide2 == EXT ? ((id3 >> (29 - 14)) << 0 ) : ((id3 & 0x7FF) << 3 ))  ; /* second ID is EXT or STD? */
  FLEXCANb_RXIMR(_baseAddress, filter) = mask; // (RXIMR)
  masks[filter] = mask;
  FLEXCAN_ExitFreezeMode();
  return 1;
}

// FIFO TABLE_C QUAD ID EXT/STD, 8MSBs each.
bool FlexCAN_T4::setFIFOFilter(uint8_t filter, uint32_t id1, uint32_t id2, uint32_t id3, uint32_t id4 ) {

  if ( !(FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_FEN )) return 0; /* FIFO not enabled. */
  if ( ((FLEXCANb_MCR(_baseAddress) & FLEXCAN_MCR_IDAM_MASK) >> FLEXCAN_MCR_IDAM_BIT_NO) != 2 ) return 0; /* must be TableC to process */
  if ( filter >= mailboxOffset() ) return 0; /* not in FIFO region */

  filter_set[filter] = 1; /* show that it's set */
  for ( uint8_t i = 0; i < mailboxOffset(); i++ ) filter_enhancement[i][0] = 0; /* disable FIFO enhancement */
  filter_enhancement[filter][1] = 1; /* set it as range based */
  filter_enhancement_config[filter][0] = id1;
  filter_enhancement_config[filter][1] = id2;
  filter_enhancement_config[filter][2] = id3;
  filter_enhancement_config[filter][3] = id4;
  filter_enhancement_config[filter][4] = id4;

  FLEXCAN_EnterFreezeMode();
  FLEXCANb_IDFLT_TAB(_baseAddress, filter) =
    (( id1 > 0x7FF ) ? ((id1 >> (29 - 8)) << 24) : ((id1 >> (11 - 8)) << 24)) |
    (( id2 > 0x7FF ) ? ((id2 >> (29 - 8)) << 16) : ((id2 >> (11 - 8)) << 16)) |
    (( id3 > 0x7FF ) ? ((id3 >> (29 - 8)) << 8) :  ((id3 >> (11 - 8)) <<  8)) |
    (( id4 > 0x7FF ) ? ((id4 >> (29 - 8)) << 0) :  ((id4 >> (11 - 8)) <<  0));

  uint32_t mask = ( id1 > 0x7FF ) ? (id1>>(29-8)) << 24 : (id1>>(11-8)) << 24;
  mask |= ( id2 > 0x7FF ) ? (id2>>(29-8)) << 16 : (id2>>(11-8)) << 16;
  mask |= ( id3 > 0x7FF ) ? (id3>>(29-8)) << 8  : (id3>>(11-8)) <<  8;
  mask |= ( id4 > 0x7FF ) ? (id4>>(29-8)) << 0  : (id4>>(11-8)) <<  0;

  FLEXCANb_RXIMR(_baseAddress, filter) = mask; // (RXIMR)
  masks[filter] = mask;
  FLEXCAN_ExitFreezeMode();
  return 1;
}
