#include <menuFun.h>
#include <Arduino.h> //TEMP
#include <clocks.h>
#include <buttons.h>
#include <screen.h>
#include <bluetooth.h>

namespace menu
{
    State state = MENU1_GRAFIC_ESPECTRAL;

    //Mana iniciar o parar l'inhibició depenent del seu estat actual, i actualitza el seu estat a la barra superior.
    void toggleInhibir(void)
    {
        if(buttons::input == buttons::ESQ_LLARG)
        {
            switch (bluetooth::action){
                case bluetooth::START_JAM:
                case bluetooth::JAMMING:
                {
                    bluetooth::action = bluetooth::STOP_JAM;
                    break;
                }
                case bluetooth::STOP_JAM:
                case bluetooth::OFF:
                {
                    bluetooth::action = bluetooth::START_JAM;
                    break;
                }
                default: {/*Do nothing*/}
            }
        }
        buttons::input = buttons::RES;
        screen::print_MENU1_BLUETOOTH(); //Imprimeix cursor
        screen::updateTopBarJam();
    }

    //Activa i desactiva l'alarma
    void toggleAlarma(void)
    {
        if(buttons::input == buttons::ESQ_LLARG)
        {
            clocks::alarm.on = !clocks::alarm.on; //canvia estat de l'alarma si s'apreta i manté el botó esquerre
        }
        buttons::input = buttons::RES;
        screen::print_MENU2_CLOCKS(); //imprimeix cursor
        screen::updateTopBarAlarm();
    }

    //Imprimeix submenu de canvi d'hora o alarma per pantalla, Incrementa en 1 el valor actual si l'input és ESQ_CURT i
    //inicia un timer de SWITCH_OPTION_TIME us per passar al següent estat.
    void alarmIncSingle(void)
    {
        //demanar l'input TIMER passat el temps determinat
        buttons::restartTimer2Countdown(SWITCH_OPTION_TIME);

        if(buttons::input == buttons::ESQ_CURT)
        {
            switch(state)
            {
                case HORA_ESPERA_HORA:{
                    clocks::time.hh = (clocks::time.hh + 1 ) % 24; break;
                }
                case HORA_ESPERA_MINUT:{
                    clocks::time.mm = (clocks::time.mm + 1 ) % 60; break;
                }
                case ALARMA_ESPERA_HORA:{
                    clocks::alarm.hh = (clocks::alarm.hh + 1 ) % 24; break;
                }
                case ALARMA_ESPERA_MINUT:{
                    clocks::alarm.mm = (clocks::alarm.mm + 1 ) % 60; break;
                }
                case ALARMA_ESPERA_CANAL:{
                    clocks::alarm.ch = (clocks::alarm.ch + 1 ) % 81; break;
                }
                default:{/*Do nothing*/}
            }
        }
        buttons::input = buttons::RES;

        switch(state)
        {
            case HORA_ESPERA_HORA:
            case HORA_ESPERA_MINUT:
            {
                screen::print_MOD_RELLOTGE(); break;
            }
            case ALARMA_ESPERA_HORA:
            case ALARMA_ESPERA_MINUT:
            case ALARMA_ESPERA_CANAL:
            {
                screen::print_MOD_ALARMA(); break;
            }
            default:{/*Do nothing*/}
        }
    }

    //Imprimeix submenu de canvi d'hora o alarma per pantalla, Incrementa en 1 el valor actual si l'input és TIMER i
    //inicia un timer de TIME_BETWEEN_INCREMENT per continuar incrementant el valor
    void alarmIncMult(void)
    {
        //Només es crida a aquesta funció si el botó esquerre s'acaba d'apretar llarg, o si ha saltat el timer.
        //Per tant, no cal fer comprovacions. Es fa així per compatibilitat amb altres parts de l'estructura de la fsm.
        buttons::input = buttons::RES;
        buttons::restartTimer2Countdown(TIME_BETWEEN_INCREMENT);
        switch(state)
        {
            case HORA_INCR_HORA:{
                clocks::time.hh = (clocks::time.hh + 1 ) % 24; break;
            }
            case HORA_INCR_MINUT:{
                clocks::time.mm = (clocks::time.mm + 1 ) % 60; break;
            }
            case ALARMA_INCR_HORA:{
                clocks::alarm.hh = (clocks::alarm.hh + 1 ) % 24; break;
            }
            case ALARMA_INCR_MINUT:{
                clocks::alarm.mm = (clocks::alarm.mm + 1 ) % 60; break;
            }
            case ALARMA_INCR_CANAL:{
                clocks::alarm.ch = (clocks::alarm.ch + 1 ) % 81; break;
            }
            default:{/*Do nothing*/}
        }

        switch(state)
        {
            case HORA_INCR_HORA:
            case HORA_INCR_MINUT:
            {
                screen::print_MOD_RELLOTGE(); break;
            }
            case ALARMA_INCR_HORA:
            case ALARMA_INCR_MINUT:
            case ALARMA_INCR_CANAL:
            {
                screen::print_MOD_ALARMA(); break;
            }
            default:{/*Do nothing*/}
        }
    }

    //Funció inofensiva genèrica
    void fun(void){
        Serial.println(state);
        buttons::input = buttons::RES;
    }

} // namespace menu
