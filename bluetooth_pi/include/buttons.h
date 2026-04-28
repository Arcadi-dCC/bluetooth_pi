#ifndef buttons_H
#define buttons_H

#define L_BTN_PIN 4
#define R_BTN_PIN 2

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

    extern volatile Input input;

    void init(void);

}; //namespace buttons

#endif //buttons_H