// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialRecv.ino: Sample DMX application for retrieving 3 DMX values:
// address 1 (red) -> PWM Port 9
// address 2 (green) -> PWM Port 6
// address 3 (blue) -> PWM Port 5
//
// Copyright (c) 2011-2015 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
//
// Documentation and samples are available at http://www.mathertel.de/Arduino
// 25.07.2011 creation of the DmxSerial library.
// 10.09.2011 fully control the serial hardware register
//            without using the Arduino Serial (HardwareSerial) class to avoid ISR implementation conflicts.
// 01.12.2011 include file and extension changed to work with the Arduino 1.0 environment
// 28.12.2011 changed to channels 1..3 (RGB) for compatibility with the DmxSerialSend sample.
// 10.05.2012 added some lines to loop to show how to fall back to a default color when no data was received since some time.
// - - - - -
/* 
 * with modifications for cqrobot-DMXShield
 */


#include <Arduino.h>
#include <DMXSerial.h>

// Constants for demo program

const int RedPin = 9; // PWM output pin for Red Light.
const int GreenPin = 6; // PWM output pin for Green Light.
const int BluePin = 5; // PWM output pin for Blue Light.

const int OutputEnable = 2 ; // Enable the output of the cqrobot-DMXShield

// This Example receives the 3 values starting with this channel:
const int startChannel = 0 * 3 + 1;

void setup() {
  DMXSerial.init(DMXReceiver);

  // set some default values
  DMXSerial.write(1, 80);
  DMXSerial.write(2, 0);
  DMXSerial.write(3, 0);

  // enable pwm outputs
  pinMode(RedPin, OUTPUT); // sets the digital pin as output
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(OutputEnable, OUTPUT);
}


void loop() {
  digitalWrite(OutputEnable, LOW);

  // Calculate how long no data bucket was received
  unsigned long lastPacket = DMXSerial.noDataSince();

  if (lastPacket < 5000) {
    digitalWrite(RedPin, LOW);
    // read recent DMX values and set pwm levels
    analogWrite(GreenPin, DMXSerial.read(startChannel + 1));
    analogWrite(BluePin, DMXSerial.read(startChannel + 2));

  } else {
    // Show pure red color, when no data was received since 5 seconds or more.
    digitalWrite(RedPin, HIGH);
  } // if
}

// End.
