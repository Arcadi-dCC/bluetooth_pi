#include <menuFun.h>
#include <Arduino.h> //TEMP

#include <clocks.h>
#include <buttons.h>
#include <bluetooth.h>

namespace menu
{
    State state = MENU1_GRAFIC_ESPECTRAL;

    //Mostra el menu 1 per pantalla. Depenent de l'estat, col·loca el ratolí al costat de cada nom.
    void showMenu1(void)
    {
        switch(state)
        {
            case(MENU1_GRAFIC_ESPECTRAL):{
                Serial.println("Gràfic Espectral");
                break;
            }
            case(MENU1_CANALS_ACTIUS):{
                Serial.println("Canals actius");
                break;
            }
            case(MENU1_INHIBIR_MANUAL):{
                Serial.println("Inhibir manual");
                break;
            }
            case(MENU1_INHIBIR_ESPECTRE):{
                bluetooth::inhibir = false;
                Serial.println("Inhibir espectre");
                break;
            }
            default:{
                /*Do nothing*/
            }
        }
    }

    //Mostra el menu 2 per pantalla. Depenent de l'estat, col·loca el ratolí al costat de cada nom.
    void showMenu2(void)
    {
        switch(state)
        {
            case(MENU2_CANVI_HORA):{
                Serial.print("Hora: ");
                Serial.print(clocks::time.hh);
                Serial.print(":");
                Serial.print(clocks::time.mm);
                Serial.print(":");
                Serial.print(clocks::time.ss);
                Serial.println(" - Modificar");
                
                break;
            }
            case(MENU2_CANVI_ALARMA):{
                Serial.print("Alarma: ");
                Serial.print(clocks::alarm.hh);
                Serial.print(":");
                Serial.print(clocks::alarm.mm);
                Serial.print(":");
                Serial.print(clocks::alarm.ss);
                Serial.println(" - Modificar");
                break;
            }
            case(MENU2_ACTIVAR_ALARMA):{
                if(buttons::input == buttons::ESQ_LLARG)
                {
                    clocks::alarm.on = !clocks::alarm.on; //canvia estat de l'alarma si s'apreta i manté el botó esquerre
                }
                buttons::input = buttons::RES;
                Serial.print("Alarma: Des/activar - ");
                Serial.println(clocks::alarm.on);

                break;
            }
            default:{
                /*Do nothing*/
            }
        }
    }

    void fun(void){/*Generic dummy funciton*/
        Serial.println(state);
        buttons::input = buttons::RES;
    }

} // namespace menu
