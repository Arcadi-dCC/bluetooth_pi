#include <menuFun.h>
#include <clocks.h>
#include <buttons.h>
#include <screen.h>
#include <bluetooth.h>
#include <sleeep.h>

namespace menu
{
    State state = MENU1_GRAFIC_ESPECTRAL;

    //Mana parar l'inhibicio de Bluetooth si està activa, i dona instruccions per iniciar el gràfic espectral o la llista de canals més actius
    void beginAnalisiEsp(void)
    {
        if(bluetooth::action == bluetooth::START_JAM || bluetooth::action == bluetooth::JAMMING)
        {
            bluetooth::action = bluetooth::STOP_JAM;
        }

        screen::clearMainScreen();

        switch(state)
        {
            case GRAFIC_ESPECTRAL:
            {
                screen::pantalla = screen::GRAFIC_ESPECTRAL;
                break;
            }
            case CANALS_ACTIUS:
            {
                screen::pantalla = screen::CANALS_ACTIUS;
                for(uint8 i=0; i<5; i++)
                {
                    screen::intensitats[i] = 0;
                    screen::canals_actius[i] = 0;
                }
                screen::printNoActius();
                break;
            }
            default:{/*Do nothing*/break;}
        }
    }

    //Mana iniciar o parar l'inhibició depenent del seu estat actual.
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
                case bluetooth::READING:
                case bluetooth::OFF:
                {
                    bluetooth::action = bluetooth::START_JAM;
                    break;
                }
                default: {/*Do nothing*/}
            }
        }
        buttons::input = buttons::RES;
        screen::print_MENU1_BLUETOOTH(); //Imprimeix cursor per si s'ha entrat aqui navegant pels menus.
    }

    //Activa i desactiva l'alarma
    void toggleAlarma(void)
    {
        if(buttons::input == buttons::ESQ_LLARG)
        {
            clocks::alarm.on = !clocks::alarm.on; //canvia estat de l'alarma si s'apreta i manté el botó esquerre
        }
        buttons::input = buttons::RES;
        screen::print_MENU2_CLOCKS(); //Imprimeix cursor per si s'ha entrat aqui navegant pels menus.
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
                default:{/*Do nothing*/break;}
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
            {
                screen::print_MOD_ALARMA(); break;
            }
            default:{/*Do nothing*/break;}
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
            default:{/*Do nothing*/break;}
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
            {
                screen::print_MOD_ALARMA(); break;
            }
            default:{/*Do nothing*/break;}
        }
    }

    //Desactiva els perifèrics, llegeig l'alarma per saber si cal tornar a aixecar-se passat un temps, i crida a sleeep::powerDown per...
    ///...configurar l'sleep i entrar en ell.
    void apagar(void)
    {
        //Desactivar perifèrics
        bluetooth::turnOff();
        screen::sleep();

        uint32 max_sleep_time = 0;
        //Si l'alarma està activada, calcular temps entre ara i l'alarma
        if(clocks::alarm.on == true)
        {
            //Obtenim instant de temps actual i de l'alarma, en segons
            uint32 actual = (uint32)clocks::time.hh*3600 + (uint32)clocks::time.mm*60 + (uint32)clocks::time.ss;
            uint32 alarma = (uint32)clocks::alarm.hh*3600 + (uint32)clocks::alarm.mm*60;

            //Calculem temps entre ara i la propera alarma, en segons, i li restem 3 segons. Així, timeMgr() activarà l'alarma sol.
            if(actual <= alarma) //L'alarma està programada més endavant el mateix dia
            {
                max_sleep_time = alarma - actual;
            }
            else //L'alarma està programada per l'endemà. 86400 = 24h * 3600s en un hora
            {
                max_sleep_time = 86400 - actual + alarma; // equivalent a: 86400 - (actual-alarma)
            }
        }

        //Configura sleep i posa a dormir
        sleeep::powerDown(clocks::alarm.on, max_sleep_time);
    }

} // namespace menu
