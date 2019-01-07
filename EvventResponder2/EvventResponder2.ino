#include <EventResponder.h>
#include <Arduino.h>

const int LED = 13;

volatile int blinkcode = 0;

#define EVENT2_TIME 6000
#define EVENT3_TIME 1050
#define EVENT4_TIME 250
#define EVENT0_TIME 750

uint32_t event0_expected_time;
uint32_t event2_expected_time;
uint32_t event3_expected_time;
uint32_t event4_expected_time;

EventResponder myevent;
EventResponder mytimerevent;
EventResponder mytimerevent2;
EventResponder mytimerevent3;
EventResponder mytimerevent4;

MillisTimer mytimer;
MillisTimer mytimer3;
MillisTimer mytimer2;
MillisTimer mytimer4;


void setup() {
  while (!Serial) ;
  Serial.println("EventResponder test");
  
  myevent.attach(dosomething);
  mytimerevent.attach(dotimer);
  mytimerevent2.attach(dotimer2);
  mytimerevent3.attach(dotimer3);
  mytimerevent4.attach(dotimer4);
  
  event0_expected_time = millis() +  EVENT0_TIME;
  mytimer2.beginRepeating( EVENT2_TIME, mytimerevent2);
  event2_expected_time = millis() +  EVENT2_TIME;
  mytimer3.beginRepeating(EVENT3_TIME, mytimerevent3);
  event3_expected_time = millis() +  EVENT3_TIME;
  mytimer4.beginRepeating(EVENT4_TIME, mytimerevent4);
  event4_expected_time = millis() +  EVENT4_TIME;

  delay(1000);
  
}


void loop() {

  Serial.println("loop trigger dosomething");
  myevent.triggerEvent();
  delay(249);

}

void dosomething(EventResponderRef event)
{
  Serial.println("dosomething event");
  mytimer.begin(EVENT0_TIME, mytimerevent);
  event0_expected_time = millis() +  EVENT0_TIME;
}

void dotimer(EventResponderRef event)
{
  uint32_t t = millis();
  Serial.printf("%u - T0(%d)\n", t, (t >= event0_expected_time)? t - event0_expected_time : event0_expected_time - t);
  event0_expected_time  += EVENT0_TIME;}

void dotimer3(EventResponderRef event)
{
  blinkcode = 3;
  uint32_t t = millis();
  Serial.printf("%u - T3(%d)\n", t, (t >= event3_expected_time)? t - event3_expected_time : event3_expected_time - t);
  event3_expected_time  += EVENT3_TIME;
  //mytimerevent3.detach();
}

void dotimer4(EventResponderRef event)
{
  uint32_t t = millis();
  Serial.printf("%u - T4(%d)\n", t, (t >= event4_expected_time)? t - event4_expected_time : event4_expected_time - t);
  event4_expected_time  += EVENT4_TIME;
}

void dotimer2(EventResponderRef event)
{
  blinkcode = 0;
  uint32_t t = millis();
  Serial.printf("%u - T2(%d)\n", t, (t >= event2_expected_time)? t - event2_expected_time : event2_expected_time - t);
  event2_expected_time  += EVENT2_TIME;
}
