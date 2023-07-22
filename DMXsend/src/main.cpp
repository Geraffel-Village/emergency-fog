// - - - - -
// DmxSerial - A hardware supported interface to DMX.
// DmxSerialSend.ino: Sample DMX application for sending 3 DMX values.
// There colors in the 3 lists (RedList, GreenList and BlueList) are placed into the DMX buffer with a slow fade.
// DMXSerial works in the background and constantly sends the actual values over the DMX interface.
// The actual values are also available on the built in PWM ports:
// address 1 (red) -> also available on PWM Port 9
// address 2 (green) -> also available on PWM Port 6
// address 3 (blue) -> also available on PWM Port 5
//
// Copyright (c) 2011-2015 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
//
// Documentation and samples are available at http://www.mathertel.de/Arduino
// The repository is on github : https://github.com/mathertel/DMXSerial
// The repository on github is made available in the Arduino Library Manager.
//
// 25.07.2011 creation of the DmxSerial library.
// 10.09.2011 fully control the serial hardware register
//            without using the Arduino Serial (HardwareSerial) class to avoid ISR implementation conflicts.
// 01.12.2011 include file and extension changed to work with the Arduino 1.0 environment
// - - - - -

#include <Arduino.h>
#include <DMXSerial.h>

// Constants for demo program

const int RedPin = 9; // PWM output pin for Red Light.
const int GreenPin = 6; // PWM output pin for Green Light.
const int BluePin = 5; // PWM output pin for Blue Light.

const int OutputEnable = 2 ; // Enable the output of the cqrobot-DMXShield
const int Button1Pin = 13;

// The color fading pattern

int GreenList[] = {64, 128, 255, 255,   0,   0};
int BlueList[] =  {64,   0,   0, 255, 255, 128};

int RedLevel, GreenLevel, BlueLevel;

int state = 0;
int buttonState = 0;        // current state of the button
int lastButtonState = 0;    // previous state of the button

void setup() {
  DMXSerial.init(DMXController);

  pinMode(OutputEnable, OUTPUT);
  pinMode(Button1Pin, INPUT_PULLUP);
  pinMode(RedPin, OUTPUT); // sets the digital pin as output
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
} // setup


// loop through the rainbow colors
void loop() {
  digitalWrite(OutputEnable, HIGH);

  GreenLevel = GreenList[state];
  BlueLevel = BlueList[state];

  buttonState = digitalRead(Button1Pin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
//      buttonPushCounter++;
      // Serial.println("on");
      // Serial.print("number of button pushes: ");
      // Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      // Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
//    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

if (buttonState == HIGH) {
  DMXSerial.write(3, 0);
} else {
  DMXSerial.write(3, 1);
}
//    state += 1;
    if (state == 6)
      state = 0;

} // loop
