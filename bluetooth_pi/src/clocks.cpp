#include <clocks.h>
#include <platformTypes.h>
#include <Arduino.h>
#include <screen.h>
#include <bluetooth.h>

namespace clocks
{
    volatile bool adv_sec = false; //flag per indicar que ha passat 1 segon

    bool new_min = false; //flag per indicar a pantalla que ha passat un minut: cal actualitzar el temps mostrat i comprovar l'alarma
    
    //ISR per a timer1: cridada 1 cop per segon
    void IRAM_ATTR secISR(void)
    {
        adv_sec = true; //aixeca flag
    }

    Alarm time;
    Alarm alarm;

    hw_timer_t* sec_timer = NULL; //punter a timer1

    //Inicialitza el timer pel rellotge, i inicia rellotge i alarma a mitjanit
    //Return: 0-OK 1-error
    uint8 init(void)
    {
        //Inicialitza rellotge i alarma a mitjanit
        time.hh=0;
        time.mm=0;
        time.ss=0;

        alarm.hh=0;
        alarm.mm=0;
        alarm.ss=0;
        alarm.ch=0;
        alarm.on=false;

        adv_sec = false;
        new_min = false;

        //Inicialitzar timer1
        sec_timer = timerBegin(1, 80, true); //clock source default: 80MHz. Dividit per 80: 1MHz
        if (sec_timer == NULL)
        {
            return 1; //sortir i avisar si el timer no s'ha inicialitzat correctament
        }

        timerAttachInterrupt(sec_timer, secISR, true); //fixa secISR com a funció per la interrupció de timer1
        timerAlarmWrite(sec_timer, 1e6, true); //fixa el límit de timer1 a 1M -> cada segon
        timerAlarmEnable(sec_timer); //permet les interrupcions de timer1

        return 0;
    }

    //Time manager. Actualitza l'hora si s'ha aixecat el flag que indica que ha passat 1 segon
    void timeMgr(void)
    {
        if(adv_sec == true){
            adv_sec=false; //desactiva el flag
            
            time.ss++;
            if(time.ss >= 60)
            {
                time.ss = 0;
                time.mm++;
                new_min = true;
            }
            if(time.mm >= 60)
            {
                time.mm = 0;
                time.hh++;
            }
            if(time.hh >= 24)
            {
                time.hh = 0;
            }

            //Si s'ha actualitzat el minut...
            if(new_min == true)
            {
                new_min = false; //desactiva el flag
                screen::updateTime(); //...actualitza el temps per pantalla.
                
                //...i fa saltar l'alarma si està activada i és el moment.
                //TODO L'alarma només hauria de poder inhibir tot l'espectre.
                //TODO: això no funciona si es posa l'aparell en mode sleep
                if(alarm.on == true && time.hh == alarm.hh && time.mm == alarm.mm && (bluetooth::action == bluetooth::STOP_JAM || bluetooth::action == bluetooth::OFF))
                {
                    bluetooth::action = bluetooth::START_JAM;
                    screen::updateTopBarJam();
                }
            }
        }
    }

}; //namespace clocks
