#include <bluetooth.h>
#include <platformTypes.h>

#include "RF24.h"
#include <SPI.h>

#define BT_CS   39  // pass as the csn_pin parameter to the RF24 constructor
#define BT_MISO 37
#define BT_MOSI 35
#define BT_SCLK 36

#define BT_CE 38

namespace bluetooth{

    SPIClass hspi;
    RF24 radio(BT_CE, BT_CS); // the (ce_pin, csn_pin) connected to the radio
    
    byte ch = 45;  // Initial channel for nRF24L01
    Action action = OFF;

    //inicia el mòdul bluetooth
    //Return: 0-OK 1-error
    uint8 init(void)
    {
        hspi.begin(BT_SCLK, BT_MISO, BT_MOSI, BT_CS);

        action = OFF;
        ch = 45;
        if (!radio.begin(&hspi)) return 1; //error

        //OK
        delay(300);
        radio.setAutoAck(false);       // Disable automatic acknowledgment
        radio.stopListening();         // Set to transmitter mode
        radio.setRetries(0, 0);        // Disable retries
        radio.setPayloadSize(5);       // Set payload size to 5 bytes
        radio.setAddressWidth(3);      // Set address width to 3 bytes
        radio.setPALevel(RF24_PA_MAX, true); // Set power amplification to maximum
        radio.setDataRate(RF24_2MBPS); // Set data rate to 2 Mbps
        radio.setCRCLength(RF24_CRC_DISABLED); // Disable CRC
        radio.startConstCarrier(RF24_PA_MAX, ch);  // Start continuous carrier in channel 0
        //radio.powerDown(); //estalvia energia
        //radio.printPrettyDetails();    // Print radio details for debugging
        return 0;
    }

    //Envia comandes al modul Bluetooth per inhibir o no inhibir, depenent de l'estat de "action".
    void jamMgr(void)
    {
        switch(action)
        {
            case START_JAM:
            {
                //Posa en marxa el mòdul Bluetooth
                //radio.powerUp();


                action = JAMMING;
            }
            case JAMMING:
            {
                //Sweep through all 79 channels (0 to 78)
                for(ch = 0; ch < 79; ch++) {radio.setChannel(ch);}

                // Specifically target BLE advertising channels (37, 38, 39)
                for(ch = 37; ch < 40; ch++) radio.setChannel(ch);
                break;
            }
            case STOP_JAM:
            {
                //radio.stopConstCarrier(); //això posa el mòdul en sleep
                action = OFF;
                break;
            }
            default:
            {
                /*Do nothing*/
            }
        }
    }

}; //namespace bluetooth
