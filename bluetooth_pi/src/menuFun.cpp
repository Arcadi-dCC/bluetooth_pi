#include <menuFun.h>
#include <Arduino.h> //TEMP

#include <clocks.h>
#include <buttons.h>
#include <bluetooth.h>

namespace menu
{
    State state = MENU1_GRAFIC_ESPECTRAL;

    //Activa i desactiva l'alarma
    void modAlarma(void)
    {
        if(buttons::input == buttons::ESQ_LLARG)
        {
            clocks::alarm.on = !clocks::alarm.on; //canvia estat de l'alarma si s'apreta i manté el botó esquerre
        }
        buttons::input = buttons::RES;
        //TODO Canvia el signe de l'alarma
    }

    //Funció inofensiva genèrica
    void fun(void){
        Serial.println(state);
        buttons::input = buttons::RES;
    }

} // namespace menu
