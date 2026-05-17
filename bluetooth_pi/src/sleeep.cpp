#include <sleeep.h>
#include <esp_sleep.h>
#include <driver/rtc_io.h>

#include <platformTypes.h>
#include <buttons.h>

#define TEMPS_PRE_ALARMA 2 //s 

namespace sleeep
{

    RTC_DATA_ATTR bool from_sleep = false; //flag per indicar que el microcontrolador ha arrencat després d'estar en mode sleep, i no després d'un reset
    RTC_DATA_ATTR struct timeval instant; //per guardar l'instant de temps en que s'ha entrat en mode sleep.

    //Rutina per preparar les condicions de wake up (botó esquerre, alarma), i registrar l'instant de sleep per actualitzar el rellotge en wakeup.
    //Paràmtres: timer: indica si cal despertar passat un temps. time: quan temps fins despertar-se, en segons
    void powerDown(bool timer, uint32 time)
    {
        //Permetre wake up en apretar el botó esquerre
        rtc_gpio_hold_en((gpio_num_t)L_BTN_PIN);
        esp_sleep_enable_ext1_wakeup(1ULL << L_BTN_PIN, ESP_EXT1_WAKEUP_ANY_LOW); //Argument 1: màscara amb el bit 21 a 1.

        //Si cal permetre el wakeup per temps
        if(timer == true)
        {
            //Restem TEMPS_PRE_ALARMA segons per donar temps a que timeMgr() activi l'alarma després del wake up, convertim a microsegons...
            //...i habilitem wakeup per timer
            esp_sleep_enable_timer_wakeup(((uint64)time - TEMPS_PRE_ALARMA) * 1e6);
        }
        
        gettimeofday(&instant, NULL); //Guardar l'instant de temps en que s'entra al mode sleep, en segons.
        from_sleep = true; //Aixecar flag per indicar que s'entra en mode sleep
        esp_deep_sleep_start(); //Posar a dormir
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
