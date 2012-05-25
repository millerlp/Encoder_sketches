/*
  This code uses software-based pin-change interrupts.
  This is a minimal example, to let you turn the motor
  shaft by hand and watch the count change. If the motor
  spins fast enough, even by hand, it will miss some 
  counts.
*/

//#include <mySoftwareSerial.h>
//#include <PololuQik.h>
#include <myPololuWheelEncoders.h>

/* 

There is normally a conflict between
SoftwareSerial and PololuWheelEncoders both trying to define
pin interrupt routines on the same sets of pins. The solution is
to modify one or both libraries to use different ports(B,C,D).
See 
http://code.google.com/p/arduino-pinchangeint/issues/detail?id=7
http://baldwisdom.com/we-interrupt-our-regular-programming/
*/

PololuWheelEncoders encoder;

long time;


void setup() {
  //  encoderInit(); 255 refers to non-existing pins
  // Requires two pins per encoder, here on pins 2,3
  encoder.init(2,4,255,255);

  Serial.begin(115200);

  Serial.print("test ardubot...");
  Serial.println();
  delay(3000);
}


void loop() {
  time = millis();
  while (1) {
    // Sit in a loop and print encoder count every second
    if (millis() > (time + 1000)) {
      Serial.println(encoder.getCountsM1());
      time = millis();
    }
  }

}

