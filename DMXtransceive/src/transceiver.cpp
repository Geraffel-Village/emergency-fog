#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dmx_transceiver.h"

//  the pins that will be set HIGH when in that mode (for max485 module)
#define RX_ENABLE_PIN 6
#define TX_ENABLE_PIN 5
#define BUTTON1_PIN 13

// transceiver instance best to be pointer
DMX_Transceiver *dmx_transceiver;

const int RedPin = 9; // PWM output pin for Red Light.
const int GreenPin = 6; // PWM output pin for Green Light.
const int BluePin = 5; // PWM output pin for Blue Light.
const int OutputEnable = 2 ; // Enable the output of the cqrobot-DMXShield

// This Example receives the 3 values starting with this channel:
const int FogChannel = 255;

bool button_active;
unsigned long button_lastactive_ts;

void setup() {
  //  initialize a new transceiver instance
  dmx_transceiver = new DMX_Transceiver();
  dmx_transceiver->set_rx_enable_pin(RX_ENABLE_PIN);
  dmx_transceiver->set_tx_enable_pin(TX_ENABLE_PIN);
  dmx_transceiver->init();  

  // enable pwm outputs
  pinMode(RedPin, OUTPUT); // sets the digital pin as output
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(OutputEnable, OUTPUT);

  //configure pin 13 as an input and enable the internal pull-up resistor
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  button_active = false;
  button_lastactive_ts = 0;
}



//  forward all incoming to out
void set_output_dmx() {
  for(int i = 1; i <= 512; i++) {
    switch(i) {
    case FogChannel:
      const unsigned long ACTIVITY_EXTENSION = 15 * 1000; // 15 seconds
      analogWrite(GreenPin, dmx_transceiver->get_dmx_value(FogChannel));
      if ( (button_active) or ( (millis() - button_lastactive_ts) < ACTIVITY_EXTENSION) ) {
        dmx_transceiver->set_dmx_value(FogChannel, 255);
      } else {
        dmx_transceiver->set_dmx_value(FogChannel, dmx_transceiver->get_dmx_value(FogChannel));
      }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  //  listen for incoming dmx
  //  returns when a packet is received or after a timeout
  dmx_transceiver->receive();

  // high = button open/not pressed
  if (digitalRead(BUTTON1_PIN) == false) {
    button_active = true;
    button_lastactive_ts = millis();
  } else {
    button_active = false;
  }

  if (button_active)
    analogWrite(RedPin, 200);
  else
    analogWrite(RedPin,0);

  //  forward all incoming dmx packets to the output
  set_output_dmx();

  //  transmit the output
  dmx_transceiver->transmit();
}
