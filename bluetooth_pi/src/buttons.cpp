#include <buttons.h>
#include <Arduino.h>

namespace buttons{

    volatile Input input;

    volatile bool ms100_flag = false;
    volatile bool button_flag = false;
    volatile bool s3_flag = false;

    hw_timer_t *timer_1 = NULL;

    void IRAM_ATTR rButtonInt(void);
    void IRAM_ATTR lButtonInt(void);
    void IRAM_ATTR rTimerInt(void);
    void IRAM_ATTR lTimerInt(void);

    //Right button interrupt function
    void IRAM_ATTR rButtonInt(void)
    {
        if(digitalRead(R_BTN_PIN) == LOW) //just s'ha apretat
        {
            //desactivar interrupció dos botons
            detachInterrupt(digitalPinToInterrupt(R_BTN_PIN));
            detachInterrupt(digitalPinToInterrupt(L_BTN_PIN));

            //activar timer 100ms
            timerAlarmWrite(timer_1, 1000000, true); //set boundary to 1M: 100 ms
            timerAlarmEnable(timer_1);

            ms100_flag = true; //activar flag
        }

        if(digitalRead(R_BTN_PIN) == HIGH) //s'ha soltat
        {
            if(s3_flag == false) //encara no han passat 3 segons
            {
                input = DRE_CURT;

            }
            /*else //No existeix aquesta opció: el micro està en sleep
            {
                input = DRE_SOLTAR;
            }*/
            
            //Activar interrupció de l'altre botó
            attachInterrupt(L_BTN_PIN, lButtonInt, CHANGE);
        }
    }

    //Timer interrupt to manage debouncing and short-long press for right button
    void IRAM_ATTR rTimerInt(void)
    {
        if(ms100_flag == true) //debouncing
        {
            ms100_flag = false; //desactivar flag

            attachInterrupt(R_BTN_PIN, rButtonInt, CHANGE); //reactivar interrupció botó
            if(digitalRead(L_BTN_PIN) == LOW) //botó segueix apretat → debouncing OK
            {
                button_flag = false; //preparant flag botó

                //programar timer per a 3 s
                timerAlarmWrite(timer_1, 30000000, true); //set boundary to 30M: 3 segons
                timerAlarmEnable(timer_1);
                s3_flag = false; //preparant flag timer 3 segons
            }
            else //es tractava d'un rebot
            {
                attachInterrupt(L_BTN_PIN, lButtonInt, CHANGE); //reactivar interrupció  de l'altre botó
            }
        }
        else //timer 3s ha acabat
        {
            s3_flag = true; //flag per timer 3 s activat
            input = DRE_LLARG;
        }

    }

    //Left button interrupt function
    void IRAM_ATTR lButtonInt(void)
    {
        if(digitalRead(L_BTN_PIN) == LOW) //just s'ha apretat
        {
            //desactivar interrupció dos botons
            detachInterrupt(digitalPinToInterrupt(R_BTN_PIN));
            detachInterrupt(digitalPinToInterrupt(L_BTN_PIN));

            //activar timer 100ms
            timerAlarmWrite(timer_1, 1000000, true); //set boundary to 1M: 100 ms
            timerAlarmEnable(timer_1);

            ms100_flag = true; //activar flag
        }

        if(digitalRead(L_BTN_PIN) == HIGH) //s'ha soltat
        {
            if(s3_flag == false) //encara no han passat 3 segons
            {
                input = ESQ_CURT;

            }
            else //ja han passat els 3 segons
            {
                input = ESQ_SOLTAR;
            }
            
            //Activar interrupció de l'altre botó
            attachInterrupt(R_BTN_PIN, rButtonInt, CHANGE);
        }
    }

    //Timer interrupt to manage debouncing and short-long press for right button
    void IRAM_ATTR lTimerInt(void)
    {
        if(ms100_flag == true) //debouncing
        {
            ms100_flag = false; //desactivar flag

            attachInterrupt(L_BTN_PIN, lButtonInt, CHANGE); //reactivar interrupció botó
            if(digitalRead(L_BTN_PIN) == LOW) //botó segueix apretat → debouncing OK
            {
                button_flag = false; //preparant flag botó

                //programar timer per a 3 s
                timerAlarmWrite(timer_1, 30000000, true); //set boundary to 30M: 3 segons
                timerAlarmEnable(timer_1);
                s3_flag = false; //preparant flag timer 3 segons
            }
            else //es tractava d'un rebot
            {
                attachInterrupt(R_BTN_PIN, rButtonInt, CHANGE); //reactivar interrupció  de l'altre botó
            }
        }
        else //timer 3s ha acabat
        {
            s3_flag = true; //flag per timer 3 s activat
            input = ESQ_LLARG;
        }

    }

    //Configures pins used by buttons and activates interrupts
    void init(void)
    {
        //Left button pin configuration and interrupt
        pinMode(L_BTN_PIN, INPUT_PULLUP);
        attachInterrupt(L_BTN_PIN, lButtonInt, CHANGE);

        //Left button pin configuration and interrupt
        pinMode(R_BTN_PIN, INPUT_PULLUP);
        attachInterrupt(R_BTN_PIN, rButtonInt, CHANGE);

        //Configuració pel timer antirebots i  separació curt-llarg
        timer_1 = timerBegin(1, 80, true); //clock source default: 80MHz. divided by 80
        timerAttachInterrupt(timer_1, rTimerInt, true); //set interrupt function to timerInt
    }

}; //namespace buttons