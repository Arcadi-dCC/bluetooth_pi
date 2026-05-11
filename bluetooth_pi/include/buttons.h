#ifndef buttons_H
#define buttons_H

#include <platformTypes.h>

#define SWITCH_OPTION_TIME 5e6 //us. Per a submenus de canvi d'hora i alarma
#define TIME_BETWEEN_INCREMENT 250e3 //us. Per incrementar valors d'hora i alarma mantenint el botó esquerre apretat.

namespace buttons{

    enum Input{
        ESQ_CURT,
        ESQ_LLARG,
        ESQ_SOLTAR,
        DRE_CURT,
        DRE_LLARG,
        TIMER,

        TOTAL_INPUTS,
        RES,
        DRE_SOLTAR //No emprada
    };

    uint8 init(void);
    void restartTimer2Countdown(uint64 us);
    
    extern volatile Input input;

}; //namespace buttons

#endif //buttons_H
