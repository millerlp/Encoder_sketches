/* This version uses external interrupts.
*/
#include "Arduino.h"
#include <digitalWriteFast.h>

#include <mySoftwareSerial.h>
#include <PololuQik.h>

#define c_EncoderPinA 2     // digital pin 2
#define c_EncoderPinB 4    // digital pin 4 on Ard328

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

int counts = 0;

void setup() {
  Serial.begin(115200);
    //Quadrature encoders
  pinMode(c_EncoderPinA, INPUT);  // sets encoder pin A as input
  digitalWrite(c_EncoderPinA, LOW);  // turn on pullup resistor
  pinMode(c_EncoderPinB, INPUT);  // sets encoder pin B as input
  digitalWrite(c_EncoderPinB, LOW); // turn on pullup resistor
  // The value in parentheses is the serial comm speed
  // for the 2s9v1 controller
  qik.init(38400);
  delay(2000);
}

void loop() {
  // Turn motor 0 on forward
  qik.setM0Speed(40);
  while (counts < 800){
    while (digitalReadFast(c_EncoderPinA) == HIGH) 
      ;    // wait for encoder pin A to go high
    while (digitalReadFast(c_EncoderPinA) == LOW)  
      ;    // wait for A to go low
    counts = counts++;  //increment count 
//   Serial.println(counts); 
  }
  qik.setM0Speed(0);
  Serial.print(counts);
  Serial.println(" counts forward");
  delay(1000);
  
  // Turn motor 0 on reverse
  qik.setM0Speed(-40);
  while (counts > 0){
    while (digitalReadFast(c_EncoderPinA) == HIGH)
      ;  // wait for encoder pin A to go high
    while (digitalReadFast(c_EncoderPinA) == LOW)
      ;  // wait for A to go low
    counts = counts--;  //increment count  
  }
  qik.setM0Speed(0);
  Serial.print(counts);
  Serial.println(" counts reverse");
  delay(1000);
  
}
