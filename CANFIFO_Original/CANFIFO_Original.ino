#include <IFCT.h>

IFCT CAN = IFCT();

void setup() {
  Serial.begin(115200);
  delay(1000);
  //pinMode(2, OUTPUT); digitalWrite(2, 0);
  Can0.setBaudRate(1000000);
  Can0.setMB(MB9, TX);  //add ,IDE for extended frames
  
  Can0.enableFIFO(1); // turn on fifo
  Can0.enableFIFOInterrupt(0); //  <---- interrupt off, we use pollFIFO // FIFO -------------->>>>>>>>
  Can0.onReceive(myMSG);
  Can0.onReceive(MB8, MB9cb);
}
// the "slave" transmitter is broadcasting 5 STD IDs and 1 EXT ID every second ------------------>>>>

void loop() {
  CAN_message_t msg; // setup a temporary storage buffer

  static uint32_t _timer3 = millis();
  if ( millis() - _timer3 > 10 ) {
    while ( FLEXCAN0_IFLAG1 & FLEXCAN_IMASK1_BUF5M && FLEXCAN0_MCR & FLEXCAN_MCR_FEN ) {
      Can0.pollFIFO(msg, 0); // <--- here, 1 FIFO interrupts will be disabled (if enabled) and restored when overload is 0.
      Serial.print("  LEN: "); Serial.print(msg.len);
      Serial.print(" EXT: "); Serial.print(msg.flags.extended);
      Serial.print(" REMOTE: "); Serial.print(msg.rtr);
      Serial.print(" TS: "); Serial.print(msg.timestamp);
      Serial.print(" ID: "); Serial.print(msg.id, HEX);
      Serial.print(" Buffer: ");
      for ( uint8_t i = 0; i < msg.len; i++ ) {
        Serial.print(msg.buf[i], HEX); Serial.print(" ");
      } Serial.println();
    }
    _timer3 = millis();
  }

  //  Serial.println(FLEXCAN0_IMASK1);


  delay(20);


  msg.flags.extended = 0;
  msg.flags.remote = 0;
  msg.len = 8;
  msg.id = 0x321;
  msg.buf[0] = 0;
  msg.buf[1] = 1;
  msg.buf[2] = 2;
  msg.buf[3] = 3;
  msg.buf[4] = 4;
  msg.buf[5] = 1;
  msg.buf[6] = 2;
  msg.buf[7] = 3;
  Can0.write(msg, MB9);
  //Can0.write(msg);
  // here we sending can frames randomly <---------

delay(500);
  msg.flags.extended = 0;
  msg.flags.remote = 0;
  msg.len = 8;
  msg.id = 0x123;
  msg.buf[0] = 0;
  msg.buf[1] = 1;
  msg.buf[2] = 2;
  msg.buf[3] = 3;
  msg.buf[4] = 4;
  msg.buf[5] = 1;
  msg.buf[6] = 2;
  msg.buf[7] = 3;
  Can0.write(msg, MB9);

  //      here we read frames directly, without fifo, or callbacks:
//  if (Can0.read(msg)) {
//    Serial.print("  LEN: "); Serial.print(msg.len);
//    Serial.print(" EXT: "); Serial.print(msg.flags.extended);
//    Serial.print(" REMOTE: "); Serial.print(msg.rtr);
//    Serial.print(" TS: "); Serial.print(msg.timestamp);
//    Serial.print(" ID: "); Serial.print(msg.id);
//    Serial.print(" Buffer: ");
//    for ( uint8_t i = 0; i < msg.len; i++ ) {
//      Serial.print(msg.buf[i], HEX); Serial.print(" ");
//    } Serial.println();
//  }

  static uint32_t _timer = millis();
  if ( millis() - _timer > 4000 ) {
    pinMode(13, OUTPUT); digitalWrite(13, !digitalRead(13));
    Serial.print("*HEARTBEAT* "); Serial.println(millis());
    //
    //    Can0.setMB(MB5, RX, IDE);
    //    Can0.setMB(MB4, RX, IDE);
    //    Can0.setMB(MB4, RX);
    //    Can0.setMB(MB5, RX);
    //    Can0.setMB(MB6, RX); // <---- use this to switch/setup mailbox custom.
    //    Can0.setMB(MB7, RX);
    //    Can0.enableMBInterrupt(MB0);
//            Can0.enableMBInterrupt(MB1);
    static bool moo = 1;
    if ( moo ) {
            Can0.setMB(MB8, RX);
            //Can0.setMB(MB9, RX);
//            Can0.enableMBInterrupt(MB2);
            Can0.enableMBInterrupt(MB8); // <--- enable per mailbox interrupt
      //      moo = 0;
    }
//        Can0.enableMBInterrupt(MB4);
    //    Can0.enableMBInterrupt(MB5);
    //    //    Can0.enableMBInterrupt(MB6);
    //    Can0.enableMBInterrupt(MB7);
    _timer = millis();
    //    if ( FLEXCAN0_MCR & FLEXCAN_MCR_FEN ) FLEXCAN0_IMASK1 = 0b00100000;
    //    else FLEXCAN0_IMASK1 = 0x255;
//    Can0.mailboxStatus();
  }
}



void MB9cb(const CAN_message_t &msg) { // <--- mailbox 1 callback
  Serial.print("*** MB "); Serial.print(msg.mb);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" REMOTE: "); Serial.print(msg.rtr);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}




void myMSG(const CAN_message_t &msg) { // global callback
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" REMOTE: "); Serial.print(msg.rtr);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}
