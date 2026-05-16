#include <sleeep.h>
#include <esp_sleep.h>
#include <driver/rtc_io.h>

#include <platformTypes.h>
#include <clocks.h>
#include <buttons.h>
#include <bluetooth.h>

#define TEMPS_PRE_ALARMA 3 //s 

namespace sleeep
{

    RTC_DATA_ATTR bool from_sleep = false; //flag per indicar que el microcontrolador ha arrencat després d'estar en mode sleep, i no després d'un reset

    //Rutina per preparar les condicions de wake up (botó esquerre i alarma), i posar el microcontrolador i perifèrics en mode sleep.
    void powerDown(void)
    {
        //Permetre wake up en apretar el botó esquerre
        rtc_gpio_hold_en((gpio_num_t)L_BTN_PIN);
        esp_sleep_enable_ext1_wakeup(1ULL << L_BTN_PIN, ESP_EXT1_WAKEUP_ANY_LOW); //Argument 1: màscara amb el bit 21 a 1.

        
        //Si l'alarma està activada, calcular temps entre ara i l'alarma i demanar un wakeup passat aquest temps
        if(clocks::alarm.on == true)
        {
            //Obtenim instant de temps actual i de l'alarma, en segons
            uint64 ara = (uint64)clocks::time.hh*3600 + (uint64)clocks::time.mm*60 + (uint64)clocks::time.ss;
            uint64 alarma = (uint64)clocks::alarm.hh*3600 + (uint64)clocks::alarm.mm*60;

            //Calculem temps entre ara i la propera alarma, en segons, i li restem 3 segons. Així, timeMgr() activarà l'alarma sol.
            if(ara <= alarma)
            {
                //Si l'alarma està programada més endavant el mateix dia
                alarma = alarma - ara;
            }
            else
            {
                //Si l'alarma està programada per l'endemà. 86400 = 24h * 3600s en un hora
                alarma = 86400 - ara + alarma; // equivalent a → 86400 - (ara-alarma)
            }

            //Restem TEMPS_PRE_ALARMA segons per donar temps a que timeMgr() activi l'alarma després del wake up, i convertim a microsegons
            alarma = (alarma - TEMPS_PRE_ALARMA) * 1e6;

            esp_sleep_enable_timer_wakeup(alarma);
        }

        //Apagar perifèrics
        bluetooth::turnOff();

        //Aixecar flag per indicar que s'entra en mode sleep
        from_sleep = true;

        //Posar a dormir
        esp_deep_sleep_start();
    }

    //Funció a cridar després d'un wake up (!= reset).
    //Cridar després de clocks::init(), ja que aquesta empra el flag from_sleep per inicialitzar (o no) el rellotge i l'alarma.
    void onWakeUp(void)
    {
        if(from_sleep == true)
        {
            from_sleep = false;
            rtc_gpio_deinit((gpio_num_t)L_BTN_PIN); //allibera el pull up de RTC per a L_BTN_PIN
        }
    }
    
}; // namespace sleep
