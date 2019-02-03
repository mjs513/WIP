#if !defined(_FLEXCAN_T4_H_)
#define _FLEXCAN_T4_H_

#include "Arduino.h"
#include "circular_buffer.h"
#include "imxrt_flexcan.h"

#define FLEXCAN_RX_BUFFER_SIZE 16
#define FLEXCAN_TX1_BUFFER_SIZE 128
#define FLEXCAN_TX2_BUFFER_SIZE 16

typedef struct CAN_message_t {
  uint32_t id = 0;          // can identifier
  uint16_t timestamp = 0;   // FlexCAN time when message arrived
  struct {
    bool extended = 0; // identifier is extended (29-bit)
    bool remote = 0;  // remote transmission request packet type
    bool overrun = 0; // message overrun
    bool reserved = 0;
  } flags;
  uint8_t len = 8;      // length of data
  uint8_t buf[8] = { 0 };       // data
  uint8_t mb = 0;       // used to identify mailbox reception
  uint8_t bus = 0;      // used to identify where the message came from when events() is used.
  bool seq = 0;         // sequential frames
} CAN_message_t;

typedef void (*_MB_ptr)(const CAN_message_t &msg); /* mailbox / global callbacks */

typedef enum FLEXCAN_PINS {
  ALT = 0,
  DEF = 1,
} FLEXCAN_PINS;

typedef enum FLEXCAN_MAILBOX {
  MB0 = 0,
  MB1 = 1,
  MB2 = 2,
  MB3 = 3,
  MB4 = 4,
  MB5 = 5,
  MB6 = 6,
  MB7 = 7,
  MB8 = 8,
  MB9 = 9,
  MB10 = 10,
  MB11 = 11,
  MB12 = 12,
  MB13 = 13,
  MB14 = 14,
  MB15 = 15,
  MB16 = 16,
  MB17 = 17,
  MB18 = 18,
  MB19 = 19,
  MB20 = 20,
  MB21 = 21,
  MB22 = 22,
  MB23 = 23,
  MB24 = 24,
  MB25 = 25,
  MB26 = 26,
  MB27 = 27,
  MB28 = 28,
  MB29 = 29,
  MB30 = 30,
  MB31 = 31,
  MB32 = 32,
  MB33 = 33,
  MB34 = 34,
  MB35 = 35,
  MB36 = 36,
  MB37 = 37,
  MB38 = 38,
  MB39 = 39,
  MB40 = 40,
  MB41 = 41,
  MB42 = 42,
  MB43 = 43,
  MB44 = 44,
  MB45 = 45,
  MB46 = 46,
  MB47 = 47,
  MB48 = 48,
  MB49 = 49,
  MB50 = 50,
  MB51 = 51,
  MB52 = 52,
  MB53 = 53,
  MB54 = 54,
  MB55 = 55,
  MB56 = 56,
  MB57 = 57,
  MB58 = 58,
  MB59 = 59,
  MB60 = 60,
  MB61 = 61,
  MB62 = 62,
  MB63 = 63,
  FIFO = 64
} FLEXCAN_MAILBOX;

typedef enum FLEXCAN_RXTX {
  TX,
  RX,
  DISABLE
} FLEXCAN_RXTX;

typedef enum FLEXCAN_IDE {
  NONE = 0,
  EXT = 1,
  RTR = 2,
  STD = 3
} FLEXCAN_IDE;

typedef enum FLEXCAN_FLTEN {
  ACCEPT_ALL = 0,
  REJECT_ALL = 1
} FLEXCAN_FLTEN;

typedef enum FLEXCAN_FIFOTABLE {
  A = 0,
  B = 1,
  C = 2,
} FLEXCAN_FIFOTABLE;


