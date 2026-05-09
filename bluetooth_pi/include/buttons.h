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
        TIMER,

        TOTAL_INPUTS,
        RES,
        DRE_SOLTAR //No emprada
    };

    uint8 init(void);
    
    extern volatile Input input;

}; //namespace buttons

#endif //buttons_H
