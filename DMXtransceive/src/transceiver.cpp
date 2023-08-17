#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "dmx_transceiver.h"

//  the pins that will be set HIGH when in that mode (for max485 module)
#define RX_ENABLE_PIN 6
#define TX_ENABLE_PIN 5

// transceiver instance best to be pointer
DMX_Transceiver *dmx_transceiver;

const int RedPin = 9; // PWM output pin for Red Light.
const int GreenPin = 6; // PWM output pin for Green Light.
const int BluePin = 5; // PWM output pin for Blue Light.
const int OutputEnable = 2 ; // Enable the output of the cqrobot-DMXShield

// This Example receives the 3 values starting with this channel:
const int FogChannel = 255;

//  listener channel
//uint16_t listener_channel = 511;

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
}


int alter_output_dmx(int channel, int value) {
  switch(channel) {
  case FogChannel:
    dmx_transceiver->set_dmx_value(FogChannel, 255);
    analogWrite(GreenPin, dmx_transceiver->get_dmx_value(FogChannel));
  }
}


//  forward all incoming to out
void set_output_dmx() {
  for(int i = 1; i <= 512; i++) {
//      dmx_transceiver->set_dmx_value(i, alter_output_dmx(i,dmx_transceiver->get_dmx_value(i)));
      dmx_transceiver->set_dmx_value(i, 128);
    }
}

void loop() {
  // put your main code here, to run repeatedly:

  //  listen for incoming dmx
  //  returns when a packet is received or after a timeout
  dmx_transceiver->receive();


  //  forward all incoming dmx packets to the output
  set_output_dmx();

  //  alter the output
//  alter_output_dmx();

  //  transmit the output
  dmx_transceiver->transmit();
}
