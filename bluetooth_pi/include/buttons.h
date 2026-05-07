#ifndef buttons_H
#define buttons_H

#include <platformTypes.h>

namespace buttons{

    enum Input{
        ESQ_CURT,
        ESQ_LLARG,
        ESQ_SOLTAR,
        DRE_CURT,
        DRE_LLARG,
        DRE_SOLTAR,
        //TIMER,

        TOTAL_INPUTS,
        RES
    };

    enum Sort{
        FREE,
        L_DEBOUNCE,
        R_DEBOUNCE,
        L_SORT,
        R_SORT,
        L_TIMER_FIRST,
        R_TIMER_FIRST
    };

    uint8 init(void);
    
    extern volatile Input input;

}; //namespace buttons

#endif //buttons_H
