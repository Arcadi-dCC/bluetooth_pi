#include <buttons.h>
#include <Arduino.h>

namespace buttons{

    enum Input{
        ESQ_CURT,
        ESQ_LLARG,
        ESQ_SOLTAR,
        DRE_CURT,
        DRE_LLARG,
        TIMER,

        TOTAL_INPUTS,
        RES
    };

    volatile Input input;

    //Configures pins used by buttons and activates interrupts
    void init(void)
    {
        //Left button pin configuration and interrupt
        pinMode(L_BTN_PIN, INPUT_PULLUP);
        attachInterrupt(L_BTN_PIN, lButtonInt, CHANGE);

        //Left button pin configuration and interrupt
        pinMode(R_BTN_PIN, INPUT_PULLUP);
        attachInterrupt(R_BTN_PIN, rButtonInt, CHANGE);
    }

    //Right button interrupt function
void IRAM_ATTR rButtonInt(void)
{
    if(digitalRead(R_BTN_PIN) == LOW)
    {
        //begin timer
    }
    if(digitalRead(R_BTN_PIN) == HIGH /*&& timer < 3s*/)
    {
        input = DRE_CURT;
    }
}

//Right button interrupt function
void IRAM_ATTR lButtonInt(void)
{
    if(digitalRead(L_BTN_PIN) == LOW)
    {
        //begin timer
    }
    if(digitalRead(L_BTN_PIN) == HIGH /*&& timer < 3s*/)
    {
        input = ESQ_CURT;
    }
}

}; //namespace buttons