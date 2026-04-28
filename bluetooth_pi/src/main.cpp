#include <Arduino.h>
#include "RF24.h"
#include <SPI.h>

#define MY_MISO 13
#define MY_MOSI 11
#define MY_SCLK 12
#define MY_SS   10  // pass MY_SS as the csn_pin parameter to the RF24 constructor

// notice these pin numbers are not the same used in the library examples
RF24 radio(9, 10); // the (ce_pin, csn_pin) connected to the radio

SPIClass* hspi = nullptr; // we'll instantiate this in the `setup()` function
// by default the HSPI bus pre-defines the following pins
// HSPI_MISO = 12
// HSPI_MOSI = 13
// HSPI_SCLK = 14
// HSPI_SS   = 15

byte i = 45;  // Initial channel for nRF24L01
unsigned int flag = 0;  // Flag to control channel hopping direction

void setup() {
  Serial.begin(115200); // Initialize serial communication
  delay(800);
  Serial.println("Serial COM Enabled");
  hspi = new SPIClass(); // by default VSPI is used
  hspi->begin(MY_SCLK, MY_MISO, MY_MOSI, MY_SS);
  //hspi->begin();

 if (radio.begin(hspi)) {
    delay(200);
    Serial.println("Radio Initialized!");
    radio.setAutoAck(false);       // Disable automatic acknowledgment
    radio.stopListening();         // Set to transmitter mode
    radio.setRetries(0, 0);        // Disable retries
    radio.setPayloadSize(5);       // Set payload size to 5 bytes
    radio.setAddressWidth(3);      // Set address width to 3 bytes
    radio.setPALevel(RF24_PA_MAX, true); // Set power amplification to maximum
    radio.setDataRate(RF24_2MBPS); // Set data rate to 2 Mbps
    radio.setCRCLength(RF24_CRC_DISABLED); // Disable CRC
    radio.printPrettyDetails();    // Print radio details for debugging
    radio.startConstCarrier(RF24_PA_MAX, i);  // Start continuous carrier with specified channel
  } else {
    Serial.println("Failed to initialize radio!");
  }
}

void loop() {
  // Sweep through all channels (0 to 79)
   for (int i = 0; i < 79; i++) {
    radio.setChannel(i);
    //Serial.println("Jamming channel:");
    //Serial.println(i);
   }
    // Specifically target BLE advertising channels (37, 38, 39)
  byte ble_channels[] = {37, 38, 39};
  for (int j = 0; j < 3; j++) {
    radio.setChannel(ble_channels[j]);
  }
}