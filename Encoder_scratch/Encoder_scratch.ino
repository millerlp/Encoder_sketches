/*
  This code uses software-based pin-change interrupts.
  It appears to miss a variable number of interrupts 
  every time the motor runs. 
*/

#include <mySoftwareSerial.h>
#include <PololuQik.h>
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

/*
Required connections between Arduino and qik 2s9v1:

      Arduino    qik 2s9v1
           5V -> VCC
          GND -> GND
Digital Pin 8 -> TX pin on 2s9v1 (optional if you don't need talk-back from the unit) 
Digital Pin 9 -> RX pin on 2s9v1
Digital Pin 10 -> RESET
*/
PololuQik2s9v1 qik(8, 9, 10);

PololuWheelEncoders encoder;

long Total = 0;  // Total turns during this actuation
long TotalTurns = 0; // Total turns overall (i.e. current position)

int stepVal = 3200;  // 64 counts per motor rev, 3200 per output shaft rev
int backVal = 0;
const int motorSpeed = 40;

void setup() {
  
  // Initialize qik 2s9v1 serial motor controller
  qik.init();
  
  //  encoderInit(); 255 refers to non-existing pins
  // Requires two pins per encoder, here on pins 2,3
  encoder.init(2,4,255,255);

  Serial.begin(115200);

  Serial.print("test ardubot...");
  Serial.println();
  delay(4000);
}



void loop() {

  qik.setM0Speed(motorSpeed);
  while (encoder.getCountsM1() < stepVal) {
      ;
  }
  qik.setM0Speed(0);
  Serial.print("Target ticks: ");
  Serial.print(stepVal);
  Serial.print(" = ");
  Serial.print(stepVal * 0.1125); // each step = 0.1125 deg on output shaft
  Serial.println(" degrees");
  Serial.print("Encoded ticks: ");
  Serial.print(encoder.getCountsM1());
  Serial.print(" = ");
  Serial.print(encoder.getCountsM1() * 0.1125);
  Serial.print(" degrees");
  Serial.println();
  delay(3000);
  
  
  //****************************************
  // Reverse direction

  qik.setM0Speed((motorSpeed * -1));
  while (encoder.getCountsM1() > backVal) {
    ;
  }
  qik.setM0Speed(0);
  Serial.print("Target reverse ticks: ");
  Serial.print(backVal);
  Serial.print(" = ");
  Serial.print(backVal * 0.1125);  // each step = 0.1125 deg on output shaft
  Serial.println(" degrees");
  Serial.print("Encoded reverse ticks: ");
  Serial.print(encoder.getCountsM1());
  Serial.print(" = ");
  Serial.print(encoder.getCountsM1() * 0.1125);
  Serial.print(" degrees");
  Serial.println();
  delay(3000); 
  
}

