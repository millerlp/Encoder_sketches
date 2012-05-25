
/*  
  This code misses interrupt signals if the motor spins quickly.
  
  Test code to use hardware interrupts. Code adapted from
 http://www.billporter.info/sagar-update-new-motor-controller-and-motors/
 This code reduces the resolution of the encoder by a factor of 4. 
 I'm using this with a Pololu 50:1 gear reduction 37mm motor with attached
 encoder.
 Originally the motor encoder output 64 pulses (high or low changes on the
 two output pins combined) per rotation of the motor's armature. Because
 this code only triggers on a RISING change on a single interrupt pin,
 counts only happen when encoder pin A goes high, which only happens 16 
 times per motor armature revolution. This reduces the resolution of the 
 encoder so you can only tell that the motor has moved approximately 1/16th
 of a revolution instead of 1/64th (22.5° vs 5.625° resolution). On a 
 gear-reduced motor this shouldn't amount to much error at the ouput shaft.
 
 */

#include "Arduino.h"
#include <digitalWriteFast.h>

#include <mySoftwareSerial.h>
#include <PololuQik.h>

#define c_EncoderPinInterrupt 0    // interrupt 0 (digital pin 2 on Ard328)
#define c_EncoderPinA 2     // digital pin 2
#define c_EncoderPinB 4    // digital pin 4 on Ard328
//#define EncoderIsReversed
volatile bool _EncoderBSet;
volatile long _EncoderTicks = 0;

long time;




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

void setup() {
  Serial.begin(115200);
  Serial.println("Beginning...");

  //Quadrature encoders
  pinMode(c_EncoderPinA, INPUT);  // sets encoder pin A as input
  digitalWrite(c_EncoderPinA, LOW);  // turn on pullup resistor
  pinMode(c_EncoderPinB, INPUT);  // sets encoder pin B as input
  digitalWrite(c_EncoderPinB, LOW); // turn on pullup resistor
  attachInterrupt(c_EncoderPinInterrupt, HandleInterruptA, RISING);

  // The value in parentheses is the serial comm speed
  // for the 2s9v1 controller
  qik.init(38400);
  delay(4000);

}


void loop() {
  time = millis();
  while (1) {
    // Sit in a loop and print encoder count every second
    if (millis() > (time + 1000)) {
      Serial.println(_EncoderTicks);
      time = millis();
    }
  }

}


// Interrupt service routine, should trigger each time
// digital pin 2 (interrupt 0) get a RISING signal.
void HandleInterruptA()
{
  //Test transition of pin B, we already know pin A just went high
  _EncoderBSet = digitalReadFast(c_EncoderPinB); // read pin B

#ifdef EncoderIsReversed
    _EncoderTicks += _EncoderBSet ? -1 : +1;
#else
  _EncoderTicks -= _EncoderBSet ? -1 : +1;    
#endif
}




