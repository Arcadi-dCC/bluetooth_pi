#include <clocks.h>
#include <platformTypes.h>
#include <Arduino.h>

namespace clocks
{
    volatile bool adv_sec = false; //flag to be raised when a second has passed
    void IRAM_ATTR timerInt(void)
    {
        adv_sec = true;
    }

    typedef struct Alarm{
        uint8 hh, mm, ss;
        uint8 ch; //channel for alarm
        bool on; //on-off for alarm
    }Alarm;

    Alarm time;
    Alarm alarm;

    void init(void)
    {
        //initialize time and alarm
        time.hh=0;
        time.mm=0;
        time.ss=0;

        alarm.hh=0;
        alarm.mm=0;
        alarm.ss=0;
        alarm.on=false;

        //Inicialitzar timer0
        timerBegin(0, 80, true); //clock source default: 80MHz. divided by 80
        timerAttachInterrupt(0, timerInt, true); //set interrupt function to timerInt
        timerAlarmWrite(0, 10000000, true); //set boundary to 10M: 1 sec
        timerAlarmEnable(0);
    }

    //Time manager. If second passed flag is enabled, updates time memory.
    void timeMgr(void)
    {
        if(adv_sec == true){
            time.ss++;

            if(time.ss >= 60)
            {
                time.ss = 0;
                time.mm++;
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
        }
        adv_sec=false;
    }

}; //namespace clocks