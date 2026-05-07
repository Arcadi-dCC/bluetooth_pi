#include <buttons.h>
#include <Arduino.h>

#define L_BTN_PIN 6
#define R_BTN_PIN 2

#define DEBOUNCE_TIME 50e3 //us
#define SORT_TIME 2e6 //us

namespace buttons{

    volatile hw_timer_t *timer_0 = NULL;

    //Prepara i inicia el debouncing: desactiva les interrupcions i fa cridar el timer en DEBOUNCE_TIME us
    void beginDebouncing(void)
    {
        detachInterrupt(digitalPinToInterrupt(R_BTN_PIN)); //desactivem interrupcions per als dos botons
        detachInterrupt(digitalPinToInterrupt(L_BTN_PIN));
        timerWrite((hw_timer_t*)timer_0, 0);
        timerAlarmWrite((hw_timer_t*)timer_0, DEBOUNCE_TIME, false);
        timerAlarmEnable((hw_timer_t*)timer_0);
    }

    void IRAM_ATTR pinISR(void);

    volatile Input input = RES;

        enum Sort{
        FREE,
        L_DEBOUNCE,
        R_DEBOUNCE,
        L_SORT,
        R_SORT,
        L_TIMER_FIRST,
        R_TIMER_FIRST
    };
    
    volatile Sort sort_state = FREE;


    //Rutina de servei a la interrupció pels dos pins associats als botons
    void IRAM_ATTR pinISR(void)
    {
        switch(sort_state)
        {
            case FREE:{ //nothing going. Start debouncing
                if(digitalRead(R_BTN_PIN) == LOW)
                {
                    beginDebouncing();
                    sort_state = R_DEBOUNCE;
                }
                else if(digitalRead(L_BTN_PIN) == LOW)
                {
                    beginDebouncing();
                    sort_state = L_DEBOUNCE;

                };
                break;
            }
            case L_SORT:{ //sorting. Button has been released before time threshold
                if(digitalRead(L_BTN_PIN) == HIGH)
                {
                    input = ESQ_CURT;
                    sort_state = FREE;
                    attachInterrupt(digitalPinToInterrupt(R_BTN_PIN), pinISR, CHANGE); //reactiva interrupció per l'altre pin
                }
                break;
            }
            case R_SORT:{
                if(digitalRead(R_BTN_PIN) == HIGH)
                {
                    input = DRE_CURT;
                    sort_state = FREE;
                    attachInterrupt(digitalPinToInterrupt(L_BTN_PIN), pinISR, CHANGE);
                }
                break;
            }
            case L_TIMER_FIRST:{ //time threshold has passed

                if(digitalRead(L_BTN_PIN) == HIGH)
                {
                    input = ESQ_SOLTAR;
                    sort_state = FREE;
                    attachInterrupt(digitalPinToInterrupt(R_BTN_PIN), pinISR, CHANGE);
                }
                break;
            }
            case R_TIMER_FIRST:{

                if(digitalRead(R_BTN_PIN) == HIGH)
                {
                    input = DRE_SOLTAR;
                    sort_state = FREE;
                    attachInterrupt(digitalPinToInterrupt(L_BTN_PIN), pinISR, CHANGE);
                }
                break;
            }
            default:{
                /*Do nothing*/
            }
        }

    }

    //Rutina de servei a la interrupció pel timer 0
    void IRAM_ATTR timer0ISR(void)
    {
        switch(sort_state){
            case L_DEBOUNCE:{ //debounce time finished.
                if(digitalRead(L_BTN_PIN) == LOW) //Check and begin sorting time...
                {
                    sort_state = L_SORT;
                    timerWrite((hw_timer_t*)timer_0, 0);
                    timerAlarmWrite((hw_timer_t*)timer_0, SORT_TIME, false);
                    timerAlarmEnable((hw_timer_t*)timer_0);
                }
                else //...or return to initial state.
                {
                    sort_state = FREE;
                    attachInterrupt(digitalPinToInterrupt(R_BTN_PIN), pinISR, CHANGE);
                }
                attachInterrupt(digitalPinToInterrupt(L_BTN_PIN), pinISR, CHANGE);
                break;
            }
            case R_DEBOUNCE:{ //debounce time finished.
                if(digitalRead(R_BTN_PIN) == LOW) //Check and begin sorting time...
                {
                    sort_state = R_SORT;
                    timerWrite((hw_timer_t*)timer_0, 0);
                    timerAlarmWrite((hw_timer_t*)timer_0, SORT_TIME, false);
                    timerAlarmEnable((hw_timer_t*)timer_0);
                }
                else //...or return to initial state.
                {
                    sort_state = FREE;
                    attachInterrupt(digitalPinToInterrupt(L_BTN_PIN), pinISR, CHANGE);
                }
                attachInterrupt(digitalPinToInterrupt(R_BTN_PIN), pinISR, CHANGE);
                break;
            }
            case L_SORT:{ //sorting. Time threshold has been reached without button being released
                if(digitalRead(L_BTN_PIN) == LOW)
                {
                    sort_state = L_TIMER_FIRST;
                    input = ESQ_LLARG;
                }
                break;
            }
            case R_SORT:{ //sorting. Time threshold has been reached without button being released
                if(digitalRead(R_BTN_PIN) == LOW)
                {
                    sort_state = R_TIMER_FIRST;
                    input = DRE_LLARG;
                }
                break;
            }
            default:{
                /*Do nothing*/
            }
        }
    }


    //Configura els pins, interrupcions i timers emprats pels botons
    //Return: 0-OK 1-error
    uint8 init(void)
    {
        //Inicialització del timer 0 per controlar debouncing i distingir entre apretat curt/llarg/soltar
        timer_0 = timerBegin(0, 80, true);
        if(timer_0 == NULL)
        {
            return 1;
        }
        timerAttachInterrupt((hw_timer_t*)timer_0, timer0ISR, true);

        input = RES;
        sort_state = FREE;

        //Inicialització de pin i interrupció per botó esquerre.
        pinMode(L_BTN_PIN, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(L_BTN_PIN), pinISR, CHANGE);

        //Inicialització de pin i interrupció per botó dret.
        pinMode(R_BTN_PIN, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(R_BTN_PIN), pinISR, CHANGE);

        return 0;
    }

}; //namespace buttons
