#include <bluetooth.h>
#include <screen.h>

#include "RF24.h"
#include <SPI.h>

#define BT_CS   39  // pass as the csn_pin parameter to the RF24 constructor
#define BT_MISO 37
#define BT_MOSI 35
#define BT_SCLK 36

#define BT_CE 38

#define BT_INITIAL_CH 45 //0 does not work

//Segons datasheet, el temps mínim per poder fer lectures és 1,63ms en sortir de powerDown(), i després 150us cada canvi entre TX i RX.
//Amb 2ms funciona OK, pero es posa més alt perquè gràfic i llista s'actualitzen massa ràpid.
#define BT_TIME_BTW_READINGS 35 //ms

namespace bluetooth{

    SPIClass hspi;
    RF24 radio(BT_CE, BT_CS); // the (ce_pin, csn_pin) connected to the radio
    
    uint8 ch = 0;
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
        radio.startConstCarrier(RF24_PA_MAX, BT_INITIAL_CH);  // Start continuous carrier in BT_INITIAL_CH. Millora la configuració interna
        radio.stopConstCarrier(); //això posa el mòdul en sleep
        //radio.printPrettyDetails();    // Print radio details for debugging
        return 0;
    }

    //Envia comandes al modul Bluetooth per inhibir o no inhibir, depenent de l'estat de "action", i actualitza l'estat a la barra superior.
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
                screen::updateTopBarJam();
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
                screen::updateTopBarJam();
                break;
            }
            default: {/*Do nothing*/break;}
        }
    }

    //Posa el mòdul Bluetooth en mode lectura i analitza un canal cada vegada
    //Depenent de quina pantalla s'estigui mostrant, imprimeix l'informació per pantalla amb dues funcions diferents
    void readMgr(void)
    {
        if(screen::pantalla == screen::GRAFIC_ESPECTRAL || screen::pantalla == screen::CANALS_ACTIUS)
        {
            //Posa en marxa l'antena si està apagada
            switch (action)
            {
                case OFF: //Posa en marxa l'antena si està apagada
                {
                    action = READING;
                    radio.setPayloadSize(1);
                    ch = BT_TOTAL_CHANNELS; //Per començar amb el canal 0 (veure següent línia de codi)
                }
                case READING:
                {
                    ch = (ch + 1) % BT_TOTAL_CHANNELS;
            
                    radio.setChannel(ch);
                    radio.startListening();

                    delay(BT_TIME_BTW_READINGS); //Deixa al mòdul fer els canvis pertinents
                        
                    uint8 detections = 0;
                        
                    //Si detecta senyals per sobre de -64 dBm, registrar-les.
                    for(uint8 t=0; t<BT_NUM_READINGS; t++)
                    {
                        if(radio.testRPD()) detections++;
                    }
                
                    radio.stopListening();
                
                    switch(screen::pantalla)
                    {
                        case screen::GRAFIC_ESPECTRAL:
                        {
                            screen::print_GRAFIC_ESPECTRAL(ch, detections);
                            break;
                        }
                        case screen::CANALS_ACTIUS:
                        {
                            screen::print_CANALS_ACTIUS(ch, detections);
                            break;
                        }
                        default:{/*Do nothing*/break;}
                    }
                    break;
                }
                default: {/*Do nothing*/break;}
            }   
        }
    }

}; //namespace bluetooth
