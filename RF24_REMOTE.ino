#include <SPI.h>
#include "Mirf.h"
#include "nRF24L01.h"
#include "MirfHardwareSpiDriver.h"
Nrf24l Mirf = Nrf24l(9, 10);

// Pins that will read Throttle, Yaw, Pitch and Roll. Two switches are captured and sent, but not currently used.
#define pinT A0
#define pinY A1
#define pinP A7
#define pinR A6
#define pinSW1 16
#define pinSW2 19

// Data payload that will be sent. 
byte channel[6];

void setup() {

  // Use internal 5k pullups. Easy.
  pinMode(pinSW1, INPUT_PULLUP);
  pinMode(pinSW2, INPUT_PULLUP);

  // I am not sure why we need a transmitter address here, but it was included in the MIRF example, so...
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setTADDR((byte *)"FGHIJ");
  Mirf.setRADDR((byte *)"ABCDE");
  Mirf.payload = sizeof(channel);
  //Use the scanner to find unoccupied bands. Just like the wifi, it's best to have 2-3 empty ones on each side.
  Mirf.channel = 90;
  Mirf.config();
}



void loop() {
  // Pull the stick data into the payload array.
  channel[0] = map(analogRead(pinT), 0, 1023, 0, 255);
  channel[1] = map(analogRead(pinY), 0, 1023, 0, 255);
  channel[2] = map(analogRead(pinP), 0, 1023, 0, 255);
  channel[3] = map(analogRead(pinR), 0, 1023, 0, 255);
  channel[4] = digitalRead(pinSW1);
  channel[5] = digitalRead(pinSW2);

  // Send the payload 25 times per second. For this project it's plenty.  It allows AA and retries, so it's reliable.
  Mirf.send((byte *)&channel); // I have no idea why you have to cast the reference to byte pointer, but sure.
  while (Mirf.isSending()) delay(1);
  delay(40);
}
