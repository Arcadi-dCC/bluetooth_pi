#include <bluetooth.h>

#include "RF24.h"
#include <SPI.h>

#define BT_CS   39  // pass as the csn_pin parameter to the RF24 constructor
#define BT_MISO 37
#define BT_MOSI 35
#define BT_SCLK 36

#define BT_CE 38

#define BT_INITIAL_CH 45 //0 does not work

namespace bluetooth{

    SPIClass hspi;
    RF24 radio(BT_CE, BT_CS); // the (ce_pin, csn_pin) connected to the radio
    
    uint8 ch = 0;  // Initial channel for nRF24L01. 0 does not work
    Action action = OFF;

    //inicia el mòdul bluetooth
    //Return: 0-OK 1-error
    uint8 init(void)
    {
        hspi.begin(BT_SCLK, BT_MISO, BT_MOSI, BT_CS);

        action = OFF;
        ch = 0;
        if (!radio.begin(&hspi)) return 1; //error

        //OK
        delay(300);
        radio.setDataRate(RF24_2MBPS); // Set data rate to 2 Mbps
        radio.powerDown(); //estalvia energia
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
                radio.powerUp();
                radio.startConstCarrier(RF24_PA_MAX, BT_INITIAL_CH);  // Start continuous carrier in BT_INITIAL_CH

                action = JAMMING;
            }
            case JAMMING:
            {
                //Sweep through all 79 channels (0 to 78)
                for(ch = 0; ch < BT_TOTAL_CHANNELS; ch++) {radio.setChannel(ch);}

                // Specifically target BLE advertising channels (37, 38, 39) for good measure
                for(ch = 37; ch < 40; ch++) radio.setChannel(ch);
                break;
            }
            case STOP_JAM:
            {
                radio.stopConstCarrier(); //això posa el mòdul en sleep
                action = OFF;
                break;
            }
            default:
            {
                /*Do nothing*/
            }
        }
    }

    //Posa el mòdul bluetooth en mode lectura i fa NUM_READINGS lectures a tots els canals de l'espectre en busca de senyals
    //Cada cop que troba un senyal a un canal X, augmenta en 1 el valor del membre X de buf.
    //Return: 0-OK 1-l'array passat pe paràmetre és massa petit. 
    uint8 activitatEspectre(uint8 canal)
    {
        uint8 ctr = 0;

        //Configura i posa el mòdul BT en mode RX
        radio.setPALevel(RF24_PA_MAX);
        radio.setAutoAck(false);
        radio.disableCRC();
        radio.setPayloadSize(1);
        radio.setChannel(canal);
        radio.startListening();
        action = OFF;

        delay(130); //Deixa al mòdul fer els canvis pertinents
        for(uint8 t=0; t<BT_NUM_READINGS; t++)
        {
            if(radio.testRPD()) ctr++;
        }
        radio.stopListening();
        return ctr;
    }

}; //namespace bluetooth