class FlexCAN_T4 {
  public:
    FlexCAN_T4(uint32_t base);
    void FLEXCAN_ExitFreezeMode();
    void FLEXCAN_EnterFreezeMode();
    void setTX(FLEXCAN_PINS select = DEF);
    void setRX(FLEXCAN_PINS select = DEF);
    bool setMB(const FLEXCAN_MAILBOX &mb_num, const FLEXCAN_RXTX &mb_rx_tx, const FLEXCAN_IDE &ide = STD);
    void softReset();
    void setMaxMB(uint8_t last);
    void enableFIFO(bool status = 1);
    void disableFIFO() { enableFIFO(0); }
    uint8_t mailboxOffset();
    void mailboxStatus();
    void setMRP(bool mrp = 1); /* mailbox(1)/fifo(0) priority */
    void setRRS(bool rrs = 1); /* store remote frames */
    static Circular_Buffer<uint8_t, FLEXCAN_RX_BUFFER_SIZE, sizeof(CAN_message_t)> flexcanRxBuffer; /* create an array buffer of struct size, 16 levels deep. */
    static Circular_Buffer<uint8_t, FLEXCAN_TX1_BUFFER_SIZE, sizeof(CAN_message_t)> flexcanTx1Buffer; /* create an array buffer of struct size, 16 levels deep. */
    static Circular_Buffer<uint8_t, FLEXCAN_TX2_BUFFER_SIZE, sizeof(CAN_message_t)> flexcanTx2Buffer; /* create an array buffer of struct size, 16 levels deep. */
    void enableFIFOInterrupt(bool status = 1);
    void disableFIFOInterrupt() { enableFIFOInterrupt(0); }
    void enableMBInterrupt(const FLEXCAN_MAILBOX &mb_num, bool status = 1);
    void disableMBInterrupt(const FLEXCAN_MAILBOX &mb_num) { enableMBInterrupt(mb_num, 0); }
    void FlexCAN_T4_message_ISR(void);
    void softResetRestore(); // copy and restore affected registers after reset
    void setBaudRate(uint32_t baud = 1000000);
    uint32_t getBaudRate() { return currentBitrate; }
    static _MB_ptr _CAN1MBhandlers[64]; /* individual mailbox handlers for CAN1 */
    static _MB_ptr _CAN1GLOBALhandler; /* global mailbox handler for CAN1 */
    static _MB_ptr _CAN2MBhandlers[64]; /* individual mailbox handlers for CAN2 */
    static _MB_ptr _CAN2GLOBALhandler; /* global mailbox handler for CAN2 */
    void onReceive(const FLEXCAN_MAILBOX &mb_num, _MB_ptr handler); /* individual mailbox callback function */
    void onReceive(_MB_ptr handler); /* global callback function */
    void acceptedIDs(const FLEXCAN_MAILBOX &mb_num, bool list = 0);
    int getFirstTxBox();
    int write(const CAN_message_t &msg); /* use any available mailbox for transmitting */
    int write(FLEXCAN_MAILBOX mb_num, const CAN_message_t &msg); /* use a single mailbox for transmitting */
    void currentMasks(); /* lists current set masks between FIFO and MBs */
    static void struct2queueTx(const CAN_message_t &msg);
    int read(CAN_message_t &msg);
    int readMB(CAN_message_t &msg);
    int readFIFO(CAN_message_t &msg);
    static uint16_t events();
    void setMBFilter(FLEXCAN_FLTEN input); /* enable/disable traffic for all MBs (for individual masking) */
    void setMBFilter(FLEXCAN_MAILBOX mb_num, FLEXCAN_FLTEN input); /* set specific MB to accept/deny traffic */
    bool setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1); /* input 1 ID to be filtered */
    bool setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2); /* input 2 ID's to be filtered */
    bool setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2, uint32_t id3); /* input 3 ID's to be filtered */
    bool setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2, uint32_t id3, uint32_t id4); /* input 4 ID's to be filtered */
    bool setMBFilter(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2, uint32_t id3, uint32_t id4, uint32_t id5); /* input 5 ID's to be filtered */
    bool setMBFilterRange(FLEXCAN_MAILBOX mb_num, uint32_t id1, uint32_t id2); /* filter a range of ids */
    void enhanceFilter(FLEXCAN_MAILBOX mb_num);
    void setFIFOFilter(const FLEXCAN_FLTEN &input);
    bool setFIFOFilter(uint8_t filter, uint32_t id1, const FLEXCAN_IDE &ide, const FLEXCAN_IDE &remote = NONE); /* single ID per filter */
    bool setFIFOFilter(uint8_t filter, uint32_t id1, uint32_t id2, const FLEXCAN_IDE &ide, const FLEXCAN_IDE &remote = NONE); /* 2 ID's per filter */
    bool setFIFOFilterRange(uint8_t filter, uint32_t id1, uint32_t id2, const FLEXCAN_IDE &ide, const FLEXCAN_IDE &remote = NONE); /* ID range per filter */
    void setFIFOFilterTable(FLEXCAN_FIFOTABLE letter);
    bool setFIFOFilter(uint8_t filter, uint32_t id1, const FLEXCAN_IDE &ide1, const FLEXCAN_IDE &remote1, uint32_t id2, const FLEXCAN_IDE &ide2, const FLEXCAN_IDE &remote2); /* TableB 2 ID / filter */
    bool setFIFOFilter(uint8_t filter, uint32_t id1, uint32_t id2, const FLEXCAN_IDE &ide1, const FLEXCAN_IDE &remote1, uint32_t id3, uint32_t id4, const FLEXCAN_IDE &ide2, const FLEXCAN_IDE &remote2); /* TableB 4 minimum ID / filter */
    bool setFIFOFilterRange(uint8_t filter, uint32_t id1, uint32_t id2, const FLEXCAN_IDE &ide1, const FLEXCAN_IDE &remote1, uint32_t id3, uint32_t id4, const FLEXCAN_IDE &ide2, const FLEXCAN_IDE &remote2); /* TableB dual range based IDs */
    void setRFFN(uint8_t rffn); /* Number Of Rx FIFO Filters (0 == 8 filters, 1 == 16 filters, etc.. */
    bool setFIFOFilter(uint8_t filter, uint32_t id1, uint32_t id2, uint32_t id3, uint32_t id4 ); /* TableC 4 partial IDs per filter */
    void reset() { softResetRestore(); } /* reset flexcan controller while retaining configuration */
    uint8_t getNumMailBoxes() { return FLEXCANb_MAXMB_SIZE(_baseAddress); }
    uint8_t getLastTxBox() { return FLEXCANb_MAXMB_SIZE(_baseAddress) -1; }

  private:
    void setMBFilterProcessing(FLEXCAN_MAILBOX mb_num, uint32_t filter_id, uint32_t calculated_mask);
    uint32_t _baseAddress = 0;
    uint32_t NVIC_IRQ = 0UL;
    uint32_t currentBitrate = 0UL;
    bool filter_enhancement[64][2] = { { 0 } , { 0 } }; /* enhancement feature, first being enable bit, second being multiID or range based. */
    uint32_t filter_enhancement_config[64][5] = { { 0 } , { 0 } }; /* storage for filter IDs */
    bool filter_set[64] = { 0 };
    uint32_t masks[64]; /* storage for masks, since we can't read/write the register if not in freeze mode */
    static bool can_events;
    void writeTxMailbox(uint8_t mb_num, const CAN_message_t &msg);
    uint8_t mailbox_reader_increment = 0;
    static void struct2queue(const CAN_message_t &msg);
    static void queue2struct(CAN_message_t &msg);
    bool msg_distribution = 0;
    static bool one_process;
    void packet_distribution(CAN_message_t &frame);

};

extern FlexCAN_T4 Can1;
extern FlexCAN_T4 Can2;
extern void ext_output(const CAN_message_t &msg);
extern uint16_t ext_events();

#endif
